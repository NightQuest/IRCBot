#include "preCompiled.h"

class SayCommand : public IRCScript
{
public:
	SayCommand() : IRCScript("SayCommand") { }
	
	std::vector<ChatCommand> getCommands() override
	{
		ChatCommand commands[] = {
			{ "say", "Will cause the bot to say whatever is specified", handleSayCommand, ACCESS_SAY, vector<ChatCommand>() },
			{ "me", "Will cause the bot to act whatever is specified", handleMeCommand, ACCESS_SAY, vector<ChatCommand>() }
		};

		return vector<ChatCommand>(begin(commands), end(commands));
	}

	static void handleSayCommand(const UserPtr& user, const std::string& target, const std::string& arguments)
	{
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;
		sSock->sendMessage(messageTarget, arguments);
	}

	static void handleMeCommand(const UserPtr& user, const std::string& target, const std::string& arguments)
	{
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;
		sSock->sendAction(messageTarget, arguments);
	}
};

void setupSayCommand()
{
	new SayCommand();
}