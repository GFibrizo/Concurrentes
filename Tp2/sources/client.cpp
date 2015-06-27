#include <unistd.h>
#include <iostream>
#include "queue.h"
#include "message.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;

int main(void) {
    MessageQueue<message_t> m = MessageQueue<message_t>(SERVER_TEMPORAL, 'X');

    message_t request;
    request.receiver_id = SERVER_ID; //Al server
    request.sender_id = getpid();

    char name_buffer[NAME_SIZE];

    cin.getline(name_buffer, NAME_SIZE);
    string name = string(name_buffer);

    string address = "Calle de los gatos 123";
    string phone = "22-25-24";

    message_fill_record(name, address, phone, &request);

    m.write_queue(request);

    return 0;
}
