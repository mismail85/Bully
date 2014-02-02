#pragma once

#include <string>
#include <vector>

using namespace std;

class Task
{
public:
	Task();
	~Task(void);
	void addResult(const int result);
	string next();
	void splitTask(const int processCount);

	string generateNumbers();

private:
	
	vector<int> m_results;	
	vector<int> m_remainingItems;
	vector<string> m_splitedTasks;
	int m_current;
};

