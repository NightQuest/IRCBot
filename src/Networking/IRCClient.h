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
	std::vector<std::string> params;
	std::string target;
	std::string data;
	std::string raw;
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
	std::map<std::string, Channel> channels;
	std::string activeNickname;
	const Config& config;
	std::unique_ptr<MariaDB::Connection> internalDB;
	std::unique_ptr<MariaDB::Connection> externalDB;

	bool sentUser;

	int sendLine(const std::string& line) { return send(line + "\r\n"); }
	int sendCTCPResponse(const std::string& target, const std::string& ctcp, const std::string& msg) { return sendLine("NOTICE " + target + " :\x1" + ctcp + " " + msg + "\x1"); }
	int sendCTCP(const std::string& target, const std::string& ctcp) { return sendLine("PRIVMSG " + target + " :\x1" + ctcp + "\x1"); }
	int sendAction(const std::string& target, const std::string& message) { return sendCTCP(target, "ACTION " + message); }
	int sendMessage(const std::string& target, const std::string& message) { return sendLine("PRIVMSG " + target + " :" + message); }

	LineData parseLine(const std::string& line) const;
	void handleSCommand(const LineData& data);
	void handleCCommand(const std::string& command, const std::string& args, const LineData& data);
	void handlePRIVMSG(const LineData& data);
	void handleNOTICE(const LineData& data);
	void handleCTCP(const LineData& data);

	bool hasAccess(const std::string& user, unsigned int perm);

	void setNick(const std::string& newNick);
	void joinChannel(const std::string& channel);

public:
	IRCClient(const std::string& hostname, unsigned int port, bool ssl, const Config& _config);
	~IRCClient();

	bool isConnected() const { return connected; }

	void process();
};
