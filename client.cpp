#include "client.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h>

Client::Client(int fd, int epfd)
{
		m_fd = fd;
		SetClientTime();
		SetStatus(use_status);
		m_epfd = epfd;
}

Client::~Client()
{
		CloseClient();
}

void Client::SetStatus(int status)
{
		m_status = status;
}

int Client::GetStatus()
{
		return m_status;
}

time_t Client::GetClientTime()
{
		return m_time;
}

void Client::SetClientTime()
{
		m_time = time(NULL);
}

bool Client::Recv(char *buf, int len)
{
		int n = 0;    
		while(1)
		{
				int nread = read(m_fd, buf + n, len);
				if(nread < 0)
				{
						if(errno == EAGAIN || errno == EWOULDBLOCK)
						{
								continue;
						}
						else
						{
								break;//or return;
						}
				}
				else if(nread == 0)
				{
						break;
				}
				else
				{
						n += nread;
				}
				if(n == len)
				{
						break;
				}
		}
		if(n == len)
		{
				return true;
		}
		else
		{
				return false;
		}
}

bool Client::Send(char *buf, int len)
{
		int n = 0;
		while(1)
		{
				int nwrite = write(m_fd, buf + n, len);
				if(nwrite < len)
				{
						if(errno == EAGAIN || errno == EWOULDBLOCK)
						{
								continue;
						}
						else
						{
								break;//or return;		
						}

				}
				else
				{
						n += nwrite;
						len -= nwrite;
				}
		}
		if(len == 0)
		{
				return true;
		}
		else
		{
				return false;
		}
}

bool Client::CloseClient()
{
		struct epoll_event ee;
		ee.events = 0;
		ee.data.u64 = 0;
		ee.data.fd = m_fd;
		epoll_ctl(m_epfd, EPOLL_CTL_DEL, m_fd, &ee);
		close(m_fd);
		return true;
}
