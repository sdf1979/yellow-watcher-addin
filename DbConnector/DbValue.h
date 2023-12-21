#pragma once

#include "DbHeader.h"

namespace Soldy {

	class DbValue {
	public:
		DbValue(SQLSMALLINT data_type, size_t size);
		SQLPOINTER Buffer() { return &buffer_[0]; }
		const SQLLEN BufferSize() const { return buffer_.size(); }
		SQLLEN* StrLenOrInd() { return &str_len_or_ind_; }
		SQLSMALLINT DataType() { return data_type_; }
		void SetValue(unsigned char* ch);
		const bool IsValueSet() const;
		DbValueType ValueType();

		bool IsShortInt();
		bool IsInt();
		bool IsInt64();
		bool IsFloat();
		std::optional<short int> AsShortInt();
		std::optional<long int> AsInt();
		std::optional<long long> AsInt64();
		std::optional<float> AsFloat();
		std::optional<std::string> AsString();
		std::optional<std::wstring> AsWstring();
	private:
		SQLSMALLINT data_type_;
		SQLLEN str_len_or_ind_;
		std::vector<unsigned char> buffer_;
	};

}
