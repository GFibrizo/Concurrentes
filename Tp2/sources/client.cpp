#include <unistd.h>
#include <iostream>
#include <fstream>

#include "queue.h"
#include "message.h"
#include "client.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;

bool file_exists(const string &filename) {
    return std::ifstream(filename.c_str()).good();  // El archivo se cierra automaticamente cuando se destruye el ifstream
}

Client::Client() {
    queue = NULL;
}

Client::~Client() {
    free_queue();
}

bool Client::connect() {
    if (file_exists(SERVER_TMP_FILE)) {
        queue = new MessageQueue<message_t>(SERVER_TMP_FILE, 'X');
        return true;
    }
    return false;
}

bool Client::connected() {
    return queue != NULL;
}

void Client::disconnect() {
    free_queue();
}

int Client::make_request(int type, DatabaseRecord &record) {
    request.receiver_id = SERVER_ID;
    request.sender_id = getpid();
    request.message_type = type;
    message_fill_record(record.name, record.address, record.phone_number, &request);

    if (queue->write_queue(request) == -1) {
        // Error
        free_queue();
        return SERVER_ERROR;
    }
    if (queue->read_queue(getpid(), &request) == -1) {
        // Error
        free_queue();
        return SERVER_ERROR;
    }

    return request.message_type;
}

int Client::request_create(DatabaseRecord &record) {
    return make_request(CREATE_RECORD, record);
}

int Client::request_update(DatabaseRecord &record) {
    return make_request(UPDATE_RECORD, record);
}

int Client::request_retrieve(DatabaseRecord &record) {
    int ret = make_request(UPDATE_RECORD, record);
    record.set_record(request.name, request.address, request.phone_number);
    return ret;
}

void Client::free_queue() {
    queue->free_queue();
    delete queue;
    queue = NULL;  //FIXME no me acuerdo si delete lo setea a NULL o simplemente lo deja como sea
}
