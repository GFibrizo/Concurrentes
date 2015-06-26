#include "constants.h"
#include <string>

#ifndef CONCUSQL_DATABASE_RECORD_H
#define CONCUSQL_DATABASE_RECORD_H

class DatabaseRecord {

private:
    std::string name;
    std::string address;
    std::string phone_number;

public:
    DatabaseRecord();

    DatabaseRecord(std::string name, std::string address, std::string phone_number);
};


#endif //CONCUSQL_DATABASE_RECORD_H
