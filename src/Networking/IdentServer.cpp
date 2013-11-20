#include "preCompiled.h"

IdentServer::IdentServer(unsigned short _port) : port(_port), hSock(INVALID_SOCKET)
{
	WSADATA wsaData;
	if( WSAStartup(WINSOCK_VERSION, &wsaData) != 0 )
		throw SocketException("WSAStartup() failed");
}

IdentServer::~IdentServer()
{
	WSACleanup();
}

void IdentServer::runWithTimeout(const std::string& username, unsigned int timeout)
{
	sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	if( (hSock = ::socket(AF_INET, SOCK_STREAM, 0)) == -1 )
		return;

	if( ::bind(hSock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0 )
	{
		closesocket(hSock);
		return;
	}

	if( ::listen(hSock, 1) != 0 )
	{
		closesocket(hSock);
		return;
	}

	thread([=](){
		SOCKET client;
		int client_addr_len = sizeof(sockaddr_in);
		sockaddr_in client_addr;

		if( (client = ::accept(hSock, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len)) == -1 )
		{
			::closesocket(hSock);
			return;
		}

		while( true )
		{
			fd_set fd;
			timeval tv;
			FD_ZERO(&fd);
			FD_SET(client, &fd);

			tv.tv_sec = tv.tv_usec = timeout;
			if( ::select(static_cast<int>(client) + 1, &fd, nullptr, nullptr, &tv) < 0 )
			{
				::closesocket(client);
				::closesocket(hSock);
				return;
			}

			if( FD_ISSET(client, &fd) )
				break;
		}

		std::string line;
		std::vector<char> recvBuff(1024);
		int ret = ::recv(client, &recvBuff[0], static_cast<int>(recvBuff.size()), 0);
		if( ret <= 0 || ret >= 20 ) // 65535 , 65535\r\n == 15 chars
		{
			::closesocket(client);
			::closesocket(hSock);
			return;
		}

		for( auto itr = recvBuff.begin(); itr != recvBuff.end(); itr++ )
			if( (*itr) == '\r' && *(itr+1) == '\n' )
				line.assign(recvBuff.begin(), itr);

		if( !line.empty() )
		{
			std::string ret;
			if( line.find_first_of(',') == string::npos )
				ret = "0, 0 : ERROR : X-INVALID-REQUEST\r\n";
			else
			{
				auto ports = Util::explode(line, ',');
				int serverPort = 0, clientPort = 0;
				if( ports[0][ports[0].size() - 1] == ' ' )
					ports[0] = ports[0].substr(0, ports[0].size()-1);
				if( ports[1][0] == ' ' )
					ports[1] = ports[1].substr(1);

				serverPort = std::stoi(ports[0]);
				clientPort = std::stoi(ports[1]);

				if( clientPort > 65535 || clientPort < 1 || serverPort > 65535 || serverPort < 1 )
					ret = ports[0] + ", " + ports[1] + " : ERROR : INVALID-PORT\r\n";

				else if( username.empty() )
					ret = ports[0] + ", " + ports[1] + " : ERROR : NO-USER\r\n";

				else
					ret = ports[0] + ", " + ports[1] + " : USERID : UNIX : " + username + "\r\n";
			}

			::send(client, ret.c_str(), static_cast<int>(ret.length()), 0);
		}
		::closesocket(client);
		::closesocket(hSock);
	}).detach();
}
