#include <iostream>
#include "message.h"
#include "queue.h"

using std::cout;
using std::endl;

int main() {
    MessageQueue<message_t> m = MessageQueue<message_t>("server.cpp", 'X');
    message_t request;

    while (true) {
        m.read_queue(1, &request);

        cout << "PID: " << request.sender_id << " Envio: " << request.record.name << endl;
    }


    return 0;
}