#include "database_record.h"

DatabaseRecord::DatabaseRecord(std::string name, std::string address, std::string phone_number) {
    this->name = name;
    this->address = address;
    this->phone_number = phone_number;
}
