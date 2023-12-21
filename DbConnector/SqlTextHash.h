#pragma once

#include <string>
#include <string_view>
#include <sstream>
#include <iomanip>
#include "openssl/evp.h"

namespace Soldy {

	std::string SqlHashDbMsSql(const std::string* str);

	void DeleteLeftParameters(std::string_view& sv);
	void DeleteSingleQuotesLeftRight(std::string_view& sv);
	void DeleteDoubleQuotesLeftRight(std::string_view& sv);
	void DeleteParam(std::string_view& sv);
	void DeleteSpExecuteSql(std::string_view& sv);
	void DeleteXmlTag(std::string_view& sv);
	void ChangeCreateClusteredIndex(std::string& sql_hash, std::string_view& sv);

	std::string_view ReplacingParameters(std::string_view sv, std::string& str);

	std::string GetSHA256(const std::string& str, std::stringstream& ss);
	std::string GetSHA256(const std::string& str);
}
