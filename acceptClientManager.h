#ifndef ACCEPTCLIENTMANAGER_H_
#define ACCEPTCLIENTMANAGER_H_

#include "client.h"
#include "mutex.h"

#include <map>

class AcceptClientManager
{
		public:
				AcceptClientManager();
				~AcceptClientManager();
				Client* Find(int fd);
				bool Insert(int fd, int epfd);
				bool Erase(int fd);
				void ForEachItem();
		public:
				AcceptClientManager* GetInstance();
		private:
				std::map<int, Client*>	m_acceptClientMap;
				Mutex	m_mutex;
		private:
				static AcceptClientManager *g_acceptClientManager;
};

#endif
