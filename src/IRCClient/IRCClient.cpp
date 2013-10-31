#include "preCompiled.h"

IRCClient::IRCClient() : hSock(INVALID_SOCKET)
{
}

IRCClient::~IRCClient()
{
}

bool IRCClient::open(const std::string& hostname, unsigned int port)
{
	if( WSAStartup(WINSOCK_VERSION, &wsaData) != 0 )
		return false;

	if( hostname.empty() || port == 0 )
		return false;
}