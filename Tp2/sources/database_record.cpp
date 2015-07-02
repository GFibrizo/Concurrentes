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

#include "database_record.h"

DatabaseRecord::DatabaseRecord(std::string name, std::string address, std::string phone_number) {
    this->name = name;
    this->address = address;
    this->phone_number = phone_number;
}

DatabaseRecord::DatabaseRecord(const DatabaseRecord &other) {
    this->name = other.name;
    this->address = other.address;
    this->phone_number = other.phone_number;
}

DatabaseRecord::DatabaseRecord() {
    this->name = "";
    this->address = "";
    this->phone_number = "";
}

void DatabaseRecord::set_record(std::string name, std::string address, std::string phone_number) {
    this->name = name;
    this->address = address;
    this->phone_number = phone_number;
}
