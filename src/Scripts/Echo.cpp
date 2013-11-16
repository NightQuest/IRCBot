#include "preCompiled.h"

class echo : public IRCScript
{
public:
	echo() : IRCScript("echo") { }
	
	void onChatText(const std::string& user, const std::string& target, const std::string& message)
	{
		cout << "P " << user << " -> " << target << ": " << message << endl;
	}

	void onChatAction(const std::string& user, const std::string& target, const std::string& message)
	{
		cout << "A " << user << " -> " << target << ": " << message << endl;
	}

	void onCTCP(const std::string& user, const std::string& target, const std::string& ctcp, const std::string& message)
	{
		if( ctcp != "ACTION" )
		{
			cout << "C:" << ctcp << " " << user << " -> " << target << ": " << message << endl;
		}
	}
};

void setupEcho()
{
	new echo();
}
