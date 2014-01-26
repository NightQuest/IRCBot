#include "preCompiled.h"

class HelpCommand : public IRCScript
{
private:
	static time_t lastCommandSpam;

public:
	HelpCommand() : IRCScript("HelpCommand") { lastCommandSpam = 0; }

	std::vector<ChatCommand> getCommands() override
	{
		ChatCommand commands[] = {
			{ "help", "Provides help on a command", handleHelpCommand, ACCESS_NONE, vector<ChatCommand>() },
			{ "commands", "Lists all commands available to you", handleCommandsCommand, ACCESS_NONE, vector<ChatCommand>() }
		};

		return vector<ChatCommand>(begin(commands), end(commands));
	}

	static void handleHelpCommand(const UserPtr& user, const std::string& target, const std::string& arguments)
	{
		int count = 0;
		auto commands = sScriptMgr->getCommands();
		for( auto& comm : commands )
		{
			if( comm.command.compare(arguments) == 0 && user->hasAccess(comm.required_access) )
			{
				sSock->sendNotice(user->getNickname(), comm.command + " - " + comm.help);
				if( !comm.subcommands.empty() )
				{
					std::vector<std::string> list;
					for( auto& sub : comm.subcommands )
						if( user->hasAccess(sub.required_access) )
							list.push_back(sub.command + " - " + sub.help);

					if( !list.empty() )
					{
						sSock->sendNotice(user->getNickname(), "Available Sub-commands:");
						for( auto& txt : list )
							sSock->sendNotice(user->getNickname(), txt);
					}
				}
			}
		}
	}

	static void handleCommandsCommand(const UserPtr& user, const std::string& target, const std::string& arguments)
	{
		time_t thisSpam = time(NULL);
		time_t diff = (thisSpam - lastCommandSpam);
		if( diff < 10 )
		{
			sSock->sendNotice(user->getNickname(), "Please wait " + to_string((10 - diff)) + " seconds.");
			return;
		}
		lastCommandSpam = thisSpam;

		std::vector<std::string> list;
		auto commands = sScriptMgr->getCommands();
		for( auto& comm : commands )
		{
			if( user->hasAccess(comm.required_access) )
				list.push_back(comm.command + " - " + comm.help);
		}

		if( !list.empty() )
		{
			sSock->sendNotice(user->getNickname(), "The following commands are available to you:");
			int start = (arguments.empty() ? 0 : stoi(arguments) * 10);
			int pos = 0;
			for( auto& txt : list )
			{
				if( pos++ < start )
					continue;

				if( (pos - start) > 10 )
				{
					sSock->sendNotice(user->getNickname(), "To continue, type !commands " + to_string(pos / 10));
					break;
				}

				sSock->sendNotice(user->getNickname(), txt);
			}
		}
	}
};
time_t HelpCommand::lastCommandSpam;

void setupHelpCommand()
{
	new HelpCommand();
}