#include "preCompiled.h"

std::unique_ptr<MariaDB::Connection> internalDB;
std::unique_ptr<MariaDB::Connection> externalDB;
std::unique_ptr<Config> config;

AppException::AppException(const std::string& message) throw() : std::runtime_error(message)
{
}

AppException::AppException(const std::string& message, const std::string& detail) throw() : std::runtime_error(message + ": " + detail)
{
}

App::App()
{
	cout << "Loading config... ";
	config.reset(new Config());
	if( !config->load("IRCBot.conf") )
	{
		cout << "Failed" << endl;
		throw AppException("Cannot load IRCBot.conf");
	}
	cout << "OK" << endl;

	internalDB.reset(new MariaDB::Connection(true));
	externalDB.reset(new MariaDB::Connection(true));

	cout << "Establishing internal database connection... ";
	if( !internalDB->open(config->getString("database.internal.hostname"), config->getString("database.internal.username"),
		config->getString("database.internal.password"), config->getUInt("database.internal.port"), config->getString("database.internal.defaultdb")) )
	{
		cout << "Failed" << endl;
		internalDB.reset();
	}
	else
		cout << "OK" << endl;

	cout << "Establishing external database connection... ";
	if( !externalDB->open(config->getString("database.external.hostname"), config->getString("database.external.username"),
		config->getString("database.external.password"), config->getUInt("database.external.port"), config->getString("database.external.defaultdb")) )
	{
		cout << "Failed" << endl;
		externalDB.reset();
	}
	else
		cout << "OK" << endl;
}

App::~App()
{
}

void App::run()
{
	do
	{
		if( config->getBool("ident.enable") )
		{
			IdentServer serv(config->getUInt("ident.listenport"));
			serv.runWithTimeout(config->getString("irc.name"), 60);
		}

		string hostname = config->getString("irc.hostname");
		unsigned int port = config->getUInt("irc.port");
		bool useSSL = config->getBool("irc.ssl");

		irc.reset(new IRCClient());
		try
		{
			sSock.reset(new IRCSocket(hostname, port, useSSL));
		}
		catch(SocketException& e)
		{
			cerr << e.what() << endl;
		}

		irc->process();

	} while( config->getBool("irc.autoreconnect") );
}
