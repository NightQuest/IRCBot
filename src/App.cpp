#include "preCompiled.h"

AppException::AppException(const std::string& message) throw() : std::runtime_error(message)
{
}

AppException::AppException(const std::string& message, const std::string& detail) throw() : std::runtime_error(message + ": " + detail)
{
}

App::App()
{
	cout << "Loading config... ";
	if( !config.load("IRCBot.conf") )
	{
		cout << "Failed" << endl;
		throw AppException("Cannot load IRCBot.conf");
	}
	cout << "OK" << endl;
}

App::~App()
{
}

void App::run()
{
	do
	{
		if( config.getBool("irc.useident") )
		{
			/* launch an ident listener with a timeout */
		}

		// TODO: make this look nicer.
		irc = move(unique_ptr<IRCClient>(new IRCClient(config.getString("irc.hostname"), config.getUInt("irc.port"), config.getBool("irc.ssl"), config)));

		irc->process();

	} while( config.getBool("irc.autoreconnect") );
}
