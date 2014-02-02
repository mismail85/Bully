#pragma once

#include <WinSock2.h>
#include <string.h>
#include <iostream>

using namespace std;

class SocketManager
{
public:
	SocketManager(void);
	virtual ~SocketManager(void);
	void send(const string& message );
	string receive();
	void setReceiveTimeout(unsigned int timeout);

private:
	bool initialize();
	void initSendSocket();
	void initReceiveSocket();

	WSADATA m_wsaData;
	SOCKET m_sendSocket;
	SOCKET m_receiveSocket;
};

