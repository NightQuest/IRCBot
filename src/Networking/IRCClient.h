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
	std::string data;
};

class IRCClient : public Socket
{
private:
	std::vector<std::string> channels;
	std::string activeNickname;
	std::string joinUser;
	std::string joinNickname;
	std::string joinAltNickname;
	bool connected;
	bool sentUser;

	LineData parseLine(const std::string& line) const;
	void handleCommand(const LineData& data);

	void setNick(const std::string& newNick);

public:
	IRCClient(const std::string& hostname, unsigned int port, bool ssl, const std::string& user, const std::string& nick, const std::string& aNick);
	~IRCClient();

	bool isConnected() const { return connected; }

	void process(Config config);
};
