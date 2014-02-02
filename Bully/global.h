#pragma once

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#define PORT 8888 

#define TIMEOUT_MS 1000

#define DEFAULT_BUFLEN 1024


#define COORDINATOR "1"

#define ELECTION "2"

#define TCP_CONNECT "3"

#define COUNT "4"

#define COUNT_RESPONSE "5"

#define ARRAY_LENGTH 10

#define DEFAULT_BUFLEN 1024

static inline string convertIntToString(const int& value)
{
	stringstream stream;
	stream << value;
	return stream.str();
}