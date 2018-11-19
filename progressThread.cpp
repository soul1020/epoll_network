#include "progressThread.h"
#include "listClientManager.h"
#include "message.h"

#include <pthread.h>
#include <string.h>

ProgressThread::ProgressThread()
{}

ProgressThread::~ProgressThread()
{}

void ProgressThread::Run()
{
		pthread_t tid;
		pthread_create(&tid, NULL, Runtine, NULL);
}

void *ProgressThread::Runtine(void *arg)
{
		while(1)
		{
				Client *client = ListClientManager::GetInstance()->Pop();
				if(client == NULL) continue;

				//recv header
				char headBuf[8];
				if(!client->Recv(headBuf, 8)) continue;
				Header pHeader;
				memcpy(&pHeader, headBuf, 8);

				//recv body
				char *buf = new char[pHeader.bodySize];
				memset(buf, 0, pHeader.bodySize);
				if(!client->Recv(buf, pHeader.bodySize)) 
				{
						delete buf;
						buf = NULL;
						continue;
				}

				Message *message = new Message;
				message->m_header = pHeader;
				message->m_buf = buf;
				message->m_client = client;

				//add to task
		}
}
