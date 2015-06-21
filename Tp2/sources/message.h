//
// Created by hige on 21/06/15.
//

#ifndef MESSAGE_H
#define MESSAGE_H

#include "record.h"

#define    REQUEST     1
#define    RESPONSE    2

#define TEXTO_SIZE    255


typedef struct message {
    long type;
    long id;
    recod_t record;
} message_t;

#endif //SOURCES_MESSAGE_H
