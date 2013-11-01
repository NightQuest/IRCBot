#pragma once

class IRCClient : public Socket
{
public:
	IRCClient(const std::string& hostname, unsigned int port = 6667);
	~IRCClient();
};
