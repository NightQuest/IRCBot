#include "preCompiled.h"

class logger : public IRCScript
{
public:
	logger() : IRCScript("logger") { }

	void onQuit(const UserPtr& user, const std::string& message)
	{
		insertLog(user, "", "QUIT", message);
	}

	void onJoin(const UserPtr& user, const std::string& channel)
	{
		insertLog(user, channel, "JOIN", "");
	}

	void onPart(const UserPtr& user, const std::string& channel, const std::string& message)
	{
		insertLog(user, channel, "PART", message);
	}

	void onChatText(const UserPtr& user, const std::string& target, const std::string& message)
	{
		insertLog(user, target, "PRIVMSG", message);
	}

	void onKick(const UserPtr& user, const std::string& channel, const std::string& target, const std::string& message)
	{
		insertLog(user, channel, "KICK", (target + ": " + message));
	}

	void onNick(const UserPtr& user, const std::string& newNick)
	{
		insertLog(user, "", "NICK", newNick);
	}

	void onChatAction(const UserPtr& user, const std::string& target, const std::string& message)
	{
		insertLog(user, target, "ACT", message);
	}

	void insertLog(const UserPtr& user, std::string target, const std::string& action, std::string data)
	{
		if( !internalDB )
			return;

		std::string logUser = user->getFullAddress();
		if( internalDB->escape(logUser) && internalDB->escape(target) && internalDB->escape(data) )
		{
			internalDB->execute("INSERT INTO `log` (`user`, `target`, `action`, `data`) VALUES ('" + logUser + "', '" + target + "', '" + action + "', '" + data + "')");
		}
	}
};

void setupLogger()
{
	if( config->getBool("irc.log") )
		new logger;
}
