#pragma once

enum botAccess
{
	ACCESS_NONE			= 0x00000000,
	ACCESS_SQL			= 0x00000001,
	ACCESS_QUIT			= 0x00000002,
	ACCESS_SAY			= 0x00000004,
	ACCESS_BIND			= 0x00000008,
	ACCESS_INTERNAL_SQL = 0x00000010,
};

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

typedef std::shared_ptr<User> UserPtr;
