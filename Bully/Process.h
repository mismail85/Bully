#pragma once
#include "SocketManager.h"

class Process
{
public:
	Process(void);
	virtual ~Process(void);
	void initElection();

private:
	void enterSlaveryMode();
	void enterCommandMode();
	bool enterElectionMode();

	SocketManager m_socketManager;
	unsigned int m_processId;
};

