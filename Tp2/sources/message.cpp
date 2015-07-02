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

#include <string.h>
#include "message.h"

using std::string;

void message_fill_record(string name, string address, string phone_number, message_t *message) {

    name.resize(NAME_SIZE);
    address.resize(ADDRESS_SIZE);
    phone_number.resize(PHONE_SIZE);

    strncpy(message->name, name.c_str(), name.size() + 1);
    strncpy(message->address, address.c_str(), address.size() + 1);
    strncpy(message->phone_number, phone_number.c_str(), phone_number.size() + 1);
}

void message_clean_fields(message_t *message) {
    memset(message, 0, sizeof(message_t));
}