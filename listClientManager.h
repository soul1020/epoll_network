#ifndef LISTCLIENTMANAGER_H_
#define LISTCLIENTMANAGER_H_

#include "client.h"
#include "mutex.h"

#include <list>

class ListClientManager
{
		public:
				ListClientManager();
				~ListClientManager();
				void Push(Client *client);
				Client* Pop();
		public:
				static ListClientManager* GetInstance();
		private:
				Mutex	m_mutex;
				std::list<Client*>	m_clientList;
		private:
				static ListClientManager *g_listClientManager;


};
#endif
