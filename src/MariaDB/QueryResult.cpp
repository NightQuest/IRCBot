#include "preCompiled.h"

MariaDB::QueryResultSet::QueryResultSet(MYSQL_RES* _res, MYSQL_FIELD* _fields, long long _rowCount, unsigned int _fieldCount) :
	res(_res), fields(_fields), rowCount(_rowCount), fieldCount(_fieldCount), row(nullptr)
{
}

MariaDB::QueryResultSet::~QueryResultSet()
{
	if( res )
	{
		mysql_free_result(res);
		res = nullptr;
	}
	if( row )
	{
		delete row;
		row = nullptr;
	}
}

bool MariaDB::QueryResultSet::nextRow()
{
	if( row )
	{
		delete row;
		row = nullptr;
	}

	MYSQL_ROW _row = mysql_fetch_row(res);
	if( !_row )
		return false;

	unsigned long* columnLengths = mysql_fetch_lengths(res);
	if( !columnLengths )
		return false;

	row = new QueryRow(_row, fields, fieldCount, columnLengths);
	if( !row )
		return false;

	return true;
}
