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
