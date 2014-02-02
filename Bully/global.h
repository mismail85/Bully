#pragma once

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#define PORT 8888 

//Time out
#define ELECTION_TIMEOUT_MS 10
#define SLAVE_TIMEOUT 1000
#define COMMAND_TIMEOUT (SLAVE_TIMEOUT - 100)

// Commands
#define COORDINATOR "1"

#define ELECTION "2"

#define TCP_CONNECT "3"

#define COUNT "4"

#define COUNT_RESPONSE "5"


//Constants
#define ARRAY_LENGTH 1000

#define DEFAULT_BUFLEN 1024

static inline string convertIntToString(const int& value)
{
	stringstream stream;
	stream << value;
	return stream.str();
}