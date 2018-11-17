#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "client.h"

typedef struct header
{
		int code;
		int bodySize;
}Header;

typedef struct message
{
		Header	m_header;
		char	*m_buf;
		Client	*m_client;
}Message;

#endif
