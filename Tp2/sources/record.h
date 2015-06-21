//
// Created by hige on 21/06/15.
//

#ifndef RECORD_H
#define RECORD_H

#define NAME_SIZE 61
#define ADRESS_SIZE 120
#define PHONE_SIZE 13

typedef struct record {
    char name[NAME_SIZE];
    char address[ADRESS_SIZE];
    char phone_number[PHONE_SIZE];
} recod_t;

#endif //RECORD_H
