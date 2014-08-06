#include "preCompiled.h"

class SQLCommand : public IRCScript
{
public:
	SQLCommand() : IRCScript("SQLCommand") { }
	
	std::vector<ChatCommand> getCommands() override
	{
		ChatCommand commands[] = {
			{ "sql", "Execute specified SQL and returns results", handleESQLCommand, ACCESS_SQL, vector<ChatCommand>() },
			{ "isql", "Execute specified SQL and returns results", handleISQLCommand, ACCESS_INTERNAL_SQL, vector<ChatCommand>() }

		};

		return vector<ChatCommand>(begin(commands), end(commands));
	}

	static void handleESQLCommand(const UserPtr& user, const std::string& target, const std::string& arguments)
	{
		if( !externalDB )
			return;

		handleSQL(externalDB.get(), user, target, arguments);
	}

	static void handleISQLCommand(const UserPtr& user, const std::string& target, const std::string& arguments)
	{
		if( !internalDB )
			return;

		handleSQL(internalDB.get(), user, target, arguments);
	}

	static void handleSQL(MariaDB::Connection* const conn, const UserPtr& user, const std::string& target, const std::string& arguments)
	{
		if( !conn )
			return;
		
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;

		MariaDB::QueryResult res = conn->query(arguments);
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
		else if( conn->getLastAffectedRows() != 0 )
		{
			string args = arguments;
			transform(args.begin(), args.end(), args.begin(), ::tolower);
			if( strncmp(args.c_str(), "insert ", 7) == 0 || strncmp(args.c_str(), "update ", 7) == 0 || strncmp(args.c_str(), "delete ", 7) == 0 )
				sSock->sendMessage(messageTarget, "Affected rows: " + to_string(conn->getLastAffectedRows()));
			else
				sSock->sendMessage(messageTarget, "No results");
		}
		else if( conn->getLastErrorNo() )
			sSock->sendMessage(messageTarget, "Error " + to_string(conn->getLastErrorNo()) + ": " + conn->getLastError());
		else
			sSock->sendMessage(messageTarget, "No results");
	}
};

void setupSQLCommand()
{
	new SQLCommand();
}