#pragma once

class IRCClient
{
private:
	SOCKET hSock;
	WSADATA wsaData;

public:
	IRCClient();
	~IRCClient();

	bool open(const std::string& host, unsigned int port);
};