#ifndef MESSAGE_H
#define MESSAGE_H

#include "record.h"

#define    REQUEST     1
#define    RESPONSE    2

#define TEXT_SIZE    255


typedef struct message {
    long type;
    long id;
    record_t record;
} message_t;

#endif //MESSAGE_H
