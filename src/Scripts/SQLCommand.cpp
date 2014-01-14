#include "preCompiled.h"

class SQLCommand : public IRCScript
{
public:
	SQLCommand() : IRCScript("SQLCommand") { }
	
	std::vector<ChatCommand> getCommands() override
	{
		ChatCommand commands[] = {
			{ "sql", "Execute specified SQL and returns results", handleSQLCommand, ACCESS_SQL, vector<ChatCommand>() }
		};

		return vector<ChatCommand>(begin(commands), end(commands));
	}

	static void handleSQLCommand(const UserPtr& user, const std::string& target, const std::string& arguments)
	{
		if( !externalDB )
			return;
		
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;

		MariaDB::QueryResult res = externalDB->query(arguments);
		if( res && res->getRowCount() != 0 )
		{
			MariaDB::QueryRow* row;
			int rows = 0;
			while( rows < 15 && res->nextRow() && (row = res->getRow()) )
			{
				rows++;
				stringstream ss;
				unsigned int rowCount = row->getFieldCount();
				for( unsigned int x = 0; x < rowCount; x++ )
				{
					if( x > 0 && x < rowCount )
						ss << ", ";

					std::string str = (*row)[x].getString();
					if( str.empty() )
						ss << "NULL";
					else
					{
						for( size_t x = 0; x < str.length(); x++ )
							if( str[x] == '\r' )
								ss << "\\r";
							else if( str[x] == '\n' )
								ss << "\\n";
							else
								ss << str[x];
					}
				}

				sSock->sendMessage(messageTarget, ss.str());
			}
		}
		else
			sSock->sendMessage(messageTarget, "No results");
	}
};

void setupSQLCommand()
{
	new SQLCommand();
}