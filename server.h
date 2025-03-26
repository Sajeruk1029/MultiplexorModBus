#ifndef SERVER_H
#define SERVER_H

	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <signal.h>
	#include <errno.h>

	#include <unistd.h>

	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/file.h>
	#include <sys/time.h>

	#include <arpa/inet.h>
	#include <netinet/in.h>

	#include <modbus.h>

	static	unsigned int	g_connections;
	
	#pragma pack(push, 1)

		typedef struct
		{
			unsigned int	_serverPort;
			unsigned int	_serverMaxUsers;

			modbus_t*	_mbServer;

			int	_serverSocket;

			char*	_serverIp;

			char*	_device;

			int	_baud;

			char	_parity;

			int	_dataBit;
			int	_stopBit;

			int	_debug;
		} Server;

	#pragma pack(pop)

		/*
		 * Метод serverInit: Подготовка сетевой части серверной структры и сокета.
		 * 
		 * В случае успеха будет возвращено 0.
		 * 
		 * В случае неудачи будет возвращено -1.
		*/

	int	serverInit(Server*	server, unsigned int	port, unsigned int	maxUsers, char*	serverIp, int	debug);

		/*
		 * Метод hardwareInit: Подготовка настроек порта серверной структуры.
		*/

	void	hardwareInit(Server*	server, char*	device, int	baud, char	parity, int	stopBit, int	dataBit);

		/*
		 * Метод serverStart: Запуск сервера.
		 * 
		 * В случае успеха возврат не должен быть достигнут.
		 * 
		 * В случае, если серверный сокет не был подготовлен будет возвращено -3.
		*/

	int	serverStart(Server*	server);

		/*
		 * Метод handlerChild: Обработчик завершения дочерних процессов.
		*/

	void	handlerChild(int	sig, siginfo_t*	info, void*	context);

#endif
