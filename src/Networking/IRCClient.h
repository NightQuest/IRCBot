#pragma once

class IRCException : public std::runtime_error
{
public:
	IRCException(const std::string& message) throw();
	IRCException(const std::string& message, const std::string& detail) throw();
};

struct LineAuthor
{
	std::string nickname;
	std::string ident;
	std::string hostname;
	std::string full;
};
struct LineData
{
	LineAuthor author;
	std::string command;
	std::string param;
	std::string target;
	std::string data;
};

class Channel
{
private:
	std::string name;

public:
	Channel(const std::string& _name) : name(_name) { }

	const std::string getName() const { return name; }
};

enum botAccess
{
	ACCESS_SQL			= 0x00000001,
	ACCESS_QUIT			= 0x00000002,
	ACCESS_SAY			= 0x00000004,
};

class IRCClient : public Socket
{
private:
	std::vector<Channel> channels;
	std::string activeNickname;
	const Config& config;
	std::unique_ptr<MariaDB::Connection> internalDB;
	std::unique_ptr<MariaDB::Connection> externalDB;

	bool connected;
	bool sentUser;

	int recvData(char* data, int dataLen);
	int sendData(const char* data, int dataLen);
	int sendData(const std::string& data);
	int sendLine(const std::string& line) { return sendData(line + "\r\n"); }
	int sendCTCPReply(const std::string& target, const std::string& ctcp, const std::string& msg) { return sendData("NOTICE " + target + " :\x1" + ctcp + " " + msg + "\x1\r\n"); }
	int sendCTCPRequest(const std::string& target, const std::string& ctcp) { return sendData("PRIVMSG " + target + " :\x1" + ctcp + "\x1\r\n"); }

	LineData parseLine(const std::string& line) const;
	void handleSCommand(const LineData& data);
	void handleCCommand(const std::string& command, const std::string& args, const LineData& data);
	void handlePRIVMSG(const LineData& data);
	void handleNOTICE(const LineData& data);
	void handleCTCP(const LineData& data);

	bool hasAccess(const std::string& user, unsigned int perm);

	void setNick(const std::string& newNick);
	void joinChannel(const std::string& channel);
	void sendMessage(const std::string& target, const std::string& message);

public:
	IRCClient(const std::string& hostname, unsigned int port, bool ssl, const Config& _config);
	~IRCClient();

	bool isConnected() const { return connected; }

	void process();
};
