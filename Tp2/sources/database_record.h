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

#include "constants.h"
#include <string>

#ifndef CONCUSQL_DATABASE_RECORD_H
#define CONCUSQL_DATABASE_RECORD_H

class DatabaseRecord {

public:
    std::string name;
    std::string address;
    std::string phone_number;

public:
    DatabaseRecord();

    DatabaseRecord(const DatabaseRecord &another);

    DatabaseRecord(std::string name, std::string address, std::string phone_number);

    void set_record(std::string name, std::string address, std::string phone_number);
};


#endif //CONCUSQL_DATABASE_RECORD_H
