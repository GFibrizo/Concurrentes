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
        std::getline(database, phone_number, ',');

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
    std::ofstream database(DATABASE_FILE);

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
