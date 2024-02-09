//TODO debug
#include <iostream>

#include "DbValue.h"

using namespace std;

namespace Soldy {

	DbValue::DbValue(SQLSMALLINT data_type, size_t size):
		data_type_(data_type),
		str_len_or_ind_(0) {
		buffer_.resize(SizeType(data_type_, size));
	}

	void DbValue::SetValue(unsigned char* ch) {
		if (str_len_or_ind_ > 0) {
			if (static_cast<uint64_t>(str_len_or_ind_) > buffer_.size()) buffer_.resize(str_len_or_ind_);
			memcpy(&buffer_[0], ch, str_len_or_ind_);
		}
	}

	const bool DbValue::IsValueSet() const {
		return buffer_.size() && SQL_NULL_DATA != str_len_or_ind_ && SQL_NO_TOTAL != str_len_or_ind_;
	}

	DbValueType DbValue::ValueType() {
		switch (data_type_) {
		case SQL_CHAR:
			return DbValueType::String;
		case SQL_NUMERIC:
			return DbValueType::Number;
		case SQL_DECIMAL:
			return DbValueType::Number;
		case SQL_INTEGER:
			return DbValueType::Number;
		case SQL_SMALLINT:
			return DbValueType::Number;
		case SQL_FLOAT:
			return DbValueType::Number;
		case SQL_REAL:
			return DbValueType::Number;
		case SQL_DOUBLE:
			return DbValueType::Number;
		case SQL_DATETIME:						//SQL_DATE
			return DbValueType::Datetime;
		case SQL_TIME:							//SQL_INTERVAL
			return DbValueType::Datetime;
		case SQL_TIMESTAMP:
			return DbValueType::Datetime;
		case SQL_VARCHAR:
			return DbValueType::String;
		case SQL_TYPE_DATE:
			return DbValueType::Datetime;
		case SQL_TYPE_TIME:
			return DbValueType::Datetime;
		case SQL_TYPE_TIMESTAMP:
			return DbValueType::Datetime;
		case SQL_LONGVARCHAR:
			return DbValueType::String;
		case SQL_BINARY:
			return DbValueType::Binary;
		case SQL_VARBINARY:
			return DbValueType::Binary;
		case SQL_LONGVARBINARY:
			return DbValueType::Binary;
		case SQL_BIGINT:
			return DbValueType::Number;
		case SQL_TINYINT:
			return DbValueType::Number;
		case SQL_BIT:
			return DbValueType::Number;
		case SQL_WCHAR:
			return DbValueType::String;
		case SQL_WVARCHAR:
			return DbValueType::String;
		case SQL_WLONGVARCHAR:
			return DbValueType::String;
		case SQL_GUID:
			return DbValueType::Guid;
		case SQL_XML:
			return DbValueType::Xml;
		default:
			return DbValueType::Unknown;
		}
	}

	bool DbValue::IsShortInt() {
		return SQL_SMALLINT == data_type_;
	}

	bool DbValue::IsInt() {
		return SQL_INTEGER == data_type_;
	}

	bool DbValue::IsInt64() {
		return SQL_BIGINT == data_type_;
	}

	bool DbValue::IsFloat() {
		return SQL_REAL == data_type_ || SQL_FLOAT == data_type_;
	}

	optional<short int> DbValue::AsShortInt() {
		if (IsValueSet()) {
			return *reinterpret_cast<short int*>(&buffer_[0]);
		}
		else {
			return nullopt;
		}
	}

	optional<long int> DbValue::AsInt() {
		if (IsValueSet()) {
			return *reinterpret_cast<long int*>(&buffer_[0]);
		}
		else {
			return nullopt;
		}
	}

	optional<long long> DbValue::AsInt64() {
		if (IsValueSet()) {
			return *reinterpret_cast<long long*>(&buffer_[0]);
		}
		else {
			return nullopt;
		}
	}

	optional<float> DbValue::AsFloat() {
		if (IsValueSet()) {
			return *reinterpret_cast<float*>(&buffer_[0]);
		}
		else {
			return nullopt;
		}
	}

	optional<string> DbValue::AsString() {
		auto value = AsWstring();
		if (value) {
			return WideCharToUtf8(*value);
		}
		else {
			return nullopt;
		}
	}

