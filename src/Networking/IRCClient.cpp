#include "preCompiled.h"

IRCException::IRCException(const std::string& message) throw() : std::runtime_error(message)
{
}

IRCException::IRCException(const std::string& message, const std::string& detail) throw() : std::runtime_error(message + ": " + detail)
{
}

IRCClient::IRCClient(const std::string& hostname, unsigned int port, bool ssl, const Config& _config) : Socket(hostname, port, ssl), config(_config), sentUser(false)
{
	connected = (hSock.socketHandle != INVALID_SOCKET);
	internalDB = unique_ptr<MariaDB::Connection>(new MariaDB::Connection(true));
	externalDB = unique_ptr<MariaDB::Connection>(new MariaDB::Connection(true));
}

IRCClient::~IRCClient()
{
	if( connected )
		sendLine("QUIT :" + config.getString("irc.quitmessage"));

	sScriptMgr->unregisterAll();
}

LineData IRCClient::parseLine(const std::string& line) const
{
	LineData data;
	istringstream iss(line);
	vector<string> lineParts;
	string part;

	data.raw = line;

	while( getline(iss, part, ' ') )
		if( !part.empty() )
			lineParts.push_back(part);

	if( lineParts.size() >= 2 && lineParts[0][0] != ':' )
	{
		// As far as I know, this only applies to PING
		data.command = lineParts[0];
		data.data = lineParts[1].substr(1);
	}
	else if( lineParts.size() >= 3 )
	{
		// All else
		size_t pos, pos2;

		data.author.full = lineParts[0].substr(1); // drop the :
		pos = data.author.full.find_first_of('!');
		if( pos != string::npos )
			data.author.nickname = data.author.full.substr(0, pos);
		pos2 = lineParts[0].find_first_of('@');
		if( pos2 != string::npos )
		{
			if( pos != string::npos )
				data.author.ident = data.author.full.substr(++pos, (pos2-1)-pos);

			data.author.hostname = data.author.full.substr(pos2);
		}

		data.command = lineParts[1];
		data.target = lineParts[2];

		std::stringstream str;
		str << lineParts[1];
		for( int x = 2; x < lineParts.size(); ++x )
		{
			if( lineParts[x][0] == ':' )
			{
				str << " :";
				pos = line.find(str.str());
				if( pos != string::npos )
					data.data = line.substr(pos + str.str().length());
				break;
			}
			else
			{
				str << " " << lineParts[x];
				data.params.push_back(lineParts[x]);
			}
		}
	}

	return data;
}

void IRCClient::process()
{
	cout << "Establishing internal database connection... ";
	if( !internalDB->open(config.getString("database.internal.hostname"), config.getString("database.internal.username"),
		config.getString("database.internal.password"), config.getUInt("database.internal.port"), config.getString("database.internal.defaultdb")) )
	{
		cout << "Failed" << endl;
		internalDB.reset();
	}
	else
		cout << "OK" << endl;
	
	cout << "Establishing external database connection... ";
	if( !externalDB->open(config.getString("database.external.hostname"), config.getString("database.external.username"),
		config.getString("database.external.password"), config.getUInt("database.external.port"), config.getString("database.external.defaultdb")) )
	{
		cout << "Failed" << endl;
		externalDB.reset();
	}
	else
		cout << "OK" << endl;

	setupScripts(hSock);

	vector<char> recvBuff(1024);
	while( connected )
	{
		int ret = recv(&recvBuff[0], 1024);
		if( ret <= 0 )
			return;

		istringstream iss(recvBuff.data());
		string line;
		while( getline(iss, line) )
		{
			if( line.empty() )
				continue;

			if (line[line.size() - 1] == '\r')
				line.resize(line.size() - 1);

			LineData data = parseLine(line);
			if( !data.command.empty() )
				handleSCommand(data);
		}
		std::memset(&recvBuff[0], 0, 1024);
	}
}

