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

#include "constants.h"
#include <string>
#include <string.h>
#include <bits/stringfwd.h>

#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct message {
    long receiver_id; //mtype
    long sender_id;

    int message_type;

    //bool success;   //TODO: Podriamos hacer que el record que indique error en lugar de esto (tipo, el nombre vacio). No se que va mejor
    //Se puede poner directamente en el message_type (comando)
    //record
    char name[NAME_SIZE];
    char address[ADDRESS_SIZE];
    char phone_number[PHONE_SIZE];

} message_t;

void message_fill_record(std::string name, std::string address, std::string phone_number, message_t *message);

void message_clean_fields(message_t *message);

#endif //MESSAGE_H
