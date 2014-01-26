#include "stdafx.h"
#include "Process.h"
#include "global.h"
#include "Command.h"

Process::Process(void)
{
	m_processId = (unsigned int) GetCurrentProcessId();

	cout << "Process Id= "<< m_processId << endl;
}


Process::~Process(void)
{
}

void Process::initElection()
{
	
	if(enterElectionMode()){

		enterCommandMode();
	}
	
	enterSlaveryMode();
}

bool Process::enterElectionMode()
{
	while(TRUE){
		m_socketManager.send( string(ELECTION) + ":" +convertIntToString(m_processId) );

		string message = m_socketManager.receive();

		Command command(message);
		if(command.processId == m_processId)
			message = m_socketManager.receive();

		if(message.empty())
			return true;

		command.setMessage(message);
		if(command.isValid() && (command.command == COORDINATOR) && (command.processId > m_processId))
			return false;
		if(command.isValid() && (command.command == ELECTION))
			if(command.processId > m_processId)
				return false;
	}
}

void Process::enterCommandMode()
{
	cout << "I am the coordinator ===>" <<m_processId << endl;

	while(TRUE){
		m_socketManager.send( string(COORDINATOR) + ":" +convertIntToString(m_processId) );	
		string message = m_socketManager.receive();

		message = m_socketManager.receive();

		Command command(message);
		if(command.isValid() && (command.command == ELECTION) )
			return;
	}
}

void Process::enterSlaveryMode()
{
	cout << "I am Slave ===>"<<m_processId << endl;
	while(TRUE){
		string message = m_socketManager.receive();

		if(message.empty())
			return;

		Command command(message);
		if(command.isValid() && (command.command == ELECTION) && (command.processId < m_processId))
			return;
	}
}

