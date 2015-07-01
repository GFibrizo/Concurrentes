#include <iostream>
#include <fstream>
#include <sstream>

#include "server.h"

using std::cout;
using std::endl;
using std::remove;

template<typename T>
string to_string(T val) {
    std::stringstream stream;
    stream << val;
    return stream.str();
}

Server::Server() {
    Logger::open_logger("run_log.log");
    Logger::log(__FILE__, Logger::INFO, "Iniciando server");

    database = new Database(DATABASE_FILE);

    //Creates the temporal file
    std::ofstream temporal(SERVER_TMP_FILE.c_str());
    temporal.close();

    //Creates queue
    queue = new MessageQueue<message_t>(SERVER_TMP_FILE, 'X');

    SignalHandler::get_instance()->register_handler(SIGINT, this);

    Logger::log(__FILE__, Logger::INFO, "Server iniciado");
}

Server::~Server() {
    //Free queue
    queue->free_queue();
    delete queue;

    //Remove temporal connection file
    remove(SERVER_TMP_FILE.c_str());

    database->persist();
    delete database;
}

int Server::get_request() {
    int ret = queue->read_queue(SERVER_ID, &current_request);
#ifdef __DEBUG__
    if (current_request.sender_id != 0) {
        Logger::log(__FILE__, Logger::DEBUG, "Recibido request del cliente con PID: " + to_string(current_request.sender_id) + ". Peticion: " + current_request.name);
    }
#endif
    return ret;
}

int Server::handle_request(DatabaseRecord &record) {
    int request_type = current_request.message_type;
#ifdef __DEBUG__
    if (current_request.sender_id != 0) {
        Logger::log(__FILE__, Logger::DEBUG, "Procesando request del cliente con PID: " + to_string(current_request.sender_id) + ". Tipo de peticion: " + to_string(current_request.message_type));
    }
#endif
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

#ifdef __DEBUG__
    if (current_response.receiver_id != 0) {
        Logger::log(__FILE__, Logger::DEBUG, "Enviando respuesta al cliente con PID: " + to_string(current_request.sender_id) + ". Estado de la peticion: " + to_string(request_status));
    }
#endif
    return queue->write_queue(current_response);
}

int Server::handle_get(DatabaseRecord &record) {
    DatabaseRecord new_record = database->get_record(record.name);

    if (new_record.name == "") {
#ifdef __DEBUG__
        if (current_request.sender_id != 0) {
            Logger::log(__FILE__, Logger::DEBUG, "Retrieve del cliente con PID " + to_string(current_request.sender_id)
            + "fallo: no existe un registro con nombre " + record.name);
        }
#endif
        return REQUEST_ERROR;
    }

    record.phone_number = new_record.phone_number;
    record.address = new_record.address;
#ifdef __DEBUG__
    if (current_request.sender_id != 0) {
        Logger::log(__FILE__, Logger::DEBUG, "Retrieve del cliente con PID " + to_string(current_request.sender_id)
        + "exitoso: enviando el registro con nombre " + record.name);
    }
#endif
    return REQUEST_SUCCESS;
}

int Server::handle_create(DatabaseRecord &record) {
    DatabaseRecord new_record = database->get_record(record.name);

    if (new_record.name != "") {
#ifdef __DEBUG__
        if (current_request.sender_id != 0) {
            Logger::log(__FILE__, Logger::DEBUG, "Create del cliente con PID " + to_string(current_request.sender_id)
            + "fallo: ya existe un registro con nombre " + record.name);
        }
#endif
        return REQUEST_ERROR;
    }

    database->store_record(record);
#ifdef __DEBUG__
    if (current_request.sender_id != 0) {
        Logger::log(__FILE__, Logger::DEBUG, "Create del cliente con PID " + to_string(current_request.sender_id)
        + "exitoso: agregando registro con nombre " + record.name);
    }
#endif
    return REQUEST_SUCCESS;
}

int Server::handle_update(DatabaseRecord &record) {
    DatabaseRecord new_record = database->get_record(record.name);

    if (new_record.name == "") {
#ifdef __DEBUG__
        if (current_request.sender_id != 0) {
            Logger::log(__FILE__, Logger::DEBUG, "Update del cliente con PID " + to_string(current_request.sender_id)
            + "fallo: no existe un registro con nombre " + record.name);
        }
#endif
        return REQUEST_ERROR;
    }

    database->store_record(record);
#ifdef __DEBUG__
    if (current_request.sender_id != 0) {
        Logger::log(__FILE__, Logger::DEBUG, "Update del cliente con PID " + to_string(current_request.sender_id)
        + "exitoso: modificado el registro con nombre " + record.name);
    }
#endif
    return REQUEST_SUCCESS;
}

int Server::process_next_request() {
#ifdef __DEBUG__
    Logger::log(__FILE__, Logger::DEBUG, "Esperando una nueva request");
#endif
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
    remove(SERVER_TMP_FILE.c_str());
}

int Server::handle_signal(int signal_number) {
    if (signal_number == SIGINT) {
        Logger::log(__FILE__, Logger::INFO, "Deteniendo servidor");
        // Bloqueo de SIGINT
        sigset_t blocking_set;
        sigemptyset(&blocking_set);
        sigaddset(&blocking_set, SIGINT);
        sigprocmask(SIG_BLOCK, &blocking_set, NULL);
        // Graceful Quit
        stop();
        Logger::log(__FILE__, Logger::INFO, "Servidor detenido");
        return 0;
    }
    return -1;
}