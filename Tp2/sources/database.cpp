#include <fstream>
#include "database.h"

void Database::fill_map() {

    std::ifstream database(DATABASE_FILE);
    string name;
    string address;
    string phone_number;

    while (std::getline(database, name, ',')) {
        std::getline(database, address, ',');
        std::getline(database, phone_number, ',');

        DatabaseRecord record = DatabaseRecord(name, address, phone_number);

        this->data_map[name] = record;
    }

}

Database::Database(std::string filename) {
    this->filename = filename;
    this->data_map = std::map<std::string, DatabaseRecord>();

    this->fill_map();
}
