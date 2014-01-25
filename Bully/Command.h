#pragma once

#include <string>

using namespace std;

class Command
{
public:
	Command(string message);
	~Command();
	
	void setMessage(string message);
	bool isValid();

	unsigned int processId;
	string command;

private:
	void displayMessage();

	bool m_isValid;
};

