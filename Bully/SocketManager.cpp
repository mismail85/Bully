#include "stdafx.h"
#include "SocketManager.h"
#include "global.h"

SocketManager::SocketManager(void)
{
	initialize();
	initSendSocket();
	initReceiveSocket();
}

SocketManager::~SocketManager(void)
{
	closesocket(m_sendSocket);
	closesocket(m_receiveSocket);
	WSACleanup();
}

bool SocketManager::initialize()
{
	if (WSAStartup(MAKEWORD(2,2), &m_wsaData) != 0) {
		printf("WSAStartup failed: %d\n", WSAGetLastError());
		return false;
	}
	printf("Initialised.\n");
	return true;
}

void SocketManager::initSendSocket()
{
	if((m_sendSocket = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}
	printf("Socket created.\n");

		int broadcastValue = 1;
	if(setsockopt(m_sendSocket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastValue, sizeof(broadcastValue)) == SOCKET_ERROR)
		cout<< "could not set socket option";
}

void SocketManager::send(const string& message )
{

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_BROADCAST;
	address.sin_port = htons( (unsigned short) PORT );

	if (sendto(m_sendSocket, message.c_str(), message.length(), 0, (struct sockaddr*) &address, sizeof(address)) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

void SocketManager::initReceiveSocket()
{

	if((m_receiveSocket = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	sockaddr_in RecvAddr;
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(PORT);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int broadcastValue = 1;
	if(setsockopt(m_receiveSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&broadcastValue, sizeof(broadcastValue)) == SOCKET_ERROR)
		cout<< "could not set socket option";

	static int timeout = TIMEOUT_MS;
	setsockopt(m_receiveSocket, SOL_SOCKET, SO_RCVTIMEO,(char*)&timeout,sizeof(timeout));

	int iResult = 0;
	iResult = bind(m_receiveSocket, (SOCKADDR *) & RecvAddr, sizeof (RecvAddr));
	if (iResult != 0) {
		wprintf(L"bind failed with error %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

string SocketManager::receive()
{
	int iResult = 0;
	char RecvBuf[1024];
	int BufLen = 1024;

	sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof (SenderAddr);

	iResult = recvfrom(m_receiveSocket, RecvBuf, BufLen, 0, (SOCKADDR *) & SenderAddr, &SenderAddrSize);
	if(WSAGetLastError() == WSAETIMEDOUT)
			return string();

	if (iResult == SOCKET_ERROR) {
		wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	RecvBuf[iResult] = '\0';
	return string(RecvBuf);
}
