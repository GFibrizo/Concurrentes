#ifndef MESSAGE_H
#define MESSAGE_H

#include "record.h"

#define SERVER_ID   0

#define MSG_ADD     0
#define MSG_QUERY   1

typedef struct message {
    long receiver_id; //mtype
    long sender_id;
    int message_type;
    bool success;  // Podriamos hacer que el record que indique error en lugar de esto (tipo, el nombre vacio). No se que va mejor
    record_t record;
} message_t;

#endif //MESSAGE_H
