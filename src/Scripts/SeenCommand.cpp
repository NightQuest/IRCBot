#include "preCompiled.h"

class SeenCommand : public IRCScript
{
public:
	SeenCommand() : IRCScript("SeenCommand") { }
	
	std::vector<ChatCommand> getCommands() override
	{
		ChatCommand commands[] = {
			{ "seen", "Returns the last time the bot saw someone by the specified nickname", handleSeenCommand, ACCESS_NONE, vector<ChatCommand>() }
		};

		return vector<ChatCommand>(begin(commands), end(commands));
	}

	static void handleSeenCommand(const UserPtr& user, const std::string& target, const std::string& arguments)
	{
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;

		if( arguments.empty() )
		{
			sSock->sendMessage(messageTarget, "Who should I look for?");
			return;
		}

		string nickname = arguments;
		if( internalDB->escape(nickname) )
		{
			MariaDB::QueryResult res = internalDB->query("SELECT (CURRENT_TIMESTAMP - `timestamp`) AS `seconds`, `user` FROM `log` WHERE `user` LIKE '" + nickname + "!%@%' ORDER BY `id` DESC LIMIT 1");
			if( res && res->getRowCount() == 0 )
				sSock->sendMessage(messageTarget, "I don't know who " + arguments + " is.");
			else if( res )
			{
				MariaDB::QueryRow* row;
				if( res->nextRow() && (row = res->getRow()) )
				{
					string host = Util::explode((*row)["user"].getString(), '!')[1];
					unsigned long long seconds = (*row)["seconds"].getUInt64();

					stringstream ss;
					ss << arguments << "(" << host << ") was last seen ";
					
					if( seconds > YEAR )
					{
						ss << seconds / YEAR << " year";
						if( seconds >= YEAR*2 )
							ss << "s";
					}
					if( seconds > DAY )
					{
						if( seconds > YEAR )
							ss << ", ";
						ss << (seconds % YEAR) / DAY << " day";
						if( (seconds % YEAR) >= DAY*2 )
							ss << "s";
					}
					if( seconds > HOUR )
					{
						if( seconds > DAY )
							ss << ", ";
						ss << (seconds % DAY) / HOUR << " hour";
						if( (seconds % DAY) >= HOUR*2 )
							ss << "s";
					}
					if( seconds > MINUTE )
					{
						if( seconds > HOUR )
							ss << ", ";
						ss << (seconds % HOUR) / MINUTE << " minute";
						if( (seconds % HOUR) >= MINUTE*2 )
							ss << "s";
					}
					if( seconds % MINUTE )
					{
						if( seconds > MINUTE )
							ss << ", ";
						ss << (seconds % MINUTE) << " second";
						if( (seconds % MINUTE) >= 2 )
							ss << "s";
					}

					ss << " ago.";
					sSock->sendMessage(messageTarget, ss.str());
				}
			}
		}
	}
};

void setupSeenCommand()
{
	new SeenCommand();
}