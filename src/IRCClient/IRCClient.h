#pragma once

class IRCClient : public Socket
{
private:
	std::vector<std::string> channels;
	std::string activeNickname;
	std::string joinUser;
	std::string joinNickname;
	std::string joinAltNickname;

public:
	IRCClient(const std::string& hostname, unsigned int port, const std::string& user, const std::string& nick, const std::string& aNick);
	~IRCClient();
};
