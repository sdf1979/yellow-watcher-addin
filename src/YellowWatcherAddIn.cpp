﻿/*
 *  Modern Native AddIn
 *  Copyright (C) 2018  Infactum
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>

#include "YellowWatcherAddIn.h"

std::string YellowWatcherAddIn::extensionName() {
    return "YellowWatcher";
}

YellowWatcherAddIn::YellowWatcherAddIn() {
    // Universal property. Could store any supported by native api type.
    sample_property = std::make_shared<variant_t>();
    AddProperty(L"SampleProperty", L"ОбразецСвойства", sample_property);

    // Full featured property registration example
    AddProperty(L"Version", L"ВерсияКомпоненты", [&]() {
        auto s = std::string(Version);
        return std::make_shared<variant_t>(std::move(s));
    });

    AddMethod(L"CreateConnection", L"СоздатьСоединение", this, &YellowWatcherAddIn::сreateConnection);
    AddMethod(L"Connect", L"Соединиться", this, &YellowWatcherAddIn::connect);
    AddMethod(L"CloseConnection", L"ЗакрытьСоединение", this, &YellowWatcherAddIn::closeConnect);
    AddMethod(L"ExecuteAndCalculateHash", L"ВыполнитьИВычислитьХеш", this, &YellowWatcherAddIn::executeAndCalculateHash, { {2, 0} });
    AddMethod(L"Execute", L"Выполнить", this, &YellowWatcherAddIn::execute, { {1, 0} });
    AddMethod(L"QueryHash", L"ХешЗапроса", this, &YellowWatcherAddIn::queryHash);
    AddMethod(L"Drivers", L"Драйверы", this, &YellowWatcherAddIn::drivers);
    AddMethod(L"GetLastError", L"ПолучитьПоследнююОшибку", this, &YellowWatcherAddIn::getLastError);
    AddMethod(L"ParseExtentedEvent", L"АнализРасширенногоСобытия", this, &YellowWatcherAddIn::parseExtentedEvent);
}

variant_t YellowWatcherAddIn::сreateConnection() {
    return db_connecor.Create();
}

variant_t YellowWatcherAddIn::connect(const variant_t& driver, const variant_t& server, const variant_t& port,
    const variant_t& db, const variant_t& user, const variant_t& password) {
    return db_connecor.Connect(
        Soldy::Utf8ToWideChar(std::get<std::string>(driver)),
        Soldy::Utf8ToWideChar(std::get<std::string>(server)),
        std::get<int32_t>(port),
        Soldy::Utf8ToWideChar(std::get<std::string>(db)),
        Soldy::Utf8ToWideChar(std::get<std::string>(user)),
        Soldy::Utf8ToWideChar(std::get<std::string>(password))
        );
}

void YellowWatcherAddIn::closeConnect() {
    db_connecor.CloseConnect();
}

variant_t YellowWatcherAddIn::executeAndCalculateHash(const variant_t& query, const variant_t& column, const variant_t& timeout) {
    std::wstring w_query = Soldy::Utf8ToWideChar(std::get<std::string>(query));
    std::wstring w_column = Soldy::Utf8ToWideChar(std::get<std::string>(column));
    return db_connecor.Exec(w_query, w_column, std::get<int>(timeout));
}

variant_t YellowWatcherAddIn::execute(const variant_t& query, const variant_t& timeout) {
    std::wstring w_query = Soldy::Utf8ToWideChar(std::get<std::string>(query));
    return db_connecor.Exec(w_query, L"", std::get<int>(timeout));
}

variant_t YellowWatcherAddIn::queryHash(const variant_t& query) {
    std::wstring w_query = Soldy::Utf8ToWideChar(std::get<std::string>(query));
    return db_connecor.QueryHash(w_query);
}

variant_t YellowWatcherAddIn::drivers() {
    return db_connecor.GetDrivers();
}

variant_t YellowWatcherAddIn::getLastError() {
    return Soldy::WideCharToUtf8(db_connecor.GetLastError());
}

variant_t YellowWatcherAddIn::parseExtentedEvent(const variant_t& extented_event) {
    return Soldy::ParseExtentedEvent(std::get<std::string>(extented_event));
}
