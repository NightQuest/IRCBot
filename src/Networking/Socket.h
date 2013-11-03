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
	SSL_CTX* sslContext;
	bool useSSL;

protected:
	SOCKET hSock;
	SSL* sslHandle;
	
	Socket(const std::string& addr, unsigned int port, bool _useSSL = false) throw(SocketException);

	int send(const std::string& data) throw(SocketException);
	int send(const char* data, int dataLen) throw(SocketException);
	int recv(char* buffer, int bufferLen) throw(SocketException);

	bool usingSSL() const { return useSSL; }

public:
	virtual ~Socket();

};
