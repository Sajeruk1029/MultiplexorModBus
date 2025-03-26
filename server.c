#include "server.h"

void	handlerChild(int	sig, siginfo_t*	info, void*	context){ --g_connections; }

int	serverInit(Server*	server, unsigned int	port, unsigned int	maxUsers, char*	serverIp, int	debug)
{
	server->_serverPort = port;
	server->_serverMaxUsers = maxUsers;
	server->_serverIp = serverIp;

	server->_debug = debug;

	server->_mbServer = modbus_new_tcp(server->_serverIp, server->_serverPort);

	if(!server->_mbServer){ return -1; }

	return 0;
}

int	serverStart(Server*	server)
{
	pid_t	pid = 0;

	int	sock = 0;

	struct	sigaction act;

	g_connections = 0;

	server->_serverSocket = modbus_tcp_listen(server->_mbServer, server->_serverMaxUsers);

	if(server->_serverSocket == -1)
	{
		modbus_free(server->_mbServer);

		server->_mbServer = 0;

		return -3;
	}

	memset(&act, 0, sizeof(act));

	act.sa_flags = SA_NOCLDWAIT;
	act.sa_sigaction = handlerChild;

	sigaction(SIGCHLD, &act, 0);

	if(server->_debug){ printf("%s\n", "Server has been started...\0"); }

	while(1)
	{
		if((sock = accept(server->_serverSocket, 0, 0)) != -1)
		{
			if(g_connections == server->_serverMaxUsers)
			{
				close(sock);

				continue;
			}

			++g_connections;

			pid = fork();

			if(pid == -1)
			{
				if(server->_debug){ printf("%s\n", strerror(errno)); }
			}
			else if(pid == 0)
			{
				modbus_t*	mbHardware = 0;

				unsigned char	buf[MODBUS_MAX_ADU_LENGTH];
				unsigned char	answer[MODBUS_MAX_ADU_LENGTH];

				int	result = 0;
				int	result2 = 0;
				int result3 = 0;

				short	length = 0;

				char	slaveId = 0;

				int	headerLength = 0;
				int	hardwareFd = 0;

				mbHardware = modbus_new_rtu(server->_device, server->_baud, server->_parity, server->_dataBit, server->_stopBit);

				if(server->_debug){ modbus_set_debug(mbHardware, 1); }

				close(server->_serverSocket);

				if(!mbHardware)
				{
					if(server->_debug){ printf("%s\n", modbus_strerror(errno)); }

					exit(0);
				}

				if(modbus_connect(mbHardware) == -1)
				{
					if(server->_debug){ printf("%s\n", modbus_strerror(errno)); }

					modbus_close(mbHardware);
					modbus_free(mbHardware);

					exit(0);
				}

				hardwareFd = modbus_get_socket(mbHardware);

				if(hardwareFd == -1)
				{
					if(server->_debug){ printf("%s\n", strerror(errno)); }

					modbus_close(mbHardware);
					modbus_free(mbHardware);

					exit(0);
				}

				modbus_set_socket(server->_mbServer, sock);

				if(server->_debug){ modbus_set_debug(server->_mbServer, 1); }

				while(result != -1)
				{
					memset(buf, 0, MODBUS_MAX_ADU_LENGTH);
					memset(answer, 0, MODBUS_MAX_ADU_LENGTH);

					result = modbus_receive(server->_mbServer, buf);

					if(flock(hardwareFd, LOCK_EX) != 0)
					{
						if(server->_debug){ printf("%s\n", strerror(errno)); }

						exit(0);
					}
					
					if(result != -1)
					{
						headerLength = modbus_get_header_length(server->_mbServer);

						memcpy(answer, buf, 4);

						//length = (*(buf + headerLength  - 3) << 8) | *(buf + headerLength - 2);
						//slaveId = *(buf + headerLength - 1);

						memcpy(&length, (buf + headerLength - 2), 2);
						memcpy(&slaveId, (buf + headerLength - 1), 1);

						modbus_set_slave(mbHardware, slaveId);

						if(modbus_send_raw_request(mbHardware, (buf + headerLength - 1), length) == -1){ printf("%s\n", modbus_strerror(errno)); } 

						if((result2 = modbus_receive_confirmation(mbHardware, answer + headerLength - 1)) == -1)
						{
							if(server->_debug){ printf("%s\n", modbus_strerror(errno)); }
							break;
						}

						if(flock(hardwareFd, LOCK_UN) != 0)
						{
							if(server->_debug){ printf("%s\n", strerror(errno)); }
							exit(-1);
						}

						*(answer + 4) = (((result2 - 2) & 0xFF00) >> 8);
						*(answer + 5) = ((result2 - 2) & 0xFF);

						if((result3 = (write(sock, answer, result2 + headerLength - 1 - 2))) == -1) 
						{
							if(server->_debug){ printf("%s\n", modbus_strerror(errno)); }
							break;
						}

						modbus_close(mbHardware);
					}
					else
					{
						break;
					}
				}

				close(sock);
				modbus_close(mbHardware);
				modbus_free(mbHardware);

				exit(0);
			}
			else{ close(sock); }
		}
		else
		{ 
			if(server->_debug){ printf("%s\n", strerror(errno)); }
		}
	}

	return 0;
}

void	hardwareInit(Server* server, char* device, int baud, char parity, int stopBit, int dataBit)
{
	server->_device = device;
	server->_baud = baud;
	server->_parity = parity;
	server->_dataBit = dataBit;
	server->_stopBit = stopBit;
}
