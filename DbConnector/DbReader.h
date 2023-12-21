#pragma once

#include "DbHeader.h"
#include "DbColumn.h"
#include "DbValue.h"

#define READER_BUFFER_SIZE 10485760

namespace Soldy {

	class DbReader {
	public:
		DbReader(SQLHANDLE stmt, DbColumns columns);
		bool BindBuffer();
		bool Next();
		bool BindBufferAvailable() { return column_bind_available_; }
		std::vector<DbValue>& Row() { return values_; }
	private:
		SQLHANDLE stmt_;
		DbColumns columns_;
		std::vector<DbValue> values_;
		bool column_bind_available_;
		std::vector<unsigned char> buffer_;
		bool GetColumnData();
		bool GetDynamicData(DbColumn& column, DbValue& value);
	};

}
