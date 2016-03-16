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

#include <fstream>
#include "database.h"

using std::endl;

void Database::fill_map() {

    std::fstream database;
    database.open(this->filename.c_str(), std::fstream::in | std::fstream::app);
    string name;
    string address;
    string phone_number;

    while (std::getline(database, name, ',')) {
        std::getline(database, address, ',');
        std::getline(database, phone_number, '\n');

        DatabaseRecord record = DatabaseRecord(name, address, phone_number);

        this->data_map[name] = record;
    }

    database.close();
}

Database::Database(std::string filename) {
    this->filename = filename;
    this->data_map = std::map<std::string, DatabaseRecord>();

    this->fill_map();
}


void Database::persist() {
    std::ofstream database(DATABASE_FILE.c_str());

    for (std::map<string, DatabaseRecord>::iterator it = this->data_map.begin(); it != this->data_map.end(); ++it) {
        DatabaseRecord record = it->second;
        database << record.name << ',' << record.address << ',' << record.phone_number << endl;
    }

    database.close();
}


DatabaseRecord Database::get_record(string name) {

    if (this->data_map.count(name)) {
        return this->data_map[name];
    }

    return DatabaseRecord("", "", "");
}

void Database::store_record(DatabaseRecord record) {
    this->data_map[record.name] = record;
}
