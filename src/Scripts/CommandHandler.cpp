#include "preCompiled.h"

class CommandHandler : public IRCScript
{
public:
	CommandHandler() : IRCScript("CommandHandler")
	{
	}

	void onChatText(const std::shared_ptr<User>& user, const std::string& target, const std::string& message)
	{
		if( target != sSock->getNickname() )
		{
			if( message[0] == '!' )
			{
				string command, args;

				size_t pos = message.find_first_of(' ');
				if( pos != string::npos )
				{
					command = message.substr(1, pos-1);
					args = message.substr(pos+1);
				}
				else
					command = message.substr(1, message.length()-1);
				
				//HandleCommand(user, target, command, args);
			}
		}
	}
};

void setupCommandHandler()
{
	new CommandHandler();
}