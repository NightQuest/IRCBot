#include "preCompiled.h"

class echo : public IRCScript
{
public:
	echo() : IRCScript("echo") { }

	void onPing(const std::string& argument)
	{
		cout << "PING <-> PONG" << endl;
	}

	void onQuit(const UserPtr& user, const std::string& message)
	{
		cout << "Q " << user->getNickname();
		if( !message.empty() )
			cout << ": " << message;
		cout << endl;
	}

	void onJoin(const UserPtr& user, const std::string& channel)
	{
		cout << "J " << user->getNickname() << " -> " << channel << endl;
	}

	void onPart(const UserPtr& user, const std::string& channel, const std::string& message)
	{
		cout << "P " << user->getNickname() << " -> " << channel;
		if( !message.empty() )
			cout << ": " << message;
		cout << endl;
	}

	void onChatText(const UserPtr& user, const std::string& target, const std::string& message)
	{
		cout << "T " << user->getNickname() << " -> " << target << ": " << message << endl;
	}

	void onKick(const UserPtr& user, const std::string& channel, const std::string& target, const std::string& message)
	{
		cout << "K " << user->getNickname() << " -> " << channel << " -> " << target << ": " << message << endl;
	}

	void onNick(const UserPtr& user, const std::string& newNick)
	{
		cout << "N " << user->getNickname() << " -> " << newNick << endl;
	}

	void onChatAction(const UserPtr& user, const std::string& target, const std::string& message)
	{
		cout << "A " << user->getNickname() << " -> " << target << ": " << message << endl;
	}

	void onCTCP(const UserPtr& user, const std::string& target, const std::string& ctcp, const std::string& message)
	{
		if( ctcp != "ACTION" )
		{
			cout << "C:" << ctcp << " " << user->getNickname() << " -> " << target << ": " << message << endl;
		}
	}
};

void setupEcho()
{
	new echo();
}
