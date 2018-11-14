#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define SERVER_LIMIT 1024


void setnonblocking(int sock);

class Server
{
		public:
				Server()
				{}
				~Server()
				{}
				void InitServer();
		private:
				int CreateServerSocket(const std::string &ip, const int &port);
				int AcceptClient(int server_fd, int epfd);
};
#endif
