#pragma once

namespace MariaDB
{
	class QueryRowField
	{
	private:
		char *data;

	public:
		QueryRowField(char *_data);
		~QueryRowField();
		
		bool getBool() const { return getUInt8() == 1; }
		char const* getCString() const { return data; }
		std::string getString() const { return data ? data : ""; }

		char getInt8() const;
		short getInt16() const;
		int getInt32() const;
		long long getInt64() const;

		unsigned char getUInt8() const;
		unsigned short getUInt16() const;
		unsigned int getUInt32() const;
		unsigned long long getUInt64() const;

		float getFloat() const;
		double getDouble() const;

		bool isNull() const { return data == nullptr; }
	};
};
