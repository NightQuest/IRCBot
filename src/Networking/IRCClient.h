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

class IRCClient
{
private:
	std::map<std::string, Channel> channels;
	std::string activeNickname;
	const Config& config;

	bool sentUser;
	

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
	IRCClient(const Config& _config);
	~IRCClient();

	void process();
};
