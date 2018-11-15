#include "acceptClientManager.h"

AcceptClientManager* AcceptClientManager::g_acceptClientManager = NULL;

AcceptClientManager::AcceptClientManager()
{}

AcceptClientManager::~AcceptClientManager()
{}

void AcceptClientManager::ForEachItem()
{
		m_mutex.Lock();
		for(std::map<int, Client*>::iterator it=m_acceptClientMap.begin(); it!=m_acceptClientMap.end(); it++)
		{
				time_t time_now = time(NULL);
				int seconds = time_now - it->second->GetClientTime();
				if(seconds > 20)
				{
						if(it->second)
						{
								delete it->second;
						}
						m_acceptClientMap.erase(it);
				}
		}
		m_mutex.UnLock();
}

Client* AcceptClientManager::Find(int fd)
{
		m_mutex.Lock();
		std::map<int, Client*>::iterator it = m_acceptClientMap.find(fd);
		if(it == m_acceptClientMap.end())
		{
				m_mutex.UnLock();
				return NULL;
		}
		m_mutex.UnLock();
		return it->second;
}

bool AcceptClientManager::Insert(int fd, int epfd)
{
		m_mutex.Lock();
		std::map<int, Client*>::iterator it = m_acceptClientMap.find(fd);
		if(it != m_acceptClientMap.end())
		{
				m_mutex.UnLock();
				return false;
		}
		else
		{
				Client *client = new Client(fd, epfd);
				if(client)
				{
						m_acceptClientMap[fd] = client;
				}
		}
		m_mutex.UnLock();
		return true;
}

bool AcceptClientManager::Erase(int fd)
{
		m_mutex.Lock();
		std::map<int, Client*>::iterator it = m_acceptClientMap.find(fd);
		if(it == m_acceptClientMap.end())
		{
				m_mutex.UnLock();
				return false;
		}
		else
		{
				if(it->second)
				{
						delete it->second;
				}
				m_acceptClientMap.erase(it);
		}       
		m_mutex.UnLock();
		return true;
}

AcceptClientManager* AcceptClientManager::GetInstance()
{
		if(g_acceptClientManager == NULL)
		{
				g_acceptClientManager = new AcceptClientManager();
		}
		return g_acceptClientManager;
}
