#include "preCompiled.h"

User::User(const std::string& _nickname, const std::string& _ident, const std::string& _hostname)
	: nickname(_nickname), ident(_ident), hostname(_hostname), permissions(0)
{
	if( internalDB )
	{
		MariaDB::QueryResult res = internalDB->query("SELECT `access` FROM `perms` WHERE `user` = '" + getFullAddress() + "';");
		if( res && res->getRowCount() != 0 )
		{
			MariaDB::QueryRow* row;
			if( res->nextRow() && (row = res->getRow()) )
				permissions = (*row)["access"].getUInt32();
		}
	}
}