	optional<wstring> DbValue::AsWstring() {
		if (!IsValueSet()) return nullopt;;
		switch (data_type_) {
		case SQL_CHAR:
			return L"SQL_CHAR";
		case SQL_NUMERIC:
			return L"SQL_NUMERIC";
		case SQL_DECIMAL:
			return L"SQL_DECIMAL";
		case SQL_INTEGER:
			return to_wstring(*reinterpret_cast<long int*>(&buffer_[0]));
		case SQL_SMALLINT:
			return to_wstring(*reinterpret_cast<short int*>(&buffer_[0]));
		case SQL_FLOAT:
			return to_wstring(*reinterpret_cast<float*>(&buffer_[0]));
		case SQL_REAL:
			return to_wstring(*reinterpret_cast<float*>(&buffer_[0]));
		case SQL_DOUBLE:
			return L"SQL_DOUBLE";
		case SQL_DATETIME:						//SQL_DATE
			return L"SQL_DATETIME";
		case SQL_TIME:							//SQL_INTERVAL
			return L"SQL_TIME";
		case SQL_TIMESTAMP:
			return L"SQL_TIMESTAMP";
		case SQL_VARCHAR:
			return Utf8ToWideChar(string(reinterpret_cast<char*>(&buffer_[0]), str_len_or_ind_ / sizeof(char)));
		case SQL_TYPE_DATE:
			return L"SQL_TYPE_DATE";
		case SQL_TYPE_TIME:
			return L"SQL_TYPE_TIME";
		case SQL_TYPE_TIMESTAMP: {
			wstringstream ss;
			TIMESTAMP_STRUCT* ts = reinterpret_cast<TIMESTAMP_STRUCT*>(&buffer_[0]);
			ss <<
				ts->year << '-' <<
				setfill(L'0') << setw(2) << ts->month << '-' <<
				setfill(L'0') << setw(2) << ts->day << 'T' <<
				setfill(L'0') << setw(2) << ts->hour << ':' <<
				setfill(L'0') << setw(2) << ts->minute << ':' <<
				setfill(L'0') << setw(2) << ts->second << '.' <<
				setfill(L'0') << setw(3) << ts->fraction / 1000000;
			return ss.str();
		}
		case SQL_LONGVARCHAR:
			return L"SQL_LONGVARCHAR";
		case SQL_BINARY: {
			wstringstream ss;
			unsigned char* ch = reinterpret_cast<unsigned char*>(&buffer_[0]);
			unsigned char* end = ch + str_len_or_ind_;
			ss << L"0x";
			for (; ch < end; ++ch) {
				ss << setfill(L'0') << setw(2) << hex << *ch;
			}
			return ss.str();
		}
		case SQL_VARBINARY: {
			wstringstream ss;
			unsigned char* ch = reinterpret_cast<unsigned char*>(&buffer_[0]);
			unsigned char* end = ch + str_len_or_ind_;
			ss << L"0x";
			for (; ch < end; ++ch) {
				ss << setfill(L'0') << setw(2) << hex << *ch;
			}
			return ss.str();
		}
		case SQL_LONGVARBINARY:
			return L"SQL_LONGVARBINARY";
		case SQL_BIGINT:
			return to_wstring(*reinterpret_cast<long long*>(&buffer_[0]));
		case SQL_TINYINT:
			return L"SQL_TINYINT";
		case SQL_BIT:
			return to_wstring(*reinterpret_cast<unsigned char*>(&buffer_[0]));
		case SQL_WCHAR:
			return L"SQL_WCHAR";
		case SQL_WVARCHAR:
			return wstring(reinterpret_cast<wchar_t*>(&buffer_[0]), str_len_or_ind_ / sizeof(wchar_t));
		case SQL_WLONGVARCHAR:
			return wstring(reinterpret_cast<wchar_t*>(&buffer_[0]), str_len_or_ind_ / sizeof(wchar_t));
		case SQL_GUID: {
			wstringstream ss;
			SQLGUID* guid = reinterpret_cast<SQLGUID*>(&buffer_[0]);
			ss << hex << guid->Data1 << '-' << guid->Data2 << '-' << guid->Data3 << '-' << *(guid->Data4) << *(guid->Data4 + 1) << '-' <<
				*(guid->Data4 + 2) << *(guid->Data4 + 3) << *(guid->Data4 + 4) << *(guid->Data4 + 5) << *(guid->Data4 + 6) << *(guid->Data4 + 7);
			return ss.str();
		}
		case SQL_XML:
			return wstring(reinterpret_cast<wchar_t*>(&buffer_[0]), str_len_or_ind_ / sizeof(wchar_t));
		default:
			return L"SQL_UNKNOWN_TYPE";
		}
	}

}