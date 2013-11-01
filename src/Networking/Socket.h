#pragma once

#include <WinSock2.h>
#include <Ws2tcpip.h>

#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

class SocketException : public std::runtime_error
{
public:
	SocketException(const std::string& message) throw();
	SocketException(const std::string& message, const std::string& detail) throw();
};

class Socket
{
private:
	SOCKET hSock;
	SSL* sslHandle;
	SSL_CTX* sslContext;
	bool useSSL;

public:
	Socket::Socket(const std::string& addr, unsigned int port, bool _useSSL = false) throw(SocketException);
	virtual ~Socket();

	void send(const std::string& data) throw(SocketException);
	void send(const char* data, int dataLen) throw(SocketException);
	size_t recv(char* buffer, int bufferLen) throw(SocketException);

};
