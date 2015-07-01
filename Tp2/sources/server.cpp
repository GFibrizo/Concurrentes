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

    SignalHandler::get_instance()->register_handler(SIGINT, this);
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
    if (current_request.sender_id != 0) {
        cout << "PID: " << current_request.sender_id << " Envio: " << current_request.name <<
        endl; //TODO: Usar el logger
    }
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
    message_clean_fields(&current_response);
    if (get_request() == -1) {
        return -1;
    }
    DatabaseRecord record = DatabaseRecord(current_request.name, current_request.address, current_request.phone_number);
    int request_status = handle_request(record);
    int ret = send_response(record, request_status);
    message_clean_fields(&current_request);
    return ret;
}

void Server::stop() {
    queue->free_queue();

    //Remove temporal connection file
    remove(SERVER_TEMPORAL.c_str());
}

int Server::handle_signal(int signal_number) {
    if (signal_number == SIGINT) {
#ifdef __DEBUG__
    //TODO Usar logger
    //Logger::log(__FILE__, Logger::DEBUG, "Deteniendo servidor");
    cout << "Deteniendo server" << endl;
#endif
        // Bloqueo de SIGINT
        sigset_t blocking_set;
        sigemptyset(&blocking_set);
        sigaddset(&blocking_set, SIGINT);
        sigprocmask(SIG_BLOCK, &blocking_set, NULL);
        // Graceful Quit
        stop();
#ifdef __DEBUG__
    //TODO Usar logger
    //Logger::log(__FILE__, Logger::DEBUG, "Servidor detenido");
#endif
        return 0;
    }
    return -1;
}