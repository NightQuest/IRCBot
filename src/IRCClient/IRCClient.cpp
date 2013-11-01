#include "preCompiled.h"

IRCClient::IRCClient(const std::string& hostname, unsigned int port) : Socket(hostname, port)
{
}

IRCClient::~IRCClient()
{
}
