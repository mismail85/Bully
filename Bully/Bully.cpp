// Bully.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Process.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Process process;

	while(TRUE){
		process.initElection();
	}

	return 0;
}

