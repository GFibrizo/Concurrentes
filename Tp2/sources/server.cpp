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
    database = new Database(DATABASE_FILE);

    //Creates the temporal file
    std::ofstream temporal(SERVER_TEMPORAL.c_str());
    temporal.close();

    //Creates queue
    queue = new MessageQueue<message_t>(SERVER_TEMPORAL, 'X');
}

Server::~Server() {
    //Free queue
    queue->free_queue();
    delete queue;

    //Remove temporal connection file
    remove(SERVER_TEMPORAL.c_str());

    database->persist();
    delete database;
}

int Server::get_request() {
    int ret = queue->read_queue(SERVER_ID, &current_request);

//#ifdef __DEBUG__
    cout << "PID: " << current_request.sender_id << " Envio: " << current_request.name << endl; //TODO: Usar el logger
//#endif

    return ret;
}

int Server::handle_request(DatabaseRecord &record) {
    int request_type = current_request.message_type;
    switch (request_type) {
        case CREATE_RECORD:
            return handle_create(record);
        case UPDATE_RECORD:
            return handle_update(record);
        case RETRIVE_RECORD:
            return handle_get(record);
        default:
            return -2;
    }
}

int Server::send_response(DatabaseRecord &record, int request_status) {
    current_response.sender_id = SERVER_ID;
    current_response.message_type = request_status;
    current_response.receiver_id = current_request.sender_id;

    message_fill_record(record.name, record.address, record.phone_number, &current_response);

    return queue->write_queue(current_response);
}

int Server::handle_get(DatabaseRecord &record) {
    DatabaseRecord new_record = database->get_record(record.name);

    if (new_record.name == "")
        return REQUEST_ERROR;

    record.phone_number = new_record.phone_number;
    record.address = new_record.address;
    return REQUEST_SUCCESS;
}

int Server::handle_create(DatabaseRecord &record) {
    DatabaseRecord new_record = database->get_record(record.name);

    if (new_record.name != "") //Already exists
        return REQUEST_ERROR;

    database->store_record(record);
    return REQUEST_SUCCESS;
}

int Server::handle_update(DatabaseRecord &record) {
    DatabaseRecord new_record = database->get_record(record.name);

    if (new_record.name == "") //Doesn't exist
        return REQUEST_ERROR;

    database->store_record(record);
    return REQUEST_SUCCESS;
}

int Server::process_next_request() {
    if (get_request() == -1) {
        return -1;
    }
    DatabaseRecord record = DatabaseRecord(current_request.name, current_request.address, current_request.phone_number);

    int request_status = handle_request(record);

    return send_response(record, request_status);
}

void Server::stop() {
    queue->free_queue();

    //Remove temporal connection file
    remove(SERVER_TEMPORAL.c_str());
}