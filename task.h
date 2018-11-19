#ifndef TASK_H_
#define TASK_H_

#include <iostream>
#include <string>

using namespace std;

class Task
{
		public:
				Task(const string &task_name, void *pri_data)
				{
						m_task_name = task_name;
						m_pri_data = pri_data;
				}
				virtual void run()=0;
		protected:
				string		m_task_name;
				void        *m_pri_data;
};
#endif

