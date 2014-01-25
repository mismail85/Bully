#pragma once

#include <Windows.h>
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

private:
	bool initialize();
	void initSendSocket();
	void initReceiveSocket();

	WSADATA m_wsaData;
	SOCKET m_sendSocket;
	SOCKET m_receiveSocket;
};

