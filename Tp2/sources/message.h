#include "constants.h"

#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct message {
    long receiver_id; //mtype
    long sender_id;
    int message_type;
    bool success;  // TODO: Podriamos hacer que el record que indique error en lugar de esto (tipo, el nombre vacio). No se que va mejor

    //record
    char name[NAME_SIZE];
    char address[ADDRESS_SIZE];
    char phone_number[PHONE_SIZE];

} message_t;

#endif //MESSAGE_H
