#include "DbConnector.h"

using namespace std;

namespace Soldy {

	wstring GetLastErrorSql(SQLSMALLINT handle_type, SQLHANDLE handle) {
		wstring last_error;
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

	DbConnector::DbConnector():
		conn_(nullptr),
		env_(nullptr),
		stmt_(nullptr),
		is_connect_(false)
	{}

	DbConnector::~DbConnector() {
		CloseConnect();
	}

	bool DbConnector::Create() {
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env_)) {
			return false;
		}
		if (SQL_SUCCESS != SQLSetEnvAttr(env_, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, 0)) {
			return false;
		}
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, env_, &conn_)) {
			return false;
		}
		return true;
	}

	string DbConnector::GetDrivers() {
		boost::json::object j_result;
		boost::json::array j_drivers;
		wstring buffer(1024, '\0');
		SQLSMALLINT buf_len = static_cast<SQLSMALLINT>(buffer.size());
		wstring driver_attr(1024, '\0');
		SQLSMALLINT driver_attr_len = static_cast<SQLSMALLINT>(driver_attr.size());
		SQLSMALLINT buf_size, driver_attr_size;
		for (;;) {
			SQLRETURN sql_ret = SQLDriversW(env_, SQL_FETCH_NEXT, &buffer[0], buf_len, &buf_size, &driver_attr[0], driver_attr_len, &driver_attr_size);
			if (SQL_SUCCESS == sql_ret) {
				for (short i = 0; i < driver_attr_size; ++i) {
					if (driver_attr[i] == '\0') driver_attr[i] = ';';
				}
				j_drivers.emplace_back(WideCharToUtf8(wstring(buffer.c_str())));
			}
			else if (SQL_NO_DATA == sql_ret) {
				break;
			}
			else if (SQL_ERROR == sql_ret || SQL_SUCCESS_WITH_INFO == sql_ret) {
				last_error_ = GetLastErrorSql(SQL_HANDLE_STMT, stmt_);
				j_result.emplace("success", false);
				j_result.emplace("error", WideCharToUtf8(last_error_));
				return boost::json::serialize(j_result);
			}
			else if (SQL_INVALID_HANDLE == sql_ret) {
				j_result.emplace("success", false);
				j_result.emplace("error", "SQL_INVALID_HANDLE");
				return boost::json::serialize(j_result);
			}
		}
		j_result.emplace("success", true);
		j_result.emplace("drivers", j_drivers);
		return boost::json::serialize(j_result);
	}

	bool DbConnector::Connect(wstring driver, wstring server, int port, wstring db, wstring login, wstring password) {
		last_error_.clear();
		last_warning_.clear();

		if (!is_connect_) {
			wstring conn_str = wstring(L"Driver={")
				.append(driver).append(L"};")
				.append(L"Server=").append(server).append(L",").append(to_wstring(port)).append(L";")
				.append(L"Database=").append(db).append(L";")
				.append(L"UID=").append(login).append(L";")
				.append(L"PWD=").append(password).append(L";");
			wstring out_conn_str(1024, '\0');

			SQLRETURN sql_ret = SQLDriverConnectW(
				conn_,
				NULL,
				&conn_str[0],
				static_cast<SQLSMALLINT>(conn_str.size()),
				&out_conn_str[0],
				static_cast<SQLSMALLINT>(out_conn_str.size()),
				NULL,
				SQL_DRIVER_NOPROMPT);

			if (!(SQL_SUCCESS == sql_ret || SQL_SUCCESS_WITH_INFO == sql_ret)) {
				last_error_ = GetLastErrorSql(SQL_HANDLE_STMT, stmt_);
				is_connect_ = false;
			}
			else {
				is_connect_ = true;
			}
		}
		else {
			last_error_ = L"Соединение активно. Для нового подключения необходимо его закрыть.\n";
			last_error_.append(Utf8ToWideChar(EXCEPTION));
			throw std::runtime_error(WideCharToUtf8(last_error_));
			return false;
		}
		return is_connect_;
	}
	
	void DbConnector::CloseConnect() {
		if (stmt_) {
			SQLFreeHandle(SQL_HANDLE_STMT, stmt_);
			stmt_ = nullptr;
		}
		if (is_connect_) {
			SQLDisconnect(conn_);
			is_connect_ = false;
		}
		if (conn_) {
			SQLFreeHandle(SQL_HANDLE_DBC, conn_);
			conn_ = nullptr;
		}
		if (env_) {
			SQLFreeHandle(SQL_HANDLE_ENV, env_);
			env_ = nullptr;
		}
	}

	boost::json::object DbConnector::ExecAsObject(wstring& cmd, const wstring& hash_columns, int timeout) {
		last_error_.clear();
		last_warning_.clear();
		boost::json::object j_result;

		SQLRETURN sql_ret = SQLAllocHandle(SQL_HANDLE_STMT, conn_, &stmt_);
		if (SQL_SUCCESS != sql_ret) {
			last_error_ = GetLastErrorSql(SQL_HANDLE_STMT, stmt_);
			j_result.emplace("success", false);
			j_result.emplace("error", WideCharToUtf8(last_error_));
			return j_result;
		}

		if (timeout) {
			SQLSetStmtAttr(stmt_, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)timeout, 0);
		}

		if (!ExecQuery(cmd)) {
			last_error_ = GetLastErrorSql(SQL_HANDLE_STMT, stmt_);
			SQLFreeStmt(stmt_, SQL_CLOSE);
			SQLFreeStmt(stmt_, SQL_UNBIND);
			SQLFreeStmt(stmt_, SQL_RESET_PARAMS);
			j_result.emplace("success", false);
			j_result.emplace("error", WideCharToUtf8(last_error_));
			return j_result;
		}

		DbColumns db_columns;
		if (!db_columns.ReadColumns(stmt_, hash_columns)) {
			last_error_ = GetLastErrorSql(SQL_HANDLE_STMT, stmt_);
			SQLFreeStmt(stmt_, SQL_CLOSE);
			SQLFreeStmt(stmt_, SQL_UNBIND);
			SQLFreeStmt(stmt_, SQL_RESET_PARAMS);
			j_result.emplace("success", false);
			j_result.emplace("error", WideCharToUtf8(last_error_));
			return j_result;
		}

