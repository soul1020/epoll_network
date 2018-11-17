#include "server.h"
#include "acceptClientManager.h"
#include "listClientManager.h"
#include "progressThread.h"

void setnonblocking(int sock)
{
		int opts;
		opts=fcntl(sock,F_GETFL);
		if(opts<0)
		{
				perror("fcntl(sock,GETFL)");
				return;
		}
		opts = opts|O_NONBLOCK;
		if(fcntl(sock,F_SETFL,opts)<0)
		{
				perror("fcntl(sock,SETFL,opts)");
				return;
		}
}



int Server::CreateServerSocket(const std::string &ip, const int &port)
{
		int listenfd = socket(AF_INET, SOCK_STREAM, 0);
		if(listenfd == -1) return -1;

		struct sockaddr_in serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(ip.data());
		serveraddr.sin_port=htons(port);
		
		if(bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr)) ==- 1)
		{
				return -1;
		}
		
		if(listen(listenfd, SERVER_LIMIT) == -1)
		{
				return -1;
		}
		return listenfd;
}

void Server::InitServer()
{
		ProgressThread progress;
		progress.Run();

		int server_fd = CreateServerSocket("127.0.0.1", 9999);
		if(server_fd == -1) return;

		struct epoll_event ev,events[SERVER_LIMIT];
		int epfd = epoll_create(SERVER_LIMIT);
		if(epfd == -1) return;

		ev.data.fd = server_fd;
		ev.events = EPOLLIN|EPOLLET;
		epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);

		while(1)
		{
				int nfds = epoll_wait(epfd, events, SERVER_LIMIT, 0);

				for(int i=0; i<nfds; i++)
				{
						//accept function return a new clientfd
						if(events[i].data.fd == server_fd)
						{
								int client_fd = AcceptClient(server_fd, epfd);
								if(client_fd == -1) continue;
								std::cout << "accept a new client" << std::endl;
								AcceptClientManager::GetInstance()->Insert(client_fd, epfd);
								//new a client add this client to map
						}
						else if(events[i].events & EPOLLIN)
						{
								int client_fd =  events[i].data.fd;
								if(client_fd == -1) continue;
								Client *client = AcceptClientManager::GetInstance()->Find(client_fd);
								if(client == NULL) continue;
								ListClientManager::GetInstance()->Push(client);
								//find a client in map and insert to list
						}
				}
		}
}

int Server::AcceptClient(int server_fd, int epfd)
{
		struct sockaddr_in clientaddr;
		memset(&clientaddr, 0, sizeof(clientaddr));
		socklen_t clilen = sizeof(clientaddr);
		int connfd = accept(server_fd, (sockaddr *)&clientaddr, &clilen);
		if(connfd == -1) return -1;

		setnonblocking(connfd);

		struct epoll_event ev;
		ev.data.fd=connfd;
		ev.events=EPOLLIN | EPOLLET;
		epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
		return connfd;
}
