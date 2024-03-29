﻿#pragma once

#include "DbHeader.h"
#include <unordered_set>

namespace Soldy {

	class DbColumn {
	public:
		DbColumn(SQLHANDLE handle, SQLUSMALLINT number, const std::unordered_set<std::wstring>& hash_columns);
		SQLSMALLINT DataType() { return data_type_; }
		SQLULEN Size() { return size_; }
		SQLUSMALLINT Number() { return number_; }
		std::wstring Name() { return name_; }
		std::wstring DataTypeStr() { return SqlTypeToString(data_type_); }
		bool CalculateHash() { return сalculate_hash_; }
	private:
		SQLUSMALLINT number_;
		std::wstring name_;
		SQLSMALLINT data_type_;
		SQLULEN size_;
		SQLSMALLINT decimal_digits_;
		SQLSMALLINT nullable_;
		bool сalculate_hash_;
		friend std::wostream& operator<<(std::wostream& os, const DbColumn& db_column);
	};

	std::wostream& operator<<(std::wostream& os, const DbColumn& db_column);

	class DbColumns {
	public:
		bool ReadColumns(SQLHANDLE stmt, const std::wstring& hash_columns);
		std::vector<DbColumn>::iterator begin() { return columns_.begin(); }
		std::vector<DbColumn>::iterator end() { return columns_.end(); }
		size_t Size() { return columns_.size(); }
		DbColumn& operator[](const size_t index) { return columns_[index]; }
	private:
		std::vector<DbColumn> columns_;
		friend std::wostream& operator<<(std::wostream& os, const DbColumns& db_columns);
	};

	std::wostream& operator<<(std::wostream& os, const DbColumns& db_columns);
}
