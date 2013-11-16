#pragma once

class IRCSocket : public Socket
{
private:
	std::vector<char> recvBuff;

public:
	IRCSocket(const std::string& addr, unsigned int port, bool _useSSL = false);
	IRCSocket();

	bool getLine(std::string& line);
	int sendLine(const std::string& line);

	void sendQuit(const std::string& message) { sendLine("QUIT :" + message); connected = false; }
	int sendMessage(const std::string& target, const std::string& message) { return sendLine("PRIVMSG " + target + " :" + message); }
	int sendNotice(const std::string& target, const std::string& message) { return sendLine("NOTICE " + target + " :" + message); }
	int sendCTCPResponse(const std::string& target, const std::string& ctcp, const std::string& msg) { return sendNotice(target, "\x1" + ctcp + " " + msg + "\x1"); }
	int sendCTCP(const std::string& target, const std::string& ctcp) { return sendMessage(target, "\x1" + ctcp + "\x1"); }
	int sendAction(const std::string& target, const std::string& message) { return sendCTCP(target, "ACTION " + message); }

	bool isConnected() const { return connected; }

};

extern std::unique_ptr<IRCSocket> sSock;
