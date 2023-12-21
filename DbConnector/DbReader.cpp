#include "DbReader.h"

using namespace std;

namespace Soldy {

	DbReader::DbReader(SQLHANDLE stmt, DbColumns columns) :
		stmt_(stmt),
		columns_(columns),
		column_bind_available_(true) {
		buffer_.resize(READER_BUFFER_SIZE);
		for (auto it = columns_.begin(); it < columns_.end(); ++it) {
			values_.push_back(DbValue(it->DataType(), it->Size()));
			column_bind_available_ = column_bind_available_ && it->Size();
		}
	}

	bool DbReader::BindBuffer() {
		if (!column_bind_available_) {
			return false;
		}
		size_t col_size = columns_.Size();
		for (size_t index = 0; index < values_.size(); ++index) {
			auto& column = columns_[index];
			auto& value = values_[index];
			SQLRETURN sql_ret = SQLBindCol(stmt_, column.Number(), SqlTypeToC(column.DataType()), value.Buffer(), value.BufferSize(), value.StrLenOrInd());
			if (SQL_SUCCESS_WITH_INFO == sql_ret || SQL_ERROR == sql_ret) {
				return false;
			}
		}
		return true;
	}

	bool DbReader::Next() {
		SQLRETURN sql_ret = SQLFetch(stmt_);
		if (sql_ret == SQL_SUCCESS || sql_ret == SQL_SUCCESS_WITH_INFO) {
			if (!column_bind_available_) {
				GetColumnData();
			}
			return true;
		}
		else {
			return false;
		}
	}

	bool DbReader::GetColumnData() {
		for (size_t index = 0; index < columns_.Size(); ++index) {
			DbColumn& column = columns_[index];
			DbValue& value = values_[index];
			if (column.Size()) {
				SQLGetData(stmt_, column.Number(), SqlTypeToC(value.DataType()), value.Buffer(), value.BufferSize(), value.StrLenOrInd());
			}
			else {
				GetDynamicData(column, value);
			}
		}
		return true;
	}

	bool DbReader::GetDynamicData(DbColumn& column, DbValue& value) {
		SQLRETURN sql_ret = SQLGetData(stmt_, column.Number(), SqlTypeToC(value.DataType()), &buffer_[0], buffer_.size(), value.StrLenOrInd());
		value.SetValue(&buffer_[0]);
		//memset(&buffer_[0], '\0', buffer_.size());
		return true;
	}
}