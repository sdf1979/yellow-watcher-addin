#include "DbColumn.h"

using namespace std;

namespace Soldy {

	bool DbColumns::ReadColumns(SQLHANDLE stmt) {
		columns_.clear();
		SQLSMALLINT num_columns;
		SQLRETURN sql_ret = SQLNumResultCols(stmt, &num_columns);
		if (SQL_SUCCESS_WITH_INFO == sql_ret || SQL_ERROR == sql_ret) {
			return false;
		}

		for (int i = 1; i <= num_columns; ++i) {
			columns_.push_back(DbColumn(stmt, i));
		}

		return true;
	}

	wostream& operator<<(wostream& os, const DbColumns& db_columns) {
		for (auto it = db_columns.columns_.begin(); it < db_columns.columns_.end(); ++it) {
			os << *it << endl;
		}
		return os;
	}

	DbColumn::DbColumn(SQLHANDLE handle, SQLUSMALLINT number) :
		number_(number),
		data_type_(0),
		size_(0),
		decimal_digits_(0),
		nullable_(0) {
		SQLSMALLINT name_length;
		SQLDescribeColW(handle, number_, NULL, 0, &name_length, &data_type_, &size_, &decimal_digits_, &nullable_);
		if (name_length) {
			name_.resize(static_cast<size_t>(name_length) + 1);
			SQLDescribeColW(handle, number_, &name_[0], static_cast<SQLSMALLINT>(name_.size()), &name_length, &data_type_,
				&size_, &decimal_digits_, &nullable_);
			name_.resize(name_length);
		}
	}

	wostream& operator<<(wostream& os, const DbColumn& db_column) {
		os << db_column.number_ << ';' << db_column.name_ << ';' << db_column.data_type_ << ';' << SqlTypeToString(db_column.data_type_) << ';'
			<< db_column.size_ << ';' << db_column.nullable_ << ';';
		return os;
	}

}