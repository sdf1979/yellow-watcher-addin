#include "DbFunction.h"

using namespace std;

namespace Soldy {

	size_t SizeType(SQLSMALLINT data_type, SQLULEN size) {
		switch (data_type) {
		case SQL_CHAR:
			wcout << "DbFunction::SizeType not size for SQL_CHAR";
			break;
		case SQL_NUMERIC:
			wcout << "DbFunction::SizeType not size for SQL_NUMERIC";
			break;
		case SQL_DECIMAL:
			wcout << "DbFunction::SizeType not size for SQL_DECIMAL";
			break;
		case SQL_INTEGER:
			return sizeof(long int);
		case SQL_SMALLINT:
			return sizeof(short int);
		case SQL_FLOAT:
			return sizeof(float);
		case SQL_REAL:
			return sizeof(float);
		case SQL_DOUBLE:
			wcout << "DbFunction::SizeType not size for SQL_DOUBLE";
			break;
		case SQL_DATETIME:						//SQL_DATE
			wcout << "DbFunction::SizeType not size for SQL_DATETIME";
			break;
		case SQL_TIME:							//SQL_INTERVAL
			wcout << "DbFunction::SizeType not size for SQL_TIME";
			break;
		case SQL_TIMESTAMP:
			wcout << "DbFunction::SizeType not size for SQL_TIMESTAMP";
			break;
		case SQL_VARCHAR:
			return size ? sizeof(char) * (size + 1) : 0;
			break;
		case SQL_TYPE_DATE:
			wcout << "DbFunction::SizeType not size for SQL_TYPE_DATE";
			break;
		case SQL_TYPE_TIME:
			wcout << "DbFunction::SizeType not size for SQL_TYPE_TIME";
			break;
		case SQL_TYPE_TIMESTAMP:
			return sizeof(TIMESTAMP_STRUCT);
		case SQL_LONGVARCHAR:
			wcout << "DbFunction::SizeType not size SQL_LONGVARCHAR";
			break;
		case SQL_BINARY:
			return sizeof(unsigned char) * size;
		case SQL_VARBINARY:
			return sizeof(unsigned char) * size;
		case SQL_LONGVARBINARY:
			wcout << "DbFunction::SizeType not size SQL_LONGVARBINARY";
			break;
		case SQL_BIGINT:
			return sizeof(int64_t);
		case SQL_TINYINT:
			wcout << "DbFunction::SizeType not size SQL_TINYINT";
			break;
		case SQL_BIT:
			return sizeof(unsigned char);
		case SQL_WCHAR:
			wcout << "DbFunction::SizeType not size SQL_WCHAR";
			break;
		case SQL_WVARCHAR:
			return size ? sizeof(wchar_t) * (size + 2) : 0;
		case SQL_WLONGVARCHAR:
			//return size ? sizeof(wchar_t) * (size + 2) : 0;
			//Под текст запроса возвращаем буфер в 10Мб, т.к. size = 1073741823
			return 10485760;
		case SQL_GUID:
			return sizeof(SQLGUID);
		case SQL_XML:
			return size ? sizeof(wchar_t) * (size + 2) : 0;
		default:
			wcout << "DbFunction::SizeType not size SQL_UNKNOWN_TYPE";
		}
		return 0;
	}

	wstring SqlTypeToString(SQLSMALLINT type) {
		switch (type) {
		case SQL_CHAR:
			return L"SQL_CHAR";
		case SQL_NUMERIC:
			return L"SQL_NUMERIC";
		case SQL_DECIMAL:
			return L"SQL_DECIMAL";
		case SQL_INTEGER:
			return L"SQL_INTEGER";
		case SQL_SMALLINT:
			return L"SQL_SMALLINT";
		case SQL_FLOAT:
			return L"SQL_FLOAT";
		case SQL_REAL:
			return L"SQL_REAL";
		case SQL_DOUBLE:
			return L"SQL_DOUBLE";
		case SQL_DATETIME:						//SQL_DATE
			return L"SQL_DATETIME";
		case SQL_TIME:							//SQL_INTERVAL
			return L"SQL_TIME";
		case SQL_TIMESTAMP:
			return L"SQL_TIMESTAMP";
		case SQL_VARCHAR:
			return L"SQL_VARCHAR";
		case SQL_TYPE_DATE:
			return L"SQL_TYPE_DATE";
		case SQL_TYPE_TIME:
			return L"SQL_TYPE_TIME";
		case SQL_TYPE_TIMESTAMP:
			return L"SQL_TYPE_TIMESTAMP";
		case SQL_LONGVARCHAR:
			return L"SQL_LONGVARCHAR";
		case SQL_BINARY:
			return L"SQL_BINARY";
		case SQL_VARBINARY:
			return L"SQL_VARBINARY";
		case SQL_LONGVARBINARY:
			return L"SQL_LONGVARBINARY";
		case SQL_BIGINT:
			return L"SQL_BIGINT";
		case SQL_TINYINT:
			return L"SQL_TINYINT";
		case SQL_BIT:
			return L"SQL_BIT";
		case SQL_WCHAR:
			return L"SQL_WCHAR";
		case SQL_WVARCHAR:
			return L"SQL_WVARCHAR";
		case SQL_WLONGVARCHAR:
			return L"SQL_WLONGVARCHAR";
		case SQL_GUID:
			return L"SQL_GUID";
		case SQL_XML:
			return L"SQL_XML";
		default:
			return L"SQL_UNKNOWN_TYPE";
		}
	}

