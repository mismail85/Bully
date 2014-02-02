#include "stdafx.h"
#include "Process.h"
#include "global.h"
#include "Command.h"
#include "Clientthread.h"
#include "ServerThread.h"
#include "Task.h"

#include <thread>

unsigned int Process::processCount = 0;
Task Process::task = Task();

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
	cout << "I am electing my self" << endl;
	m_socketManager.setReceiveTimeout( ELECTION_TIMEOUT_MS);
	while(TRUE){

		sendMessage( string(ELECTION) );

		string message = m_socketManager.receive();

		Command command(message);
		if(command.processId == m_processId)
			message = m_socketManager.receive();

		if(message.empty())
			return true;

		command.setMessage(message);
		if(command.isValid() && (command.command == COORDINATOR) && (command.processId > m_processId))
			return false;
		if(command.isValid() && (command.command == ELECTION) && (command.processId > m_processId))
			return false;
	}
}

void Process::enterCommandMode()
{
	cout << "I am the coordinator ===>" <<m_processId << endl;

	bool finishedCount = false;
	m_socketManager.setReceiveTimeout( COMMAND_TIMEOUT);

	_beginthreadex(0, 0 , waitForStart, this, 0, 0);

	while(TRUE){
		sendMessage( string(COORDINATOR) );	
		string message = m_socketManager.receive();

		Command command(message, false);
		if(command.isValid() && (command.command == COORDINATOR) && (command.processId == m_processId)){
			message = m_socketManager.receive();
			command.setMessage(message);
		}

		if( command.isValid() ){

			if((command.command == ELECTION) && (command.processId > m_processId)){
				return;
			}

			if(command.command == COUNT_RESPONSE){
				++ Process::processCount;
				finishedCount = true;
			}
		}
		else{
			if(finishedCount){
				finishedCount = false;
				_beginthreadex(0, 0, serverThread, 0, 0, 0);
				
				sendMessage( string(TCP_CONNECT ), getMyIp());
			}
		}
	}
}

void Process::enterSlaveryMode()
{
	cout << "I am Slave ===>"<<m_processId << endl;

	m_socketManager.setReceiveTimeout( SLAVE_TIMEOUT);
	bool stopElecting = true;
	while(TRUE){
		string message = m_socketManager.receive();

		if(message.empty()){
			return;
		}

		Command command(message);

		if(command.isValid()){
			if((command.command == COORDINATOR) && (command.processId > m_processId))
				continue;
			/*if((command.command == ELECTION) && (command.processId > m_processId))
				stopElecting = false;
			if((command.command == ELECTION) && (command.processId < m_processId) && !stopElecting){
				return;
			}*/
			if(command.command == TCP_CONNECT){
				cout << "tcp connect command received" << endl;
				char ipBuf[DEFAULT_BUFLEN];
				strcpy_s(ipBuf, command.data.c_str());
				_beginthreadex(0, 0, clientThread, (void*)ipBuf, 0, 0);
			}
			if( command.command == COUNT){
				sendMessage(string(COUNT_RESPONSE));
			}
		}

	}
}

void Process::sendMessage(const string & message, const string & data)
{
	m_socketManager.send(message + ":" + convertIntToString(m_processId) + ":" + data);
}

string Process::getMyIp()
{
	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		cerr << "Error " << WSAGetLastError() <<
			" when getting local host name." << endl;
		return string();
	}

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) {
		cerr << "Yow! Bad host lookup." << endl;
		return string();
	}

	struct in_addr addr;
	memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
	return string( inet_ntoa(addr) );    
}