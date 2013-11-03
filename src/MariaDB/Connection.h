#pragma once

namespace MariaDB
{
	class Connection
	{
	private:
		MYSQL *conn;
		bool keepAlive;
		std::thread keepAliveThread;

	public:
		Connection(bool _keepAlive = false);
		~Connection();

		std::string getClientInfo();
		std::string getLastError();
		unsigned int getLastErrorNo();
		bool escape(std::string& str);
		bool open(const std::string& hostname, const std::string& username, const std::string& password = std::string(""), int port = 3306, const std::string& database = std::string(""), int flags = 0);
		void close();
		bool ping();
		bool execute(const std::string& str);
		std::unique_ptr<QueryResultSet> query(const std::string& str);
	};
};
