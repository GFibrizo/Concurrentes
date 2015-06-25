#ifndef RECORD_H
#define RECORD_H

#define NAME_SIZE 60
#define ADDRESS_SIZE 120
#define PHONE_SIZE 13

typedef struct record {
    char name[NAME_SIZE];
    char address[ADDRESS_SIZE];
    char phone_number[PHONE_SIZE];
} record_t;

#endif //RECORD_H
