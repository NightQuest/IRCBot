#include "preCompiled.h"

MariaDB::QueryRowField::QueryRowField(char *_data) : data(_data)
{
}

MariaDB::QueryRowField::~QueryRowField()
{
}

char MariaDB::QueryRowField::getInt8() const
{
	if( !data )
		return 0;
	return static_cast<char>(std::stol(data));
}

short MariaDB::QueryRowField::getInt16() const
{
	if( !data )
		return 0;
	return static_cast<short>(std::stol(data));
}

int MariaDB::QueryRowField::getInt32() const
{
	if( !data )
		return 0;
	return std::stoi(data);
}

long long MariaDB::QueryRowField::getInt64() const
{
	if( !data )
		return 0L;
	return std::stoll(data);
}

unsigned char MariaDB::QueryRowField::getUInt8() const
{
	if( !data )
		return 0;
	return static_cast<unsigned char>(std::stol(data));
}

unsigned short MariaDB::QueryRowField::getUInt16() const
{
	if( !data )
		return 0;
	return static_cast<unsigned short>(std::stol(data));
}

unsigned int MariaDB::QueryRowField::getUInt32() const
{
	if( !data )
		return 0;
	return static_cast<unsigned int>(std::stoul(data));
}

unsigned long long MariaDB::QueryRowField::getUInt64() const
{
	if( !data )
		return 0L;
	return static_cast<unsigned long long>(std::stoull(data));
}

float MariaDB::QueryRowField::getFloat() const
{
	if( !data )
		return 0.0f;
	return std::stof(data);
}

double MariaDB::QueryRowField::getDouble() const
{
	if( !data )
		return 0.0;
	return std::stod(data);
}
