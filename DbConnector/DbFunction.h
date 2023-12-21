#pragma once

#include "DbHeader.h"

namespace Soldy {

	size_t SizeType(SQLSMALLINT data_type, SQLULEN size);
	std::wstring SqlTypeToString(SQLSMALLINT type);
	SQLSMALLINT SqlTypeToC(SQLSMALLINT type);
	std::wstring DbGetLastError(SQLSMALLINT handle_type, SQLHANDLE handle);

	std::wstring AnsiToWideChar(const std::string& str);
	std::wstring Utf8ToWideChar(const std::string& str);
	std::string WideCharToUtf8(const std::wstring& wstr);
}
