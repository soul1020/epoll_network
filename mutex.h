#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>

class Mutex
{
		public:
				Mutex();
				~Mutex();
				void Lock();
				void UnLock();
		private:
				pthread_mutex_t	m_mutex;
};
#endif
