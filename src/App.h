#pragma once

class AppException : public std::runtime_error
{
public:
	AppException(const std::string& message) throw();
	AppException(const std::string& message, const std::string& detail) throw();
};

class App
{
private:
	std::unique_ptr<IRCClient> irc;

public:
	App();
	~App();

	void run();
};

extern std::unique_ptr<MariaDB::Connection> internalDB;
extern std::unique_ptr<MariaDB::Connection> externalDB;
extern std::unique_ptr<Config> config;
