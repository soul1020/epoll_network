#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "task.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

class ThreadPool
{
		public:
				ThreadPool(int thread_num);
				~ThreadPool();
				void Create();
				void AddTask(Task *task);
		private:
				static void* routine(void *s);
				void AddToIdleList(pthread_t tid);
				void MoveToIdleList();
				void MoveToBusyList();
		private:
				int                     m_thread_num;
				pthread_t*              m_tid;
				static vector<Task*>	m_task_v;
				static pthread_mutex_t  m_mutex;
				static pthread_cond_t   m_cond;

				vector<pthread_t>       m_idle_list;
				vector<pthread_t>       m_busy_list;

				pthread_mutex_t         m_idle_mutex;
				pthread_mutex_t			m_busy_mutex;
};
#endif

