#ifndef MESSAGE_H
#define MESSAGE_H

#define SERVER_ID   1

#define NAME_SIZE 60
#define ADDRESS_SIZE 120
#define PHONE_SIZE 13

typedef struct message {
    long receiver_id; //mtype
    long sender_id;
    int message_type;
    bool success;  // Podriamos hacer que el record que indique error en lugar de esto (tipo, el nombre vacio). No se que va mejor

    //record
    char name[NAME_SIZE];
    char address[ADDRESS_SIZE];
    char phone_number[PHONE_SIZE];

} message_t;

#endif //MESSAGE_H
