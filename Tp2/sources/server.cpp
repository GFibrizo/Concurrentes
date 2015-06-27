#include <iostream>
#include <fstream>
#include "message.h"
#include "queue.h"
#include "database.h"
#include "server.h"

using std::cout;
using std::endl;
using std::remove;


Server::Server() {
    database = new Database(string(DATABASE_FILE));

    //Creates the temporal file
    std::ofstream temporal(SERVER_TEMPORAL);
    temporal.close();

    //Creates queue
    queue = new MessageQueue<message_t>(SERVER_TEMPORAL, 'X');
}

Server::~Server() {
    //Free queue
    queue->free_queue();
    delete queue;

    //Remove temporal connection file
    remove(SERVER_TEMPORAL);

    database->persist();
    delete database;
}

void Server::get_request() {
    message_t request;

    queue->read_queue(SERVER_ID, &request);

//#ifdef __DEBUG__
    cout << "PID: " << request.sender_id << " Envio: " << request.name << endl;
//#endif



    DatabaseRecord record = DatabaseRecord(request.name, request.address, request.phone_number);
    database->store_record(record);
}
