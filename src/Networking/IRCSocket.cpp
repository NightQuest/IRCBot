#include "preCompiled.h"

std::unique_ptr<IRCSocket> sSock;

IRCSocket::IRCSocket(const std::string& addr, unsigned int port, bool _useSSL) : Socket(addr, port, _useSSL)
{
	connected = (hSock.socketHandle != INVALID_SOCKET);
}

void IRCSocket::setNickname(const std::string& nickname)
{
	 sendLine("NICK " + nickname);
	 prevNickname = activeNickname;
	 activeNickname = nickname;
}

bool IRCSocket::getLine(std::string& line)
{
	vector<char> in(1024);
	do
	{
		vector<char> tmp(in.begin(), in.end());
		tmp.erase(remove(tmp.begin(), tmp.end(), '\0'), tmp.end());
		if( !tmp.empty() )
			for( size_t x = 0; x < tmp.size(); x++ )
				recvBuff.push_back(tmp[x]);

		if( !recvBuff.empty() )
		{
			for( auto itr = recvBuff.begin(); itr != recvBuff.end(); itr++ )
			{
				if( (*itr) == '\r' && *(itr+1) == '\n' )
				{
					line.assign(recvBuff.begin(), itr);
					recvBuff.erase(recvBuff.begin(), itr+2);
					return true;
				}
			}
		}
	} while( recv(&in[0], 1024) > 0 );

	return false;
}

int IRCSocket::sendLine(const std::string& line)
{
	return send(line + "\r\n");
}
