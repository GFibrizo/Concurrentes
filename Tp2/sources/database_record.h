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
