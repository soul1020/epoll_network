#include "threadPool.h"
#include <algorithm>

vector<Task*> ThreadPool::m_task_v;
//pthread_mutex_t ThreadPool::m_mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t ThreadPool::m_cond = PTHREAD_COND_INITIALIZER;


ThreadPool::ThreadPool(int thread_num)
{
		//pthread_mutex_init(&m_idle_mutex, NULL);
		//pthread_mutex_init(&m_busy_mutex, NULL);
		m_thread_num = thread_num;
		m_tid = new pthread_t[m_thread_num];
		Create();
}

ThreadPool::~ThreadPool()
{}

void ThreadPool::Create()
{
		for(int i=0; i<m_thread_num; i++)
		{
				pthread_create(&m_tid[i], NULL, routine, (void *)this);
				cout << "create thread success" << endl;
		}
}

void ThreadPool::AddTask(Task *task)
{
		pthread_mutex_lock(&m_mutex);
		m_task_v.push_back(task);
		pthread_cond_signal(&m_cond);
		pthread_mutex_unlock(&m_mutex);
}

void* ThreadPool::routine(void *s)
{
		while(1)
		{
				pthread_mutex_lock(&m_mutex);
				while(m_task_v.size()<=0)
				{
						pthread_cond_wait(&m_cond, &m_mutex);
				}
				vector<Task*>::iterator it = m_task_v.begin();
				Task *task = *it;
				if(task == NULL) continue;
				m_task_v.erase(it);
				pthread_mutex_unlock(&m_mutex);
				task->run();
				delete task;
				task = NULL;
				cout << "delete task success" << endl;
				ThreadPool *pool = (ThreadPool*)s;
				if(pool)
				{
						//pool->MoveToIdleList();
				}
		}
}
