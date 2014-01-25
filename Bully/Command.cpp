#include "stdafx.h"
#include "Command.h"
#include <iostream>

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
	cout << command << "     "<<processId << endl;
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
