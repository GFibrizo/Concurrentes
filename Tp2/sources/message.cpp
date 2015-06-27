//
// Created by hige on 27/06/15.
//

#include <string.h>
#include "message.h"

using std::string;

void message_fill(string name,
                  string address,
                  string phone_number, message_t *message) {

    name.resize(NAME_SIZE);
    address.resize(ADDRESS_SIZE);
    phone_number.resize(PHONE_SIZE);

    strncpy(message->name, name.c_str(), name.size() + 1);
    strncpy(message->address, address.c_str(), address.size() + 1);
    strncpy(message->phone_number, phone_number.c_str(), phone_number.size() + 1);
}