#include "preCompiled.h"

SocketException::SocketException(const std::string& message) throw() : std::runtime_error(message)
{
}

SocketException::SocketException(const std::string& message, const std::string& detail) throw() : std::runtime_error(message + ": " + detail)
{
}

Socket::Socket(const std::string& foreignAddress, unsigned int foreignPort, bool _useSSL) throw(SocketException) : hSock(INVALID_SOCKET), useSSL(_useSSL)
{
	WSADATA wsaData;
	if( WSAStartup(WINSOCK_VERSION, &wsaData) != 0 )
		throw SocketException("WSAStartup() failed");

	addrinfo addrCriteria;
	memset(&addrCriteria, 0, sizeof(addrCriteria));

	addrCriteria.ai_family = AF_INET;
	addrCriteria.ai_socktype = SOCK_STREAM;
	addrCriteria.ai_protocol = IPPROTO_TCP;

	addrinfo* serverAddresses;
	if( getaddrinfo(foreignAddress.c_str(), std::to_string(foreignPort).c_str(), &addrCriteria, &serverAddresses) != 0 )
		throw SocketException("getaddrinfo() failed");

	for( addrinfo* currentAddress = serverAddresses; currentAddress != nullptr && hSock == INVALID_SOCKET; currentAddress = currentAddress->ai_next )
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

	if( useSSL )
	{
		SSL_load_error_strings();
		SSL_library_init();
			
		sslContext = SSL_CTX_new(SSLv23_client_method());
		if( sslContext == nullptr )
			throw SocketException("SSL_CTX_new() failed");

		sslHandle = SSL_new(sslContext);
		if( sslHandle == nullptr )
			throw SocketException("SSL_new() failed");

		if( !SSL_set_fd(sslHandle, static_cast<int>(hSock)) )
			throw SocketException("SSL_set_fd() failed");

		if( SSL_connect(sslHandle) != 1 )
			throw SocketException("SSL_connect() failed");
	}
}

Socket::~Socket()
{
	if( sslHandle != nullptr )
	{
		SSL_shutdown(sslHandle);
		SSL_free(sslHandle);
	}
	if( sslContext != nullptr )
	{
		SSL_CTX_free(sslContext);
	}

	if( hSock != INVALID_SOCKET )
	{
		::closesocket(hSock);
		hSock = INVALID_SOCKET;
	}
	WSACleanup();
}

int Socket::send(const std::string& data) throw(SocketException)
{
	if( data.empty() )
		throw SocketException("send() failed", "data is empty");

	int ret = 0;
	if( useSSL )
		ret = SSL_write(sslHandle, data.c_str(), static_cast<int>(data.size()));
	else
		ret = ::send(hSock, data.c_str(), static_cast<int>(data.length()), 0);
	return ret;
}

int Socket::send(const char* data, int dataLen) throw(SocketException)
{
	int ret = 0;
	if( useSSL )
		ret = SSL_write(sslHandle, data, dataLen);
	else
		ret = ::send(hSock, data, dataLen, 0);
	return ret;
}

int Socket::recv(char* buffer, int bufferLen) throw(SocketException)
{
	int ret = useSSL ? SSL_read(sslHandle, buffer, bufferLen) : ::recv(hSock, buffer, bufferLen, 0);
	if( ret < 0 )
		throw SocketException("::recv() failed");
	return ret;
}
