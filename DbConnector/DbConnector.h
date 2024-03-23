#pragma once

#include "DbHeader.h"
#include "DbColumn.h"
#include "DbReader.h"

namespace Soldy {
	
	class DbConnector{
	public:
		DbConnector();
		~DbConnector();
		bool Create();
		std::string GetDrivers();
		bool Connect(std::wstring driver, std::wstring server, int port, std::wstring db, std::wstring login, std::wstring password);
		std::wstring GetLastError() { return last_error_; }
		std::string Exec(std::wstring cmd, const std::wstring& hash_columns);
		std::string QueryHash(std::wstring query_text);
	private:
		SQLHANDLE env_;
		SQLHANDLE conn_;
		SQLHANDLE stmt_;
		std::wstring last_error_;
		std::wstring last_warning_;
		//void SetLastError(SQLSMALLINT handle_type, SQLHANDLE handle);
		bool ExecQuery(std::wstring& cmd);
		bool is_connect_;
	};

	
}
