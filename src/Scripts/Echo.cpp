#include "preCompiled.h"

class echo : public IRCScript
{
public:
	echo() : IRCScript("echo") { }

	void onPing(const std::string& argument)
	{
		cout << "PING <-> PONG" << endl;
	}

	void onQuit(const std::shared_ptr<User>& user, const std::string& message)
	{
		cout << "Q " << user->getNickname();
		if( !message.empty() )
			cout << ": " << message;
		cout << endl;
	}

	void onJoin(const std::shared_ptr<User>& user, const std::string& channel)
	{
		cout << "J " << user->getNickname() << " -> " << channel << endl;
	}

	void onPart(const std::shared_ptr<User>& user, const std::string& channel, const std::string& message)
	{
		cout << "P " << user->getNickname() << " -> " << channel;
		if( !message.empty() )
			cout << ": " << message;
		cout << endl;
	}

	void onChatText(const std::shared_ptr<User>& user, const std::string& target, const std::string& message)
	{
		cout << "T " << user->getNickname() << " -> " << target << ": " << message << endl;
	}

	void onChatAction(const std::shared_ptr<User>& user, const std::string& target, const std::string& message)
	{
		cout << "A " << user->getNickname() << " -> " << target << ": " << message << endl;
	}

	void onCTCP(const std::shared_ptr<User>& user, const std::string& target, const std::string& ctcp, const std::string& message)
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
