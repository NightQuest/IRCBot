#pragma once

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

public:
	Socket::Socket(const std::string& addr, unsigned int port) throw(SocketException);
	virtual ~Socket();

	void send(const std::vector<char>& data) throw(SocketException);
	void send(const char* data, int dataLen) throw(SocketException);
	size_t recv(char* buffer, int bufferLen) throw(SocketException);

};
