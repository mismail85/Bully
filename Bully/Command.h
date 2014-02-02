#pragma once

#include <string>

using namespace std;

class Command
{
public:
	Command(const string & message, bool print = true);
	~Command();
	
	void setMessage(const string & message, bool print = true);
	bool isValid();

	unsigned int processId;
	string command;
	string data;

private:
	void displayMessage();

	bool m_isValid;
};

