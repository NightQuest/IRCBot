#pragma once
#include "preCompiled.h"

typedef unsigned short  ushort;
typedef unsigned int    uint;
typedef unsigned __int64 ulonglong;
typedef __int64	longlong;
typedef int sigset_t;
typedef	long off_t;
typedef __int64 os_off_t;
#ifdef _WIN64
typedef ulonglong rf_SetTimer;
#else
typedef uint rf_SetTimer;
#endif

#include <mysql.h>

#include "QueryRowField.h"
#include "QueryRow.h"
#include "QueryResult.h"
#include "Connection.h"

namespace MariaDB
{
	class QueryRowField;
	class QueryRow;
	class QueryResultSet;
	class Connection;

	typedef std::unique_ptr<QueryResultSet> QueryResult;
};
