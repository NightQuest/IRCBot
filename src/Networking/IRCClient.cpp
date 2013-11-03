#include "preCompiled.h"

IRCException::IRCException(const std::string& message) throw() : std::runtime_error(message)
{
}

IRCException::IRCException(const std::string& message, const std::string& detail) throw() : std::runtime_error(message + ": " + detail)
{
}

IRCClient::IRCClient(const std::string& hostname, unsigned int port, bool ssl, const Config& _config) : Socket(hostname, port, ssl), config(_config), sentUser(false)
{
	connected = (hSock != INVALID_SOCKET);
	internalDB = move(unique_ptr<MariaDB::Connection>(new MariaDB::Connection(true)));
}

IRCClient::~IRCClient()
{
}

LineData IRCClient::parseLine(const std::string& line) const
{
	LineData data;
	istringstream iss(line);
	vector<string> lineParts;
	string part;

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
		
		if( lineParts.size() >= 4 && lineParts[3][0] != ':' )
		{
			data.target = lineParts[3];
			data.param = lineParts[2];
		}
		else
			data.target = lineParts[2];

		pos = line.find(lineParts[1] + ' ' + lineParts[2] + " :");
		if( pos != string::npos )
			data.data = line.substr(pos + string(lineParts[1] + ' ' + lineParts[2] + " :").length());
	}

	return data;
}

void IRCClient::process()
{
	if( !internalDB->open(config.getString("database.internal.hostname"), config.getString("database.internal.username"), config.getString("database.internal.password"), config.getUInt("database.internal.port"), config.getString("database.internal.defaultdb")) )
		internalDB.reset();

	vector<char> recvBuff(1024);
	while( connected )
	{
		int ret = recv(&recvBuff[0], 1024);
		if( ret <= 0 )
		{
			connected = false;
			return;
		}
		
		istringstream iss(recvBuff.data());
		string line;
		while( getline(iss, line) )
		{
			if( line.empty() )
				continue;

			if (line[line.size() - 1] == '\r')
				line.resize(line.size() - 1);

			cout << line << endl;

			LineData data = parseLine(line);
			if( !data.command.empty() )
				handleCommand(data);
		}
		std::memset(&recvBuff[0], 0, 1024);
	}
}

void IRCClient::handleCommand(const LineData& data)
{
	if( data.command.empty() )
		return;

	if( data.command == "PING" )
		send("PONG :" + data.data);

	else if( data.command == "001" )
		sentUser = true;

	else if( data.command == "433" && activeNickname == config.getString("irc.nickname") ) // Nickname in use
		setNick(config.getString("irc.altnickname"));

	else if( data.command == "376" && !config.getString("irc.channels").empty() ) // end of /MOTD
		joinChannel(config.getString("irc.channels"));

	else if( data.command == "PRIVMSG" && data.data == "\x1VERSION\x1" ) // ctcp VERSION
		send("NOTICE " + data.author.nickname + " :\x1VERSION " + config.getString("irc.ctcpversion") + "\x1\r\n");

	else if( !sentUser && data.command == "NOTICE" && data.target == "AUTH" && data.data.find("*** Found") != string::npos )
	{
		send("USER " + config.getString("irc.name") + " 8 * :" + config.getString("irc.name") + "\r\n");
		setNick(config.getString("irc.nickname"));
	}
}

void IRCClient::setNick(const std::string& newNick)
{
	if( connected && send("NICK " + newNick + "\r\n") > 0 )
		activeNickname = newNick;
}

void IRCClient::joinChannel(const std::string& channel)
{
	if( connected && send("JOIN " + channel + "\r\n") > 0 )
	{
		istringstream iss(channel);
		string chan;
		while( getline(iss,chan,',') )
		{
			if( chan.empty() )
				continue;

			channels.push_back(Channel(chan));
		}
	}
}
