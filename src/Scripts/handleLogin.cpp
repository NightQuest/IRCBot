#include "preCompiled.h"

class handleLogin : public IRCScript
{
private:
	bool sentUser;

public:
	handleLogin() : IRCScript("handleLogin"), sentUser(false) { }

	void onNotice(const std::shared_ptr<User>& user, const std::string& target, const std::string& message)
	{
		if( target == "AUTH" )
		{
			cout << "N " << target << ": " << message << endl;
			if( !sentUser && message.find("*** Found") != string::npos ) // We're connecting
			{
				sSock->sendLine("USER " + config->getString("irc.name") + " 8 * :" + config->getString("irc.name"));
				sSock->setNickname(config->getString("irc.nickname"));
			}
		}
		else
			cout << "N " << user->getNickname() << " -> " << target << ": " << message << endl;
	}

	void onWelcome(const std::string& message)
	{
		sentUser = true;
		cout << message << endl;
	}
};

void setupHandleLogin()
{
	new handleLogin();
}
