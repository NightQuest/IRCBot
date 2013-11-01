#include "preCompiled.h"

SocketException::SocketException(const std::string& message) throw() : std::runtime_error(message)
{
}

SocketException::SocketException(const std::string& message, const std::string& detail) throw() : std::runtime_error(message + ": " + detail)
{
}

Socket::Socket(const std::string& foreignAddress, unsigned int foreignPort) throw(SocketException) : hSock(INVALID_SOCKET)
{
	WSADATA wsaData;
	if( WSAStartup(WINSOCK_VERSION, &wsaData) != 0 )
		throw SocketException("WSAStartup() failed");

	addrinfo addrCriteria;
	memset(&addrCriteria, 0, sizeof(addrCriteria));

	addrCriteria.ai_family = AF_UNSPEC;
	addrCriteria.ai_socktype = SOCK_STREAM;
	addrCriteria.ai_protocol = IPPROTO_TCP;

	addrinfo* serverAddresses;
	if( getaddrinfo(foreignAddress.c_str(), std::to_string(foreignPort).c_str(), &addrCriteria, &serverAddresses) != 0 )
		throw SocketException("getaddrinfo() failed");

	for( addrinfo* currentAddress = serverAddresses; currentAddress != nullptr && hSock != INVALID_SOCKET; currentAddress = currentAddress->ai_next )
	{
		if( (hSock = ::socket(currentAddress->ai_family, currentAddress->ai_socktype, currentAddress->ai_protocol)) != INVALID_SOCKET )
		{
			if( ::connect(hSock, currentAddress->ai_addr, static_cast<int>(currentAddress->ai_addrlen)) != 0 )
			{
				::closesocket(hSock);
				hSock = INVALID_SOCKET;
			}
		}
	}

	freeaddrinfo(serverAddresses);
	if( hSock == INVALID_SOCKET )
		throw SocketException("Unable to connect");
}

Socket::~Socket()
{
	if( hSock != INVALID_SOCKET )
	{
		::closesocket(hSock);
		hSock = INVALID_SOCKET;
	}
	WSACleanup();
}

void Socket::send(const std::vector<char>& data) throw(SocketException)
{
	if( data.empty() )
		throw SocketException("send() failed", "data vector is empty");

	if( ::send(hSock, &data[0], static_cast<int>(data.size()), 0) == SOCKET_ERROR )
		throw SocketException("::send() failed");
}

void Socket::send(const char* data, int dataLen) throw(SocketException)
{
	if( ::send(hSock, data, dataLen, 0) == SOCKET_ERROR )
		throw SocketException("::send() failed");
}

size_t Socket::recv(char* buffer, int bufferLen) throw(SocketException)
{
	int ret = ::recv(hSock, buffer, bufferLen, 0);
	if( ret == SOCKET_ERROR )
		throw SocketException("::recv() failed");
	return ret;
}
