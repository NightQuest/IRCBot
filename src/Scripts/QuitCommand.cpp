#include "preCompiled.h"

class QuitCommand : public IRCScript
{
public:
	QuitCommand() : IRCScript("QuitCommand") { }
	
	std::vector<ChatCommand> getCommands() override
	{
		ChatCommand commands[] = {
			{ "quit", "Will cause the bot to quit", handleQuitCommand, ACCESS_QUIT, vector<ChatCommand>() },
			{ "exit", "Will cause the bot to kill its own process", handleExitCommand, ACCESS_QUIT, vector<ChatCommand>() }
		};

		return vector<ChatCommand>(begin(commands), end(commands));
	}

	static void handleQuitCommand(const std::shared_ptr<User>& user, const std::string& target, const std::string& arguments)
	{
		if( !arguments.empty() )
			sSock->sendQuit(arguments);
		else
			sSock->sendQuit(config->getString("irc.quitmessage"));
	}

	static void handleExitCommand(const std::shared_ptr<User>& user, const std::string& target, const std::string& arguments)
	{
		if( !arguments.empty() )
			sSock->sendQuit(arguments);
		else
			sSock->sendQuit(config->getString("irc.quitmessage"));
		
		sScriptMgr->unregisterAll();
		exit(0);
	}
};

void setupQuitCommand()
{
	new QuitCommand();
}