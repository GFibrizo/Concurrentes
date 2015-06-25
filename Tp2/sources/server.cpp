#include <iostream>
#include <fstream>
#include "message.h"
#include "queue.h"

#include "constants.h"

using std::cout;
using std::endl;
using std::remove;


int main() {

    std::ofstream temporal(SERVER_TEMPORAL);

    MessageQueue<message_t> m = MessageQueue<message_t>(SERVER_TEMPORAL, 'X');
    message_t request;

    for (int i = 0; i < 2; i++) {
        m.read_queue(1, &request);

        cout << "PID: " << request.sender_id << " Envio: " << request.name << endl;
    }

    temporal.close();
    remove(SERVER_TEMPORAL);

    return 0;
}