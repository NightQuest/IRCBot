#include "preCompiled.h"

SocketException::SocketException(const std::string& message) throw() : std::runtime_error(message)
{
}

SocketException::SocketException(const std::string& message, const std::string& detail) throw() : std::runtime_error(message + ": " + detail)
{
}

Socket::Socket(const std::string& foreignAddress, unsigned int foreignPort, bool _useSSL) throw(SocketException) : connected(false)
{
	memset(&hSock, 0, sizeof(hSock));
	hSock.socketHandle = INVALID_SOCKET;
	hSock.useSSL = _useSSL;
	hSock.hostname = foreignAddress;
	hSock.port = foreignPort;

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

	for( addrinfo* currentAddress = serverAddresses; currentAddress != nullptr && hSock.socketHandle == INVALID_SOCKET; currentAddress = currentAddress->ai_next )
	{
		if( (hSock.socketHandle = ::socket(currentAddress->ai_family, currentAddress->ai_socktype, currentAddress->ai_protocol)) != INVALID_SOCKET )
		{
			if( ::connect(hSock.socketHandle, currentAddress->ai_addr, static_cast<int>(currentAddress->ai_addrlen)) != 0 )
			{
				::closesocket(hSock.socketHandle);
				hSock.socketHandle = INVALID_SOCKET;
			}
		}
	}

	freeaddrinfo(serverAddresses);
	if( hSock.socketHandle == INVALID_SOCKET )
		throw SocketException("Unable to connect");

	connected = true;

	if( hSock.useSSL )
	{
		SSL_load_error_strings();
		SSL_library_init();
			
		hSock.sslContext = SSL_CTX_new(SSLv23_client_method());
		if( hSock.sslContext == nullptr )
			throw SocketException("SSL_CTX_new() failed");

		hSock.sslHandle = SSL_new(hSock.sslContext);
		if( hSock.sslHandle == nullptr )
			throw SocketException("SSL_new() failed");

		if( !SSL_set_fd(hSock.sslHandle, static_cast<int>(hSock.socketHandle)) )
			throw SocketException("SSL_set_fd() failed");

		if( SSL_connect(hSock.sslHandle) != 1 )
			throw SocketException("SSL_connect() failed");
	}
}

Socket::~Socket()
{
	if( hSock.sslHandle != nullptr )
	{
		SSL_shutdown(hSock.sslHandle);
		SSL_free(hSock.sslHandle);
	}
	if( hSock.sslContext != nullptr )
	{
		SSL_CTX_free(hSock.sslContext);
	}

	if( hSock.socketHandle != INVALID_SOCKET )
	{
		::closesocket(hSock.socketHandle);
		hSock.socketHandle = INVALID_SOCKET;
	}
	WSACleanup();
}

int Socket::send(const std::string& data) throw(SocketException)
{
	if( data.empty() )
		throw SocketException("send() failed", "data is empty");

	int ret = 0;
	if( hSock.useSSL )
		ret = SSL_write(hSock.sslHandle, data.c_str(), static_cast<int>(data.length()));
	else
		ret = ::send(hSock.socketHandle, data.c_str(), static_cast<int>(data.length()), 0);

	if( ret < 0 )
		connected = false;

	return ret;
}

int Socket::send(const char* data, int dataLen) throw(SocketException)
{
	int ret = 0;
	if( hSock.useSSL )
		ret = SSL_write(hSock.sslHandle, data, dataLen);
	else
		ret = ::send(hSock.socketHandle, data, dataLen, 0);

	if( ret < 0 )
		connected = false;

	return ret;
}

int Socket::recv(char* buffer, int bufferLen) throw(SocketException)
{
	int ret = hSock.useSSL ? SSL_read(hSock.sslHandle, buffer, bufferLen) : ::recv(hSock.socketHandle, buffer, bufferLen, 0);
	if( ret <= 0 )
		connected = false;
	return ret;
}
