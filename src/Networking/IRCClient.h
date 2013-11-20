#pragma once

class IRCException : public std::runtime_error
{
public:
	IRCException(const std::string& message) throw();
	IRCException(const std::string& message, const std::string& detail) throw();
};

struct LineData
{
	std::shared_ptr<User> author;
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

	LineData parseLine(const std::string& line) const;
	void handleSCommand(const LineData& data);
	void handleCCommand(const std::string& command, const std::string& args, const LineData& data);
	void handleCTCP(const LineData& data);

	void joinChannel(const std::string& channel);

public:
	IRCClient();
	~IRCClient();

	void process();
};
