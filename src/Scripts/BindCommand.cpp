#include "preCompiled.h"

class BindCommand : public IRCScript
{
public:
	BindCommand() : IRCScript("BindCommand") { }
	
	std::vector<ChatCommand> getCommands() override
	{
		ChatCommand commands[] = {
			{ "bind", "Binds SQL to a command", handleBindCommand, ACCESS_BIND, vector<ChatCommand>() },
			{ "unbind", "Unbinds SQL from a command", handleUnbindCommand, ACCESS_BIND, vector<ChatCommand>() }
		};

		return vector<ChatCommand>(begin(commands), end(commands));
	}

	static void handleBindCommand(const UserPtr& user, const std::string& target, const std::string& arguments)
	{
		if( !internalDB )
			return;
		
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;

		string command, args = arguments;
		unsigned int required_access = 0;
		size_t ffo = args.find_first_of(' ');
		if( ffo == string::npos )
			sSock->sendMessage(messageTarget, "Syntax: <bind> <required_access> <SQL>");
		else
		{
			command = args.substr(0, ffo);
			transform(command.begin(), command.end(), command.begin(), ::tolower);
			args = args.substr(ffo+1);
			ffo = args.find_first_of(' ');
			if( ffo == string::npos )
				sSock->sendMessage(messageTarget, "Syntax: <bind> <required_access> <SQL>");
			else
			{
				required_access = stoul(args.substr(0, ffo));
				args = args.substr(ffo+1);

				bool is_used_internally = false;
				auto commands = sScriptMgr->getCommands();
				for( auto& comm : commands )
				{
					if( comm.command.compare(command) == 0 )
					{
						is_used_internally = true;
						break;
					}
				}

				if( is_used_internally )
					sSock->sendMessage(messageTarget, "That command is being used internally.");
				else
				{
					if( internalDB->escape(command) )
					{
						MariaDB::QueryResult res = internalDB->query("SELECT COUNT(*) FROM `binds` WHERE `bind` = '" + command + "'");
						if( res && res->getRowCount() != 0 )
						{
							MariaDB::QueryRow* row;
							if( res->nextRow() && (row = res->getRow()) )
							{
								if( (*row)[0].getUInt32() != 0 )
									sSock->sendMessage(messageTarget, "A bind with that name already exists.");
								else
								{
									string esql = args;
									if( internalDB->escape(esql) && 
										internalDB->execute("INSERT INTO `binds` (`bind`, `required_access`, `esql`) VALUES ('" + command + "', " + to_string(required_access) + ", '" + esql + "')") )
										sSock->sendMessage(messageTarget, "Successfully bound.");
									else
										sSock->sendMessage(messageTarget, "Could not successfully bind.");
								}
							}
						}
					}
				}
			}
		}
	}

	static void handleUnbindCommand(const UserPtr& user, const std::string& target, const std::string& arguments)
	{
		if( !internalDB )
			return;
		
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;

		if( arguments.empty() )
			sSock->sendMessage(messageTarget, "Please provide a bind");

		string command = arguments;
		if( internalDB->escape(command) )
		{
			MariaDB::QueryResult res = internalDB->query("SELECT COUNT(*) FROM `binds` WHERE `bind` = '" + command + "'");
			if( res && res->getRowCount() == 0 )
				sSock->sendMessage(messageTarget, "No bind with that name exists.");
			else
			{
				if( internalDB->execute("DELETE FROM `binds` WHERE `bind` = '" + command + "'") )
					sSock->sendMessage(messageTarget, "Successfully unbound.");
				else
					sSock->sendMessage(messageTarget, "Could not successfully unbind.");
			}
		}
	}
};

void setupBindCommand()
{
	new BindCommand();
}