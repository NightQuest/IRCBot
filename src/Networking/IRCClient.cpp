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
	string tmpPart;

	data.raw = line;

	while( getline(iss, tmpPart, ' ') )
		if( !tmpPart.empty() )
			lineParts.push_back(tmpPart);

	size_t part = 0;
	if( lineParts[0][0] == ':' ) // We have an origin
	{
		std::string full, nickname, ident, hostname;

		full = lineParts[0].substr(1); // drop the :

		size_t pos = full.find_first_of('@');
		if( pos != string::npos )
		{
			hostname = full.substr(pos+1);
			size_t pos2 = full.find_first_of('!');
			if( pos2 != string::npos )
			{
				nickname = full.substr(0, pos2);
				ident = full.substr(pos2+1, (pos-1)-pos2);
			}
			else
				nickname = full.substr(0, pos);
		}
		else
			nickname = full;

		data.author.reset(new User(nickname, ident, hostname));
		part++;
	}

	data.command = lineParts[part++];

	for( size_t oldPart = part; part < lineParts.size(); part++ )
	{
		if( lineParts[part][0] == ':' ) // message
		{
			data.data = lineParts[part++].substr(1);
			for( ; part < lineParts.size(); part++ )
				data.data += " " + lineParts[part];

			break;
		}
		else
			data.params.push_back(lineParts[part]);
	}

	if( data.command == "JOIN" ) // TODO: parse comma delimited list
		data.target = data.data;

	else if( !data.params.empty() )
	{
		data.target = data.params[0];
		data.params.pop_front();
	}

	if( data.command == "MODE" && data.data.size() > 0 && data.params.empty() ) // hackfix: move modes to params
	{
		// TODO: parse modes
		iss.str(data.data);
		iss.clear();
		while( getline(iss, tmpPart, ' ') )
			if( !tmpPart.empty() )
				data.params.push_back(tmpPart);

		data.data.clear();
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
		sScriptMgr->onPing(data.data);

	else if( data.command == "001" ) // RPL_WELCOME
		sScriptMgr->onWelcome(data.data);

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
		if( !config->getBool("irc.channels.joinwhenidentified") )
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
		sScriptMgr->onJoin(data.author, data.target);

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
