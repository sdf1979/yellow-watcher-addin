/*
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

#ifndef YELLOWWATCHERADDIN_H
#define YELLOWWATCHERADDIN_H

#include "Component.h"
#include "DbConnector.h"

class YellowWatcherAddIn final : public Component {
public:
    const char *Version = u8"1.0.5";

    YellowWatcherAddIn();

private:
    std::string extensionName() override;
    variant_t сreateConnection();
    variant_t connect(const variant_t& driver, const variant_t& server, const variant_t& port,
    const variant_t& db, const variant_t& user, const variant_t& password);
    variant_t executeAndCalculateHash(const variant_t& query, const variant_t& column);
    variant_t execute(const variant_t& query);
    variant_t queryHash(const variant_t& query);
    variant_t drivers();
    variant_t getLastError();
    void closeConnect();

    std::shared_ptr<variant_t> sample_property;
    Soldy::DbConnector db_connecor;
};

#endif //YELLOWWATCHERADDIN_H