	SQLSMALLINT SqlTypeToC(SQLSMALLINT type) {
		switch (type) {
		case SQL_CHAR:
			wcout << "SqlTypeToC not allocate for SQL_CHAR";
			break;
		case SQL_NUMERIC:
			wcout << "SqlTypeToC not allocate for SQL_NUMERIC";
			break;
		case SQL_DECIMAL:
			wcout << "SqlTypeToC not allocate for SQL_DECIMAL";
			break;
		case SQL_INTEGER:
			return SQL_C_SLONG;
		case SQL_SMALLINT:
			return SQL_C_SSHORT;
		case SQL_FLOAT:
			return SQL_C_FLOAT;
		case SQL_REAL:
			return SQL_C_FLOAT;
		case SQL_DOUBLE:
			wcout << "SqlTypeToC not allocate for SQL_DOUBLE";
			break;
		case SQL_DATETIME:						//SQL_DATE
			wcout << "SqlTypeToC not allocate for SQL_DATETIME";
			break;
		case SQL_TIME:							//SQL_INTERVAL
			wcout << "SqlTypeToC not allocate for SQL_TIME";
			break;
		case SQL_TIMESTAMP:
			wcout << "SqlTypeToC not allocate for SQL_TIMESTAMP";
			break;
		case SQL_VARCHAR:
			return SQL_C_CHAR;
		case SQL_TYPE_DATE:
			wcout << "SqlTypeToC not allocate for SQL_TYPE_DATE";
			break;
		case SQL_TYPE_TIME:
			wcout << "SqlTypeToC not allocate for SQL_TYPE_TIME";
			break;
		case SQL_TYPE_TIMESTAMP:
			return SQL_C_TYPE_TIMESTAMP;
		case SQL_LONGVARCHAR:
			wcout << "SqlTypeToC not allocate for SQL_LONGVARCHAR";
			break;
		case SQL_BINARY:
			return SQL_C_BINARY;
		case SQL_VARBINARY:
			return SQL_C_BINARY;
		case SQL_LONGVARBINARY:
			wcout << "SqlTypeToC not allocate for SQL_LONGVARBINARY";
			break;
		case SQL_BIGINT:
			return SQL_C_SBIGINT;
		case SQL_TINYINT:
			wcout << "SqlTypeToC not allocate for SQL_TINYINT";
			break;
		case SQL_BIT:
			return SQL_C_BIT;
		case SQL_WCHAR:
			wcout << "SqlTypeToC not allocate for SQL_WCHAR";
			break;
		case SQL_WVARCHAR:
			return SQL_C_WCHAR;
		case SQL_WLONGVARCHAR:
			return SQL_C_WCHAR;
		case SQL_GUID:
			return SQL_C_GUID;
		case SQL_XML:
			return SQL_C_BINARY;
		default:
			wcout << "SqlTypeToC - SQL_UNKNOWN_TYPE";
		}
	}

	wstring DbGetLastError(SQLSMALLINT handle_type, SQLHANDLE handle) {
		std::wstring last_error;
		SQLLEN records = 0;
		SQLGetDiagFieldW(handle_type, handle, 0, SQL_DIAG_NUMBER, &records, 0, 0);
		wstring sql_state(6, '\0'), msg(SQL_MAX_MESSAGE_LENGTH, '\0');
		SQLINTEGER NativeError;
		SQLSMALLINT msg_len;
		SQLSMALLINT i = 1;
		while (i <= records) {
			if (SQL_NO_DATA != SQLGetDiagRecW(handle_type, handle, i, &sql_state[0], &NativeError, &msg[0], static_cast<SQLSMALLINT>(msg.size()), &msg_len)) {
				last_error.append(wstring(&msg[0], msg_len));
			}
			else {
				break;
			}
			i++;
		}
		return last_error;
	}

	wstring Utf8ToWideChar(const string& str) {
		int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), NULL, 0);
		wstring wstr(count, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), &wstr[0], count);
		return wstr;
	}

	wstring AnsiToWideChar(const string& str) {
		int count = MultiByteToWideChar(CP_THREAD_ACP, 0, str.c_str(), static_cast<int>(str.length()), NULL, 0);
		wstring wstr(count, 0);
		MultiByteToWideChar(CP_THREAD_ACP, 0, str.c_str(), static_cast<int>(str.length()), &wstr[0], count);
		return wstr;
	}

	string WideCharToUtf8(const wstring& wstr) {
		int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.length()), NULL, 0, NULL, NULL);
		string str(count, 0);
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
		return str;
	}
}