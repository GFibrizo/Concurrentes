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
    request.message_type = RETRIVE_RECORD;

    char name_buffer[NAME_SIZE];

    cin.getline(name_buffer, NAME_SIZE);
    string name = string(name_buffer);

    message_fill_record(name, name, name, &request);

    m.write_queue(request);
    m.read_queue(getpid(), &request);


    if (request.message_type == REQUEST_ERROR) {
        cout << "No se pudo realizar la operacion" << endl;
    }
    else {
        cout << "Operacion: " << request.message_type << endl;
        cout << request.name << " , " << request.address << " , " << request.phone_number << endl;
    }
    return 0;
}
