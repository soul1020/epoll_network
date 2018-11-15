#include "listClientManager.h"

ListClientManager *ListClientManager::g_listClientManager = NULL;

ListClientManager::ListClientManager()
{}

ListClientManager::~ListClientManager()
{}

void ListClientManager::Push(Client *client)
{
		m_mutex.Lock();
		m_clientList.push_back(client);
		m_mutex.UnLock();
}

Client* ListClientManager::Pop()
{
		m_mutex.Lock();
		Client *client = m_clientList.front();
		m_clientList.pop_front();
		m_mutex.UnLock();
		return client;
}

ListClientManager* ListClientManager::GetInstance()
{
		if(g_listClientManager == NULL)
		{
				g_listClientManager = new ListClientManager();
		}
		return g_listClientManager;
}
