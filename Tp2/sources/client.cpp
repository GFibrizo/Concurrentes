#include <unistd.h>
#include <string.h>
#include "queue.h"
#include "message.h"

#define SERVER_TEMPORAL "server.tmp"


int main(void) {
    MessageQueue<message_t> m = MessageQueue<message_t>(SERVER_TEMPORAL, 'X');

    message_t request;
    request.receiver_id = 1; //Al server
    request.sender_id = getpid();
    strncpy(request.name, "Polez", 6);

    m.write_queue(request);

    return 0;
}
