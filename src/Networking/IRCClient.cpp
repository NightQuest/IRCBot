#include "preCompiled.h"

IRCClient::IRCClient(const std::string& hostname, unsigned int port, const std::string& user, const std::string& nick, const std::string& aNick)
	: Socket(hostname, port), joinUser(user), joinNickname(nick), joinAltNickname(aNick)
{
}

IRCClient::~IRCClient()
{
}
