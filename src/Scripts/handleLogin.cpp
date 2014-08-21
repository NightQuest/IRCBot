#include "preCompiled.h"

class handleLogin : public IRCScript
{
private:
	bool sentUser;

public:
	handleLogin() : IRCScript("handleLogin"), sentUser(false)
	{
		sSock->setNickname(config->getString("irc.nickname"));
		sSock->sendLine("USER " + config->getString("irc.name") + " 0 * :" + config->getString("irc.name"));
	}

	void NickServIdentify()
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

	void onNotice(const UserPtr& user, const std::string& target, const std::string& message)
	{
		//cout << "NOTICE " << user->getNickname() << " -> " << target << ": " << message << endl;
		if( user->getNickname() == "NickServ" )
		{
			string msg = message;
			transform(message.begin(), message.end(), msg.begin(), ::tolower);
			if( !config->getBool("irc.alwaysidentify") && (msg.find("nickserv identify") != string::npos || message.find("if this is your") != string::npos) )
			{
				NickServIdentify();
			}
			else if( msg.find("password accepted") != string::npos || msg.find("you are now identified") != string::npos )
			{
				if( config->getBool("irc.channels.joinwhenidentified") )
					sSock->joinChannel(config->getString("irc.channels"));
			}
			else if( msg.find("ghost with your nick has been killed") != string::npos )
			{
				sSock->setNickname(config->getString("irc.nickname"));
			}
		}
	}

	void onWelcome(const std::string& message)
	{
		sentUser = true;

		if( sSock->getNickname() != config->getString("irc.nickname") )
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
						if( passes.size() == 2 && passes[0] == config->getString("irc.nickname") )
						{
							sSock->sendMessage("NickServ", "GHOST " + config->getString("irc.nickname") + " " + passes[1]);
							break;
						}
					}
				}
			}
		}
		else if( config->getBool("irc.alwaysidentify") )
			NickServIdentify();

		if( !config->getBool("irc.channels.joinwhenidentified") )
			sSock->joinChannel(config->getString("irc.channels"));
	}
};

void setupHandleLogin()
{
	new handleLogin();
}
