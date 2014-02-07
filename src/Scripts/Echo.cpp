#include "preCompiled.h"

class echo : public IRCScript
{
public:
	echo() : IRCScript("echo") { }

	void onPing(const std::string& argument)
	{
		if( !config->getBool("echo.pingpong") )
			return;

		cout << "PING <-> PONG" << endl;
	}

	void onWelcome(const std::string& message)
	{
		if( !config->getBool("echo.welcome") )
			return;

		cout << message << endl;
	}

	void onQuit(const UserPtr& user, const std::string& message)
	{
		if( !config->getBool("echo.quit") )
			return;

		cout << "Q " << user->getNickname();
		if( !message.empty() )
			cout << ": " << message;
		cout << endl;
	}

	void onJoin(const UserPtr& user, const std::string& channel)
	{
		if( !config->getBool("echo.join") )
			return;

		cout << "J " << user->getNickname() << " -> " << channel << endl;
	}

	void onPart(const UserPtr& user, const std::string& channel, const std::string& message)
	{
		if( !config->getBool("echo.part") )
			return;

		cout << "P " << user->getNickname() << " -> " << channel;
		if( !message.empty() )
			cout << ": " << message;
		cout << endl;
	}

	void onChatText(const UserPtr& user, const std::string& target, const std::string& message)
	{
		if( !config->getBool("echo.text") )
			return;

		cout << "T " << user->getNickname() << " -> " << target << ": " << message << endl;
	}

	void onKick(const UserPtr& user, const std::string& channel, const std::string& target, const std::string& message)
	{
		if( !config->getBool("echo.kick") )
			return;

		cout << "K " << user->getNickname() << " -> " << channel << " -> " << target << ": " << message << endl;
	}

	void onNick(const UserPtr& user, const std::string& newNick)
	{
		if( !config->getBool("echo.nick") )
			return;

		cout << "N " << user->getNickname() << " -> " << newNick << endl;
	}

	void onChatAction(const UserPtr& user, const std::string& target, const std::string& message)
	{
		if( !config->getBool("echo.action") )
			return;

		cout << "A " << user->getNickname() << " -> " << target << ": " << message << endl;
	}

	void onCTCP(const UserPtr& user, const std::string& target, const std::string& ctcp, const std::string& message)
	{
		if( !config->getBool("echo.ctcp") )
			return;

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