void IRCClient::handleSCommand(const LineData& data)
{
	if( data.command.empty() )
		return;

	if( data.command == "PING" )
	{
		sendLine("PONG :" + data.data);
		cout << "PING <-> PONG" << endl;

		if( externalDB && externalDB->keepAlive )
			externalDB->ping();
		if( internalDB && internalDB->keepAlive )
			internalDB->ping();
	}

	else if( data.command == "001" ) // Welcome
	{
		sentUser = true;
		cout << data.raw << endl;
	}

	else if( data.command == "372" ) // MOTD
	{
		cout << "MOTD: " << data.data << endl;
	}

	else if( data.command == "375" ) // beginning of /MOTD
	{
		cout << "MOTD: " << data.data << endl;
	}

	else if( data.command == "376" ) // end of /MOTD
	{
		joinChannel(config.getString("irc.channels"));
		cout << "MOTD: " << data.data << endl;
	}

	else if( data.command == "433" && activeNickname == config.getString("irc.nickname") ) // Nickname in use
	{
		setNick(config.getString("irc.altnickname"));
		cout << "E " << data.data << endl;
	}

	else if( data.command == "439" ) // Please wait while we process your connection.
	{
		cout << data.data << endl;
	}

	else if( data.command == "MODE" )
	{
		cout << "M " << data.author.nickname << " -> " << data.target << ":";
		for( const string& str : data.params )
			cout << " " << str;
		cout << endl;
	}

	else if( data.command == "JOIN" )
	{
		cout << "J " << data.author.nickname << " -> " << data.target << endl;
	}

	else if( data.command == "PART" )
	{
		cout << "P " << data.author.nickname << " -> " << data.target << endl;
	}

	else if( data.command == "QUIT" )
	{
		cout << "Q " << data.author.nickname;
		if( data.data.empty() )
			cout << endl;
		else
			cout << ": " << data.data << endl;
	}

	else if( data.command == "PRIVMSG" )
	{
		if( data.data[0] == 1 && data.data[data.data.size()-1] == 1 )
		{
			string str = data.data.substr(1, data.data.length()-2);
			size_t pos = str.find_first_of(' ');
			if( pos != string::npos )
			{
				string ctcp = str.substr(0, pos);
				string args = str.substr(++pos);
				
				sScriptMgr->onCTCP(data.author.nickname, data.target, ctcp, args);

				if( ctcp == "ACTION" )
					sScriptMgr->onChatAction(data.author.nickname, data.target, args);
			}
			else
				sScriptMgr->onCTCP(data.author.nickname, data.target, str, "");
		}
		sScriptMgr->onChatText(data.author.nickname, data.target, data.data);
		handlePRIVMSG(data);
	}

	else if( data.command == "NOTICE" )
	{
		if( data.target == "AUTH" )
			cout << "N " << data.target << ": " << data.data << endl;
		else
			cout << "N " << data.author.nickname << " -> " << data.target << ": " << data.data << endl;
		handleNOTICE(data);
	}

	else
		cout << data.raw << endl;
}

void IRCClient::handleCCommand(const std::string& command, const std::string& args, const LineData& data)
{
	if( command == "quit" && hasAccess(data.author.full, ACCESS_QUIT) )
	{
		sendLine("QUIT :" + config.getString("irc.quitmessage"));
	}

	else if( command == "exit" && hasAccess(data.author.full, ACCESS_QUIT) )
	{
		sendLine("QUIT :" + config.getString("irc.quitmessage"));
		exit(0);
	}

	else if( command == "say" && hasAccess(data.author.full, ACCESS_SAY) )
	{
		sendMessage(data.target, args);
	}

	else if( command == "me" && hasAccess(data.author.full, ACCESS_SAY) )
	{
		sendAction(data.target, args);
	}

	else if( command == "sql" && hasAccess(data.author.full, ACCESS_SQL) && externalDB )
	{
		MariaDB::QueryResult res = externalDB->query(args);
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
					ss << (*row)[x].getString();
				}

				sendMessage(data.target, ss.str());
			}
		}
		else
			sendMessage(data.target, "No results");
	}
}

void IRCClient::handlePRIVMSG(const LineData& data)
{
	if( data.data[0] == 1 && data.data[data.data.size()-1] == 1 )
		handleCTCP(data);

	else if( data.target != activeNickname )
	{
		if ( data.data[0] == '!' )
		{
			string command, args;

			size_t pos = data.data.find_first_of(' ');
			if( pos != string::npos )
			{
				command = data.data.substr(1, pos-1);
				args = data.data.substr(pos+1);
			}
			else
				command = data.data.substr(1, data.data.length()-1);

			handleCCommand(command, args, data);
		}
	}
}

void IRCClient::handleNOTICE(const LineData& data)
{
	if( !sentUser && data.target == "AUTH" && data.data.find("*** Found") != string::npos ) // we're connecting
	{
		sendLine("USER " + config.getString("irc.name") + " 8 * :" + config.getString("irc.name"));
		setNick(config.getString("irc.nickname"));
	}
}

void IRCClient::handleCTCP(const LineData& data)
{
	string query, args;

	size_t pos = data.data.find(' ');
	if( pos != string::npos )
	{
		query = data.data.substr(1, pos-1);
		args = data.data.substr(++pos, data.data.length()-pos);
	}
	else
		query = data.data.substr(1, data.data.length()-2);

	if( query == "VERSION" )
		sendCTCPResponse(data.author.nickname, "VERSION", config.getString("irc.ctcpversion"));
}

bool IRCClient::hasAccess(const std::string& user, unsigned int perm)
{
	if( user.empty() || perm == 0 || !internalDB )
		return false;

	MariaDB::QueryResult res = internalDB->query("SELECT `access` FROM `perms` WHERE `user` = '" + user + "';");
	if( res && res->getRowCount() != 0 )
	{
		MariaDB::QueryRow* row;
		if( res->nextRow() && (row = res->getRow()) )
			return ((*row)["access"].getUInt32() & perm) == perm;
	}

	return false;
}

void IRCClient::setNick(const std::string& newNick)
{
	if( connected && sendLine("NICK " + newNick) > 0 )
		activeNickname = newNick;
}

void IRCClient::joinChannel(const std::string& channel)
{
	if( !connected || channel.empty() )
		return;

	if( sendLine("JOIN " + channel) > 0 )
	{
		istringstream iss(channel);
		string chan;
		while( getline(iss,chan,',') )
		{
			if( chan.empty() )
				continue;

			channels.insert(make_pair(chan, Channel(chan)));
		}
	}
}