#ifdef _DEBUG
		std::wcout << db_columns << endl;
#endif // _DEBUG

		boost::json::object j_object;
		boost::json::array j_columns;

		for (auto it = db_columns.begin(); it < db_columns.end(); ++it) {
			boost::json::object j_column;
			j_columns.emplace_back(WideCharToUtf8(it->Name()));
			if (it->CalculateHash()) {
				j_columns.emplace_back(WideCharToUtf8(it->Name().append(L"_normalized")));
				j_columns.emplace_back(WideCharToUtf8(it->Name().append(L"_hash")));
			}
		}
		j_object.emplace("columns", j_columns);

		DbReader db_reader(stmt_, db_columns);
		if (db_reader.BindBufferAvailable() && !db_reader.BindBuffer()) {
			last_error_ = GetLastErrorSql(SQL_HANDLE_STMT, stmt_);
			SQLFreeStmt(stmt_, SQL_CLOSE);
			SQLFreeStmt(stmt_, SQL_UNBIND);
			SQLFreeStmt(stmt_, SQL_RESET_PARAMS);
			j_result.emplace("success", false);
			j_result.emplace("error", WideCharToUtf8(last_error_));
			return j_result;
		};

		stringstream ss;
		boost::json::array j_rows;
		while (db_reader.Next()) {
			boost::json::array j_row;
			vector<DbValue>& row = db_reader.Row();
			SQLUSMALLINT index_column = 0;
			bool add_row = true;
			for (auto it = row.begin(); it < row.end(); ++it) {
				switch (it->ValueType())
				{
				case DbValueType::Number:
					if (it->IsShortInt()) {
						auto value = it->AsShortInt();
						if (value) {
							j_row.emplace_back(*value);
						}
						else {
							j_row.emplace_back(boost::json::value());
						}
					}
					else if (it->IsInt()) {
						auto value = it->AsInt();
						if (value) {
							j_row.emplace_back(*value);
						}
						else {
							j_row.emplace_back(boost::json::value());
						}
					}
					else if (it->IsInt64()) {
						auto value = it->AsInt64();
						if (value) {
							j_row.emplace_back(*value);
						}
						else {
							j_row.emplace_back(boost::json::value());
						}
					}
					else if (it->IsFloat()) {
						auto value = it->AsFloat();
						if (value) {
							j_row.emplace_back(*value);
						}
						else {
							j_row.emplace_back(boost::json::value());
						}
					}
					else if (it->IsBit()) {
						auto value = it->AsBit();
						if (value) {
							j_row.emplace_back(*value);
						}
						else {
							j_row.emplace_back(boost::json::value());
						}
					}
					break;
				case DbValueType::String: {
					auto value = it->AsString();
					if (value) {
						j_row.emplace_back(*value);
						if (db_columns[index_column].CalculateHash()) {
							string sql_hash = SqlHashDbMsSql(&*value);
							string sql_hash_sha256 = GetSHA256(sql_hash, ss);
							j_row.emplace_back(sql_hash);
							j_row.emplace_back(sql_hash_sha256);
							if (add_row && !db_columns[index_column].FilterHash().empty() && db_columns[index_column].FilterHash() != sql_hash_sha256) {
								add_row = false;
							}
						}
					}
					else {
						j_row.emplace_back(boost::json::value());
						if (db_columns[index_column].CalculateHash()) {
							j_row.emplace_back(boost::json::value());
							j_row.emplace_back(boost::json::value());
							if (add_row && !db_columns[index_column].FilterHash().empty()) {
								add_row = false;
							}
						}
					}
				}
										break;
				case DbValueType::Datetime: {
					auto value = it->AsString();
					if (value) {
						j_row.emplace_back(*value);
					}
					else {
						j_row.emplace_back(boost::json::value());
					}
				}
										  break;
				case DbValueType::Binary: {
					auto value = it->AsString();
					if (value) {
						j_row.emplace_back(*value);
					}
					else {
						j_row.emplace_back(boost::json::value());
					}
				}
										break;
				case DbValueType::Guid: {
					auto value = it->AsString();
					if (value) {
						j_row.emplace_back(*value);
					}
					else {
						j_row.emplace_back(boost::json::value());
					}
				}
									  break;
				case DbValueType::Xml: {
					auto value = it->AsString();
					if (value) {
						j_row.emplace_back(*value);
					}
					else {
						j_row.emplace_back(boost::json::value());
					}
				}
									 break;
				default:
					j_row.emplace_back("unknow value type");
					break;
				}
				++index_column;
			}
			if (add_row) {
				j_rows.emplace_back(j_row);
			}
		}

		j_object.emplace("rows", j_rows);

		SQLFreeStmt(stmt_, SQL_CLOSE);
		SQLFreeStmt(stmt_, SQL_UNBIND);
		SQLFreeStmt(stmt_, SQL_RESET_PARAMS);

		j_result.emplace("success", true);
		if (!last_warning_.empty()) {
			j_object.emplace("warning", WideCharToUtf8(last_warning_));
		}
		j_result.emplace("data", j_object);
		return j_result;
	}

	string DbConnector::Exec(wstring cmd, const wstring& hash_columns, int timeout) {
		boost::json::object j_result = ExecAsObject(cmd, hash_columns, timeout);
		return boost::json::serialize(j_result);
	}

	string DbConnector::QueryHash(std::wstring query_text) {
		boost::json::object j_object;
		string w_query_text = WideCharToUtf8(query_text);
		string normalized_query_text = SqlHashDbMsSql(&w_query_text);
		j_object.emplace("normalized_query_text", normalized_query_text);
		j_object.emplace("query_hash", GetSHA256(normalized_query_text));
		return boost::json::serialize(j_object);
	}

	//private
	bool DbConnector::ExecQuery(wstring& cmd) {
		SQLRETURN sql_ret = SQLExecDirectW(stmt_, &cmd[0], static_cast<SQLINTEGER>(cmd.size()));
		if (SQL_ERROR == sql_ret) {
			return false;
		}
		else if (SQL_SUCCESS_WITH_INFO == sql_ret) {
			last_warning_ = GetLastErrorSql(SQL_HANDLE_STMT, stmt_);
		}
		return true;
	}

}