#include "mutex.h"

Mutex::Mutex()
{
		pthread_mutex_init(&m_mutex, NULL);
}

Mutex::~Mutex()
{
		pthread_mutex_destroy(&m_mutex);
}

void Mutex::Lock()
{
		pthread_mutex_lock(&m_mutex);
}

void Mutex::UnLock()
{
		pthread_mutex_unlock(&m_mutex);
}
