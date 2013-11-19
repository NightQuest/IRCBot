#pragma once

class User
{
private:
	std::string nickname;
	std::string ident;
	std::string hostname;

	unsigned int permissions;

public:
	User(const std::string& _nickname, const std::string& _ident, const std::string& _hostname);

	std::string getNickname() const { return nickname; }
	std::string getIdent() const { return ident; }
	std::string getHostname() const { return hostname; }
	std::string getFullAddress() const { return nickname + "!" + ident + "@" + hostname; }

	bool hasAccess(const unsigned int perm) { return (permissions & perm) == perm; }
};
