#ifndef CLIENT_H_
#define CLIENT_H_

#include <time.h>

enum Status
{
		use_status = 1,
		dele_status = 2
};

class Client
{
		public:
				Client(int fd, int epfd);
				~Client();
				bool Recv(char *buf, int len);
				bool Send(char *buf, int len);
				bool CloseClient();
				time_t GetClientTime();
				void SetClientTime();
				void SetStatus(int status);
				int GetStatus();
		private:
				int		m_fd;
				time_t	m_time;
				int		m_status;
				int		m_epfd;
};
#endif
