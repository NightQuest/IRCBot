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
	std::deque<std::string> params;
	std::string target;
	std::string data;
	std::string raw;
};

class IRCClient
{
private:
	LineData parseLine(const std::string& line) const;
	void handleSCommand(const LineData& data);
	bool handleCCommand(const std::vector<ChatCommand>& commands, const std::deque<std::string>& line, const LineData& data);

public:
	IRCClient();
	~IRCClient();

	void process();
};
