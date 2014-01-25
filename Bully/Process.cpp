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
	m_socketManager.send( string(ELECTION) + ":" +convertIntToString(m_processId) );

	string message = m_socketManager.receive();

	Command command(message);
	if(command.processId == m_processId)
		message = m_socketManager.receive();

	command.setMessage(message);
	cout << message;
	if(message.empty()){
				
		enterCommandMode();
	}
	else if( command.processId < m_processId){
		string message = m_socketManager.receive();
			if(message.empty())
				enterCommandMode();
		return;
	}
	else if( command.processId > m_processId){
		cout << "I am Slave"<< endl;
		enterSlaveryMode();
	}
	cout << "end of initelection"<<endl;
}


void Process::enterCommandMode()
{
	cout << "I am the coordinator " << endl;
	while(TRUE){
		m_socketManager.send( string(COORDINATOR) + ":" +convertIntToString(m_processId) );
		string message = m_socketManager.receive();

			Command command(message);
				
			if(command.command == COORDINATOR)
				message = m_socketManager.receive();
			command.setMessage(message);
			if(command.isValid() && command.command == ELECTION && command.processId > m_processId)
				return;
		}
}

void Process::enterSlaveryMode()
{
	while(TRUE){
		string message = m_socketManager.receive();
		Command command(message);
		if(command.isValid() && command.command == ELECTION && command.processId > m_processId){
			return;
		}
	}
}

void Process::sendProcessId()
{
	m_socketManager.send( convertIntToString(m_processId) );
}
