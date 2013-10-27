#include "preCompiled.h"

MariaDB::Connection::Connection(bool _keepAlive) : conn(nullptr), keepAlive(_keepAlive)
{
}

MariaDB::Connection::~Connection()
{
	close();
	if( keepAlive )
		keepAliveThread.join();
}

std::string MariaDB::Connection::getClientInfo()
{
	return mysql_get_client_info();
}

std::string MariaDB::Connection::getLastError()
{
	return mysql_error(conn);
}

unsigned int MariaDB::Connection::getLastErrorNo()
{
	return mysql_errno(conn);
}

bool MariaDB::Connection::open(const std::string& hostname, const std::string& username, const std::string& password, int port, const std::string& database, int flags)
{
	if( !conn && !(conn = mysql_init(nullptr)) )
		return false;

	if( !mysql_real_connect(conn, hostname.c_str(), username.c_str(), password.c_str(), database.c_str(), port, nullptr, flags) )
		return false;

	if( keepAlive )
	{
		keepAliveThread = std::thread([=](){
			while( true )
			{
				std::chrono::seconds dur = std::chrono::seconds::zero();
				while( dur < std::chrono::seconds(60) )
				{
					if( !conn )
						return;
					std::this_thread::sleep_for(std::chrono::seconds(2));
				}
				if( mysql_ping(conn) != 0 )
					break;
			}
		});
	}

	return true;
}

void MariaDB::Connection::close()
{
	if( conn )
	{
		mysql_close(conn);
		conn = nullptr;
	}
}

bool MariaDB::Connection::ping()
{
	return mysql_ping(conn) == 0;
}

bool MariaDB::Connection::execute(const std::string& str)
{
	return mysql_query(conn, str.c_str()) == 0;
}

MariaDB::QueryResult MariaDB::Connection::query(const std::string& str)
{
	MYSQL_RES* res = nullptr;
	MYSQL_FIELD* fields = nullptr;
	long long rowCount = 0L;
	unsigned int fieldCount = 0;

	if( mysql_query(conn, str.c_str()) != 0 )
		return nullptr;

	res = mysql_store_result(conn);
	if( !res )
		return nullptr;

	rowCount = mysql_affected_rows(conn);
	fieldCount = mysql_field_count(conn);
	fields = mysql_fetch_fields(res);

	QueryResult ret(new QueryResultSet(res, fields, rowCount, fieldCount));
	return std::move(ret);
}
