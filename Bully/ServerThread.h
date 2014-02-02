#pragma once

#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>

#include "Task.h"
#include "SocketManager.h"
#include "global.h"

#define DEFAULT_PORT 2222

unsigned int _stdcall doWork(void* data)
{
	int iResult = 0;

	char recvbuf[DEFAULT_BUFLEN];
	memset(recvbuf, 0, DEFAULT_BUFLEN);

	SOCKET *s = (SOCKET*) data;
	SOCKET ClientSocket = *s;
	delete s;

	string task = Process::task.next() + "\\";
	
	iResult = send(ClientSocket, task.c_str(), task.length(), 0);

	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);

		return 1;
	}
	
	iResult = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
	if (iResult == SOCKET_ERROR) {
		printf("could not receive data: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		return 1;
	}
	
	cout << "received result = " << recvbuf << endl;		

	Process::task.addResult( atoi(recvbuf) );

	closesocket(ClientSocket);
	return 0;
}

unsigned int _stdcall serverThread(void *data)
{
	cout << "server started!!!!" << endl;

	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;

	// Create a SOCKET for connecting to server
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());

		return 1;
	}

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(DEFAULT_PORT);
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	// Setup the TCP listening socket
	iResult = ::bind( ListenSocket, (SOCKADDR *) &address, sizeof(address));
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());

		closesocket(ListenSocket);

		return 1;
	}

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);

		return 1;
	}

	Process::task.splitTask(Process::processCount);

	while(true){
		SOCKET * ClientSocket = new SOCKET;
		*ClientSocket = accept(ListenSocket, NULL, NULL);

		if(*ClientSocket == INVALID_SOCKET){
			cout << "invalid socket : " << WSAGetLastError()<<endl;
			continue;
		}
		
		_beginthreadex(0, 0, doWork, (void*)ClientSocket, 0, 0);
	}

	closesocket(ListenSocket);	

	return 0;
}

unsigned int _stdcall waitForStart(void *data)
{

	Process *process = (Process*) data;
	
	string str;
	cout << "Enter \"start\" to start the task:"<< endl;
	while(true){
		cin >> str;

		if (str == "start"){
			Process::processCount = 0;
			process->sendMessage( string(COUNT ));
			break;
		}
	}
	return 0;
}