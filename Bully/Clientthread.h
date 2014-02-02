#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include "global.h"

#define DEFAULT_PORT 2222

int findMinNumber(const string & numbers)
{
	int min = -1;
	int pos = 0;
	int prevPos = 0;
	do{
		pos = numbers.find(',', pos);
		if(pos != -1){
			string number = numbers.substr(prevPos, pos - prevPos);
			int iNumber = atoi(number.c_str());

			if(iNumber < min || min == -1){
				min = iNumber;
				cout << "pos = " << pos<< endl;
				cout << "min =" << min << endl;
			}
			prevPos = pos + 1;
			++pos;
		}
	}while(pos  != -1);
	return min;
}

unsigned int _stdcall clientThread(void *data)
{
	cout << "client thread started!" << endl;

	SOCKET ConnectSocket = INVALID_SOCKET;

	char recvbuf[DEFAULT_BUFLEN];
	int iResult;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());

		return 1;
	}

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(DEFAULT_PORT);
	address.sin_addr.s_addr = inet_addr ((char*)data);

	iResult = connect(ConnectSocket, (SOCKADDR*)&address, sizeof(address));

	if(iResult == SOCKET_ERROR){
		printf("unable to connect to server: %d\n", WSAGetLastError());
		return 1;
	}

	if(ConnectSocket == INVALID_SOCKET)
		cout << "socket is invalid" << endl;

	string numbers;

	do{
		memset(recvbuf, 0, DEFAULT_BUFLEN);
		iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN-1, 0);

		if ( iResult == SOCKET_ERROR){
			cout << "could not receive data!!!" <<  endl;
			closesocket(ConnectSocket);
			return 1;
		}

		char *ptr = strstr(recvbuf, "\\");
		if(ptr){
			*ptr = '\0';
			numbers.append(recvbuf);
			break;
		}
		numbers.append(recvbuf);
	}while(iResult > 0);
	
	cout << "received data = " << numbers << endl;	
	int min = findMinNumber(numbers);

	string minNumber = convertIntToString(min);

	// Send an initial buffer
	iResult = send( ConnectSocket, minNumber.c_str(), minNumber.length(), 0 );
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);

		return 1;
	}

	cout << "value sent =" << min << endl;

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		return 1;
	}

	closesocket(ConnectSocket);
	return 0;
}
