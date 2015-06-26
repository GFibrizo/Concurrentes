#ifndef CONCUSQL_DATABASE_H
#define CONCUSQL_DATABASE_H

#include <string>
#include <map>
#include "database_record.h"

using std::map;
using std::string;

class Database {
private:
    std::string filename;
    std::map<std::string, DatabaseRecord> data_map;

private:
    void fill_map();

public:
    Database(std::string filename);

    void persist();

    void store_record(DatabaseRecord record);

    DatabaseRecord get_record(std::string name);
};


#endif //CONCUSQL_DATABASE_H
