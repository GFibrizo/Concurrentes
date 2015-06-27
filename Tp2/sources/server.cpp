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
    DatabaseRecord record = DatabaseRecord(request.name, request.address, request.phone_number);

//#ifdef __DEBUG__
    cout << "PID: " << request.sender_id << " Envio: " << request.name << endl; //TODO: Usar el logger
//#endif

    handle_request(request.message_type, record);
    send_response(request.sender_id, record, 0);
}

void Server::stop() {
    queue->free_queue();

    //Remove temporal connection file
    remove(SERVER_TEMPORAL);
}

void Server::handle_request(int request_type, DatabaseRecord &record) {
    switch (request_type) {
        case 1:
        case 2:
        case 3:
        default:
            break;
    }
}

void Server::send_response(long receiver_id, DatabaseRecord &record, int status) {
    message_t response;

    response.sender_id = SERVER_ID;
    response.message_type = status;
    response.receiver_id = receiver_id;

    message_fill_record(record.name, record.address, record.phone_number, &response);

    this->queue->write_queue(response);
}
