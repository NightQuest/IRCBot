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

	LineData parseLine(const std::string& line) const;
	void handleCommand(const LineData& data);

	void setNick(const std::string& newNick);
	void joinChannel(const std::string& channel);

public:
	IRCClient(const std::string& hostname, unsigned int port, bool ssl, const Config& _config);
	~IRCClient();

	bool isConnected() const { return connected; }

	void process();
};
