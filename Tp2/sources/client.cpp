#include <unistd.h>
#include <string.h>
#include "queue.h"
#include "message.h"

int main(void) {
    MessageQueue<message_t> m = MessageQueue<message_t>(SERVER_TEMPORAL, 'X');

    message_t request;
    request.receiver_id = SERVER_ID; //Al server
    request.sender_id = getpid();
    strncpy(request.name, "Polez", 6);

    m.write_queue(request);

    m.free_queue();

    return 0;
}
