#include <unistd.h>
#include <string.h>
#include "queue.h"
#include "message.h"

int main(void) {
    MessageQueue<message_t> m = MessageQueue<message_t>("server.cpp", 'X');

    message_t request;
    request.receiver_id = 1; //Al server
    request.sender_id = getpid();

    record_t record;

    strncpy(record.name, "Polez", 6);

    memcpy(&request.record, &record, sizeof(record_t));

    m.write_queue(request);

    return 0;
}
