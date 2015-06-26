#ifndef CONCUSQL_DATABASE_H
#define CONCUSQL_DATABASE_H

#include <string>
#include <map>
#include "database_record.h"

using std::map;
using std::string;

class Database {

    Database(std::string filename);

    std::string filename;
    std::map<std::string, DatabaseRecord> data_map;

private:
    void fill_map();
};


#endif //CONCUSQL_DATABASE_H
