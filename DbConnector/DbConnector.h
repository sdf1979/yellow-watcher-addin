#pragma once

#include "DbHeader.h"
#include "DbColumn.h"
#include "DbReader.h"

#define __FILENAME__ (strrchr(__FILE__, '\\')?strrchr(__FILE__, '\\')+1:__FILE__)
#if defined(__GNUC__)
#	define EXCEPTION string(__FILENAME__).append(" : ").append(to_string(__LINE__)).append("\n").append(__PRETTY_FUNCTION__)
#elif defined(_MSC_VER)
#	define EXCEPTION string(__FILENAME__).append(" : ").append(to_string(__LINE__)).append("\n").append(__FUNCSIG__)
#else
#   define EXCEPTION string(__FILENAME__).append(" : ").append(to_string(__LINE__)).append("\n").append(__func__)
#endif

namespace Soldy {
	
	class DbConnector{
	public:
		DbConnector();
		~DbConnector();
		bool Create();
		std::string GetDrivers();
		bool Connect(std::wstring driver, std::wstring server, int port, std::wstring db, std::wstring login, std::wstring password);
		void CloseConnect();
		std::wstring GetLastError() { return last_error_; }
		std::string Exec(std::wstring cmd, const std::wstring& hash_columns);
		std::string QueryHash(std::wstring query_text);
	private:
		SQLHANDLE env_;
		SQLHANDLE conn_;
		SQLHANDLE stmt_;
		std::wstring last_error_;
		std::wstring last_warning_;
		bool ExecQuery(std::wstring& cmd);
		bool is_connect_;
	};

	
}
