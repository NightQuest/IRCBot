#pragma once
#include "preCompiled.h"

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
