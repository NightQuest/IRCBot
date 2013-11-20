#pragma once

class IdentServer
{
private:
	SOCKET hSock;
	unsigned short port;

public:
	IdentServer(unsigned short _port = 113);
	~IdentServer();

	void runWithTimeout(const std::string& username, unsigned int timeout);
};
