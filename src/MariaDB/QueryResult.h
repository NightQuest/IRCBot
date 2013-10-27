#pragma once

namespace MariaDB
{
	class QueryResultSet
	{
	private:
		MYSQL_RES* res;
		MYSQL_FIELD* fields;

	protected:
		long long rowCount;
		unsigned int fieldCount;
		QueryRow* row;

	public:
		QueryResultSet(MYSQL_RES* _res, MYSQL_FIELD* _fields, long long _rowCount, unsigned int fieldCount);
		~QueryResultSet();

		long long getRowCount() const { return rowCount; }
		unsigned int getFieldCount() const { return fieldCount; }

		bool nextRow();
		QueryRow* getRow() const { return row; }
	};
};
