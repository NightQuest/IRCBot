#include "preCompiled.h"

IRCException::IRCException(const std::string& message) throw() : std::runtime_error(message)
{
}

IRCException::IRCException(const std::string& message, const std::string& detail) throw() : std::runtime_error(message + ": " + detail)
{
}

IRCClient::IRCClient()
{
}

IRCClient::~IRCClient()
{
	if( sSock->isConnected() )
		sSock->sendQuit(config->getString("irc.quitmessage"));

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

		std::string full, nickname, ident, hostname;
		full = lineParts[0].substr(1); // drop the :
		pos = full.find_first_of('!');
		if( pos != string::npos )
			nickname = full.substr(0, pos);
		pos2 = lineParts[0].find_first_of('@');
		if( pos2 != string::npos )
		{
			if( pos != string::npos )
				ident = full.substr(++pos, (pos2-1)-pos);

			hostname = full.substr(pos2);
		}
		data.author.reset(new User(nickname, ident, hostname));

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
	setupScripts();

	std::string line;
	while( sSock->getLine(line) )
	{
		LineData data = parseLine(line);
		if( !data.command.empty() )
			handleSCommand(data);
		line.clear();
	}
}

void IRCClient::handleSCommand(const LineData& data)
{
	if( data.command.empty() )
		return;

	if( data.command == "PING" )
	{
		sSock->sendLine("PONG :" + data.data);
		cout << "PING <-> PONG" << endl;

		if( externalDB && externalDB->keepAlive )
			externalDB->ping();
		if( internalDB && internalDB->keepAlive )
			internalDB->ping();
	}

	else if( data.command == "001" ) // RPL_WELCOME
		sScriptMgr->onWelcome(data.raw);

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
		sSock->joinChannel(config->getString("irc.channels"));
		cout << "MOTD: " << data.data << endl;
	}

	else if( data.command == "433" && sSock->getNickname() == config->getString("irc.nickname") ) // Nickname in use
	{
		sSock->setNickname(config->getString("irc.altnickname"));
		cout << "E " << data.data << endl;
	}

	else if( data.command == "439" ) // Please wait while we process your connection.
	{
		cout << data.data << endl;
	}

	else if( data.command == "MODE" )
	{
		cout << "M " << data.author->getNickname() << " -> " << data.target << ":";
		for( const string& str : data.params )
			cout << " " << str;
		cout << endl;
	}

	else if( data.command == "JOIN" )
		sScriptMgr->onJoin(data.author, data.data);

	else if( data.command == "PART" )
		sScriptMgr->onPart(data.author, data.target, data.data);

	else if( data.command == "QUIT" )
		sScriptMgr->onQuit(data.author, data.data);

	else if( data.command == "PRIVMSG" )
	{
		if( data.data[0] == 1 && data.data[data.data.size()-1] == 1 )
		{
			string str = data.data.substr(1, data.data.length()-2);
			string ctcp = str;
			string args = "";
			size_t pos = str.find_first_of(' ');
			if( pos != string::npos )
			{
				ctcp = str.substr(0, pos);
				args = str.substr(++pos);

				if( ctcp == "ACTION" )
					sScriptMgr->onChatAction(data.author, data.target, args);
			}
			sScriptMgr->onCTCP(data.author, data.target, ctcp, args);
		}
		else
		{
			if( config->getString("irc.commandchars").find(data.data[0]) != string::npos )
			{
				auto tmp = Util::explode(data.data, ' ');
				tmp[0] = tmp[0].substr(1); // drop the commandchar
				handleCCommand(sScriptMgr->getCommands(), tmp, data);
			}
			sScriptMgr->onChatText(data.author, data.target, data.data);
		}
	}

	else if( data.command == "NOTICE" )
	{
		if( data.data[0] == 1 && data.data[data.data.size()-1] == 1 )
		{
			string str = data.data.substr(1, data.data.length()-2);
			string ctcp = str;
			string args = "";
			size_t pos = str.find_first_of(' ');
			if( pos != string::npos )
			{
				ctcp = str.substr(0, pos);
				args = str.substr(++pos);
			}
			sScriptMgr->onCTCPReply(data.author, data.target, ctcp, args);
		}
		else
			sScriptMgr->onNotice(data.author, data.target, data.data);
	}

	else
		cout << data.raw << endl;
}

bool IRCClient::handleCCommand(const std::vector<ChatCommand>& commands, const std::deque<std::string>& line, const LineData& data)
{
	deque<string> currentLine(line.begin()+1, line.end());
	string args = Util::implode(currentLine, ' ');

	for( auto itr = commands.begin(); itr != commands.end(); itr++ )
	{
		if( itr->command.compare(line[0]) == 0 && data.author->hasAccess(itr->required_access) )
		{
			bool ret = false;
			if( !itr->subcommands.empty() && !currentLine.empty() )
				ret = handleCCommand(itr->subcommands, currentLine, data);

			if( !ret )
			{
				if( itr->function != nullptr )
					itr->function(data.author, data.target, args);
				return true;
			}
		}
	}

	return false;
}
