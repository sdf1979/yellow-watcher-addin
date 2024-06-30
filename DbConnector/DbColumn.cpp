#include "DbColumn.h"

using namespace std;

namespace Soldy {

	pair<wstring, wstring> GetKeyValue(wstring str) {
		vector<wstring> tokens;
		wchar_t* buffer;
		wchar_t* token = wcstok_s(str.data(), L"=", &buffer);
		while (token) {
			tokens.push_back(token);
			token = wcstok_s(nullptr, L"=", &buffer);
		}
		if (tokens.size() == 1) {
			return { tokens[0], L"" };
		}
		else if (tokens.size() == 2) {
			return { tokens[0], tokens[1] };
		}
		return { L"", L"" };
	}

	unordered_map<wstring, wstring> SplitString(wstring hash_columns) {
		unordered_map<wstring, wstring> tokens;
		wchar_t* buffer;
		wchar_t* token = wcstok_s(hash_columns.data(), L", ", &buffer);
		while (token) {
			tokens.insert(GetKeyValue(token));
			token = wcstok_s(nullptr, L", ", &buffer);
		}
		return tokens;
	}

	bool DbColumns::ReadColumns(SQLHANDLE stmt, const wstring& hash_columns) {
		auto hash_columns_set = SplitString(hash_columns);
		columns_.clear();
		SQLSMALLINT num_columns;
		SQLRETURN sql_ret = SQLNumResultCols(stmt, &num_columns);
		if (SQL_SUCCESS_WITH_INFO == sql_ret || SQL_ERROR == sql_ret) {
			return false;
		}

		for (int i = 1; i <= num_columns; ++i) {
			columns_.push_back(DbColumn(stmt, i, hash_columns_set));
		}

		return true;
	}

	wostream& operator<<(wostream& os, const DbColumns& db_columns) {
		for (auto it = db_columns.columns_.begin(); it < db_columns.columns_.end(); ++it) {
			os << *it << endl;
		}
		return os;
	}

	DbColumn::DbColumn(SQLHANDLE handle, SQLUSMALLINT number, const unordered_map<wstring, wstring>& hash_columns):
		number_(number),
		data_type_(0),
		size_(0),
		decimal_digits_(0),
		nullable_(0),
		сalculate_hash_(false) {
		SQLSMALLINT name_length;
		SQLDescribeColW(handle, number_, NULL, 0, &name_length, &data_type_, &size_, &decimal_digits_, &nullable_);
		if (name_length) {
			name_.resize(static_cast<size_t>(name_length) + 1);
			SQLDescribeColW(handle, number_, &name_[0], static_cast<SQLSMALLINT>(name_.size()), &name_length, &data_type_,
				&size_, &decimal_digits_, &nullable_);
			name_.resize(name_length);
			if (auto search = hash_columns.find(name_); search != hash_columns.end()) {
				сalculate_hash_ = true;
				filter_hash_ = WideCharToUtf8(search->second);
			}
		}		
	}

	wostream& operator<<(wostream& os, const DbColumn& db_column) {
		os << db_column.number_ << ';' << db_column.name_ << ';' << db_column.data_type_ << ';' << SqlTypeToString(db_column.data_type_) << ';'
			<< db_column.size_ << ';' << db_column.nullable_ << ';' << db_column.сalculate_hash_ << ';';
		return os;
	}

}