#pragma once

namespace MariaDB
{
	class QueryRow
	{
	private:
		MYSQL_ROW row;
		MYSQL_FIELD* fields;
		unsigned int fieldCount;
		unsigned long* columnLengths;

	public:
		QueryRow(MYSQL_ROW _row, MYSQL_FIELD* _fields, unsigned int _fieldCount, unsigned long* _columnLengths);
		~QueryRow();

		QueryRowField operator[](const std::string& in) const;
		QueryRowField operator[](unsigned int in) const;

		unsigned int getFieldCount() const { return fieldCount; }
	};
};
