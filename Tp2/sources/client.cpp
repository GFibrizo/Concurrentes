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
    return std::ifstream(filename).good();  // El archivo se cierra automaticamente cuando se destruye el ifstream
}

int maein(void) {


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

int Client::request_create(const DatabaseRecord &record) {
    return make_request(CREATE_RECORD, record);
    if (request.message_type == REQUEST_SUCCESS) {
        cout << "Registro agregado exitosamente" << endl;
    } else {
        if (ret != -1) {
            cout << "Ya existe un registro con el nombre " << record.name << endl;
        } else {
            cout << "Error en la comunicacion con el servidor" << endl;
        }
    }
    if (ret == -1) {

    }
    return ret;
}

int Client::request_update(const DatabaseRecord &record) {
    return make_request(UPDATE_RECORD, record);
    if (request.message_type == REQUEST_SUCCESS) {
        cout << "Registro modificado exitosamente" << endl;
    } else {
        if (ret != -1) {
            cout << "No existe un registro con el nombre " << record.name << endl;
        } else {
            cout << "Error en la comunicacion con el servidor" << endl;
        }
    }
    return ret;
}

int Client::request_retrieve(DatabaseRecord &record) {
    return make_request(UPDATE_RECORD, record);
    if (request.message_type == REQUEST_SUCCESS) {
        cout << "Registro encontrado:" << endl;
        cout << "Nombre: " << request.name << endl;
        cout << "Direccion: " << request.address << endl;
        cout << "Telefono: " << request.phone_number << endl;
    } else {
        if (ret != -1) {
            cout << "No existe un registro con el nombre " << record.name << endl;
        } else {
            cout << "Error en la comunicacion con el servidor" << endl;
        }
    }
    return ret;
}

void Client::free_queue() {
    queue->free_queue();
    delete queue;
    queue = NULL;  //FIXME no me acuerdo si delete lo setea a NULL o simplemente lo deja como sea
}
