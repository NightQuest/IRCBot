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
		else if( user->getNickname() == "NickServ" )
		{
			if( message.find("NickServ IDENTIFY") != string::npos || message.find("If this is your") != string::npos )
			{
				string nickPasses = config->getString("irc.nickpasses");
				if( !nickPasses.empty() )
				{
					auto nicksPass = Util::explode(nickPasses, ' ');
					if( !nicksPass.empty() )
					{
						for( const auto& tmp : nicksPass )
						{
							auto passes = Util::explode(tmp, ':');
							if( passes.size() == 2 && passes[0] == sSock->getNickname() )
							{
								sSock->sendMessage("NickServ", "IDENTIFY " + passes[1]);
								break;
							}
						}
					}
				}
			}
			else if( message.find("Password accepted") != string::npos || message.find("You are now identified") != string::npos )
			{
				if( config->getBool("irc.channels.joinwhenidentified") )
					sSock->joinChannel(config->getString("irc.channels"));
			}
		}
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