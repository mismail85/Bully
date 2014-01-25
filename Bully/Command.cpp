#include "stdafx.h"
#include "Command.h"
#include <iostream>
#include "global.h"
#include <time.h> 

Command::Command(string message)
	:m_isValid(false)
{
	setMessage(message);
}


Command::~Command()
{
}

void Command::displayMessage()
{
	string type;
	if(command == COORDINATOR)
		type = "Coordinator";
	else if(command == ELECTION)
		type = "ELECTION";

	time_t currentTime;
	time(&currentTime);
	char buf[1024];
	ctime_s(buf, 1024, &currentTime);
	cout << type << "     processId = "<<processId << "      time = "<< buf << endl;
}

void Command::setMessage(string message)
{
	int pos = message.find(':');
	if(pos != -1){

		processId = atol(message.substr(pos+1).c_str());
		command = message.substr(0, pos);

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
