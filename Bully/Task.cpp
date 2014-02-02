#include "stdafx.h"
#include "Task.h"
#include "global.h"
#include "Process.h"

Task::Task()
	:m_current(0)
{
}

Task::~Task(void)
{
}

void Task::addResult(const int result)
{
	//TODO : gaurd this function with mutex
	m_results.push_back( result);

	if(m_results.size() == Process::processCount){
		m_results.insert(m_results.begin(), m_remainingItems.begin(), m_remainingItems.end());
		int min = m_results[0];
		for(int i = 0; i < m_results.size(); i++){
			if(m_results[i] < min)
				min = m_results[i];
		}
		cout << "The minimum Value is = " << min << endl;
	}
}

string Task::generateNumbers()
{
	//srand();
	string numbers;
	for(int i = 0; i < ARRAY_LENGTH; i++){
		numbers.append( convertIntToString(i) );
		if(i < ARRAY_LENGTH - 1)
			numbers.append(",");
	}
	return numbers;
}

void Task::splitTask(const int processCount)
{
	if(processCount == 0)
		return;
	int itemsPerProcess = ARRAY_LENGTH / processCount;

	string numbers = generateNumbers();

	int pos = -1;
	int prevPos = 0;

	for(int i = 0; i < processCount; i++){
		int items = 0;
		do{
			++pos;
			pos = numbers.find(',', pos);
			if(pos != -1)
				++items;
		}while(pos != -1 && items < itemsPerProcess);
		string subTask = numbers.substr(prevPos, pos - prevPos);
		cout << "prevPos = " << prevPos << endl;
		cout << "pos = " << pos << endl;
		cout << "inserted "<< subTask << endl;
		m_splitedTasks.push_back(subTask);
		prevPos = pos + 1;
	}
	//TODO : there is a bug here when the slaves count is one
	// i should not insert any number in the remaining list
	// i should send all numbers to the slave process running
	do{
		++pos;
		pos = numbers.find(',', pos);
		
			string number = numbers.substr(prevPos, pos - prevPos);
			int iNumber = atoi(number.c_str());
			cout << "number inserted = " << iNumber << endl;
			m_remainingItems.push_back( iNumber);
			prevPos = pos + 1;
		
	}while(pos  != -1);
}

string Task::next()
{
	//i should protect this function against multithreading
	if(m_current < m_splitedTasks.size())
		return m_splitedTasks[m_current++];

	return string();
}