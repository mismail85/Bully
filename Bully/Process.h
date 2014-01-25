#pragma once
#include "SocketManager.h"

class Process
{
public:
	Process(void);
	virtual ~Process(void);
	void initElection();

private:
	void sendProcessId();
	void enterSlaveryMode();
	void enterCommandMode();
		
	SocketManager m_socketManager;
	unsigned int m_processId;
};

