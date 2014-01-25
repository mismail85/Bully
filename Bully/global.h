#pragma once

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#define PORT 8888 

#define TIMEOUT_MS 1000

#define COORDINATOR "1"

#define ELECTION "2"

#define ALIVE "3"


static inline string convertIntToString(const int& value)
{
	stringstream stream;
	stream << value;
	return stream.str();
}