#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "conf.h"
#include "server.h"

#define	DEVICE	"Device\0"
#define	IP	"Ip\0"
#define	PORT	"Port\0"
#define	MAXUSERS	"MaxUsers\0"
#define	DATABIT	"DataBit\0"
#define	STOPBIT	"StopBit\0"
#define	PARITY	"Parity\0"
#define	BAUD	"Baud\0"
#define	DEBUG	"Debug\0"

	/*
		* Метод help: Вывод справки.
		* 
		* Требуется передача запускного аргумента.
	*/

void	help(const char*	nameProg)
{
	printf("%s%s%s\n", "Usage: ", nameProg, " [keys] <args>");
	printf("%s\n", "-f -- path to config file.");
	printf("%s\n", "-h -- Help.");
}

	/*
		* Метод main: Метод входа.
	*/

int main(int	argc, char**	argv)
{
	char*	buf = 0;

	char*	ip = 0;
	char*	device = 0;
	char*	prop = 0;

	char	keys[9][10] = { DEVICE, IP, PORT, MAXUSERS, DATABIT, STOPBIT, PARITY, BAUD, DEBUG };

	char	parity = 0;

	int	port = 0;
	int	maxUsers = 0;
	int	dataBit = 0;
	int	stopBit = 0;
	int	baud = 0;
	int	debug = 0;

	FILE*	file = 0;

	Server server;

	memset(&server, 0, sizeof(Server));

	if((argc >= 3) && ((strcmp(*(argv + 1), "-f") == 0))){ prop = *(argv + 2); }
	else
	{
		help(*(argv + 0));
		return 0;
	}

	if(existProp(prop) != 0)
	{
		if(openFileAppend(prop, &file) != 0)
		{
			printf("%s\n", strerror(errno));

			return -1;
		}

		if(createProp(prop) != 0)
		{
			printf("%s\n", strerror(errno));

			return -2;
		}

		for(size_t	counter = 0; counter < 9; ++counter)
		{
			if(addKeyAndValueStringOpt(file, *(keys + counter), "") != 0)
			{
				printf("%s\n", strerror(errno));

				return -3;
			}
		}

		if(closeFile(file) != 0)
		{
			printf("%s\n", strerror(errno));

			return -4;
		}

		printf("%s\n", "Config file has been created!\0");
		return 0;
	}

	if(contentProp(prop, &buf, sizeProp(prop)) != 0)
	{
		printf("%s\n", strerror(errno));
		return -5;
	}

	if(getStringValue(&buf, DEVICE, &device) != 0)
	{
		if(errno == 0)
		{
			printf("%s%s%s\n", "Parametr \0", DEVICE, " is not found!\0");
		}
		else
		{
			printf("%s\n", strerror(errno));
		}

		free(buf);

		return -6;
	}

	if(getStringValue(&buf, IP, &ip) != 0)
	{
		if(errno == 0)
		{
			printf("%s%s%s\n", "Parametr \0", IP, " is not found!\0");
		}
		else
		{
			printf("%s\n", strerror(errno));
		}

		free(buf);

		free(device);

		return -6;
	}

	if(getCharValue(&buf, PARITY, &parity) != 0)
	{
		printf("%s%s%s\n", "Parametr \0", PARITY, " is not found!\0");

		free(buf);

		free(ip);
		free(device);
		
		return -7;
	}

	if(getIntValue(&buf, PORT, &port) != 0)
	{
		printf("%s%s%s\n", "Parametr \0", PORT, " is not found!\0");

		free(buf);

		free(ip);
		free(device);

		return -8;
	}

	if(getIntValue(&buf, MAXUSERS, &maxUsers) != 0)
	{
		printf("%s%s%s\n", "Parametr \0", MAXUSERS, " is not found!\0");

		free(buf);

		free(ip);
		free(device);

		return -8;
	}

	if(getIntValue(&buf, DATABIT, &dataBit) != 0)
	{
		printf("%s%s%s\n", "Parametr \0", DATABIT, " is not found!\0");

		free(buf);

		free(ip);
		free(device);

		return -8;
	}

	if(getIntValue(&buf, STOPBIT, &stopBit) != 0)
	{
		printf("%s%s%s\n", "Parametr \0", STOPBIT, " is not found!\0");

		free(buf);

		free(ip);
		free(device);

		return -8;
	}

	if(getIntValue(&buf, BAUD, &baud) != 0)
	{
		printf("%s%s%s\n", "Parametr \0", BAUD, " is not found!\0");

		free(buf);

		free(ip);
		free(device);

		return -8;
	}

	if(getIntValue(&buf, DEBUG, &debug) != 0)
	{
		printf("%s%s%s\n", "Parametr \0", DEBUG, " is not found!\0");

		free(buf);

		free(ip);
		free(device);

		return -8;
	}

	free(buf);

	if(serverInit(&server, port, maxUsers, ip, debug) != 0)
	{
		printf("%s\n", modbus_strerror(errno));

		free(ip);
		free(device);

		return -9;
	}

	hardwareInit(&server, device, baud, parity, dataBit, stopBit);

	if(serverStart(&server) != 0)
	{
		printf("%s\n", strerror(errno));

		free(ip);
		free(device);

		return -10;
	}

	free(ip);
	free(device);

	return 0;
}
