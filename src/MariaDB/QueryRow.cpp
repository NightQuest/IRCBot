#include "preCompiled.h"

MariaDB::QueryRow::QueryRow(MYSQL_ROW _row, MYSQL_FIELD* _fields, unsigned int _fieldCount, unsigned long* _columnLengths) :
	row(_row), fields(_fields), fieldCount(_fieldCount), columnLengths(_columnLengths)
{
}

MariaDB::QueryRow::~QueryRow()
{
}

MariaDB::QueryRowField MariaDB::QueryRow::operator[](const std::string& in) const
{
	for(unsigned int x = 0; x < fieldCount; x++)
	{
		if( in.compare(fields[x].name) == 0 )
			return QueryRowField(row[x]);
	}

	return QueryRowField(nullptr);
}

MariaDB::QueryRowField MariaDB::QueryRow::operator[](unsigned int in) const
{
	if( in > fieldCount )
		return QueryRowField(nullptr);
	return QueryRowField(row[in]);
}
