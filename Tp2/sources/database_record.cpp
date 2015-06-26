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
