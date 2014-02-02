#include "stdafx.h"
#include "Command.h"
#include <iostream>
#include "global.h"
#include <time.h> 

Command::Command(const string & message, bool print)
	:m_isValid(false)
{
	setMessage(message, print);
}


Command::~Command()
{
}

void Command::displayMessage()
{
	string type;
	if(command == COORDINATOR)
		return;
		//type = "Coordinator";
	else if(command == ELECTION)
		type = "ELECTION";
	 else if(command == TCP_CONNECT)
		type = "TCP Connect";

	time_t currentTime;
	time(&currentTime);
	char buf[1024];
	ctime_s(buf, 1024, &currentTime);
	cout << type << "     processId = "<<processId << "      time = "<< buf << endl;
}

void Command::setMessage(const string & message, bool print)
{
	int pos = message.find(':');

	if(pos != -1){
		command = message.substr(0, pos);

		int fpos = message.find(':', pos + 1);
		if(fpos != -1){
			processId = atol(message.substr(pos +1 , fpos).c_str());
			data = message.substr(fpos+1);
		}
		if(print)
			displayMessage();
		m_isValid= true;
		return;
	}
	m_isValid = false;
}

bool Command::isValid()
{
	return m_isValid;
}
