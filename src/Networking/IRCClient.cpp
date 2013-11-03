#include "preCompiled.h"

IRCException::IRCException(const std::string& message) throw() : std::runtime_error(message)
{
}

IRCException::IRCException(const std::string& message, const std::string& detail) throw() : std::runtime_error(message + ": " + detail)
{
}

IRCClient::IRCClient(const std::string& hostname, unsigned int port, bool ssl, const std::string& user, const std::string& nick, const std::string& aNick)
	: Socket(hostname, port, ssl), joinUser(user), joinNickname(nick), joinAltNickname(aNick), sentUser(false)
{
	connected = (hSock != INVALID_SOCKET);
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
		pos = lineParts[0].find_first_of('!');
		if( pos != string::npos )
			data.author.nickname = lineParts[0].substr(0, pos);
		pos2 = lineParts[0].find_first_of('@');
		if( pos2 != string::npos )
		{
			data.author.ident = lineParts[0].substr(pos, pos2);
			data.author.hostname = lineParts[0].substr(pos2);
		}

		data.command = lineParts[1];
		data.param = lineParts[2];

		pos = line.find(lineParts[1] + ' ' + lineParts[2] + " :");
		if( pos != string::npos )
			data.data = line.substr(pos + string(lineParts[1] + ' ' + lineParts[2] + " :").length());
	}

	return data;
}

void IRCClient::process(Config config)
{
	vector<char> recvBuff(512);
	while( connected )
	{
		int ret = recv(&recvBuff[0], 512);
		if( ret <= 0 )
		{
			connected = false;
			return;
		}
		
		istringstream iss(recvBuff.data());
		string line;
		while( getline(iss, line) )
		{
			if (line[line.size() - 1] == '\r')
				line.resize(line.size() - 1);

			cout << line << endl;

			LineData data = parseLine(line);
			if( !data.command.empty() )
				handleCommand(data);
		}
		memset(&recvBuff[0], 0, recvBuff.size());
	}
}

void IRCClient::handleCommand(const LineData& data)
{
	if( data.command.empty() )
		return;

	if( data.command == "PING" )
		send("PONG :" + data.data);
	
	else if( !sentUser && data.command == "NOTICE" && data.param == "AUTH" && data.data.find("*** Found") != string::npos )
	{
		send("USER " + joinUser + " 8 * :" + joinUser + "\r\n");
		setNick(joinNickname);
		sentUser = true;
	}
}

void IRCClient::setNick(const std::string& newNick)
{
	if( connected && send("NICK " + newNick + "\r\n") > 0 )
		activeNickname = newNick;
}
