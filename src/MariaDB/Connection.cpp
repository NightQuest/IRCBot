#include "preCompiled.h"

MariaDB::Connection::Connection(bool _keepAlive) : conn(nullptr), keepAlive(_keepAlive)
{
}

MariaDB::Connection::~Connection()
{
	close();
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

bool MariaDB::Connection::escape(std::string& str)
{
	unsigned long requested_length = static_cast<unsigned long>(str.size());
	char* tmp = new char[str.size()*2+1];
	unsigned long ret = mysql_real_escape_string(conn, tmp, str.c_str(), requested_length);
	if( ret >= requested_length )
		str = tmp;
	delete[] tmp;
	return ret >= requested_length;
}

bool MariaDB::Connection::open(const std::string& hostname, const std::string& username, const std::string& password, int port, const std::string& database, int flags)
{
	if( !conn && !(conn = mysql_init(nullptr)) )
		return false;

	my_bool reconnect = 1;
	mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect);

	if( !mysql_real_connect(conn, hostname.c_str(), username.c_str(), password.c_str(), database.c_str(), port, nullptr, flags) )
		return false;

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
