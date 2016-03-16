//
// Copyright 2015
// Bruno Merlo Schurmann bruno290@gmail.com
// Gaston Martinez gaston.martinez.90@gmail.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses
//

#ifndef CONCUSQL_DATABASE_H
#define CONCUSQL_DATABASE_H

#include <string>
#include <map>
#include "database_record.h"

using std::map;
using std::string;

class Database {
private:
    std::string filename;
    std::map<std::string, DatabaseRecord> data_map;

private:
    void fill_map();

public:
    Database(std::string filename);

    void persist();

    void store_record(DatabaseRecord record);

    DatabaseRecord get_record(std::string name);
};


#endif //CONCUSQL_DATABASE_H
