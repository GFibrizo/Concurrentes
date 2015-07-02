//
// Copyright 2015
// Bruno Merlo Schurmann bruno290@gmail.com
// Gaston Martinez gaston.martinez.90@gmail.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses
//

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "queue.h"
#include "message.h"
#include "client.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;

template<typename T>
string to_string(T val) {
    std::stringstream stream;
    stream << val;
    return stream.str();
}

bool file_exists(const string &filename) {
    return std::ifstream(filename.c_str()).good();  // El archivo se cierra automaticamente cuando se destruye el ifstream
}

Client::Client() {
    Logger::open_logger(CLIENT_LOG_FILE_HEAD + to_string(getpid()) + CLIENT_LOG_FILE_EXTENSION);
    Logger::log(__FILE__, Logger::INFO, "Iniciando cliente");

    queue = NULL;
    Logger::log(__FILE__, Logger::INFO, "Cliente iniciado");
}

Client::~Client() {
    Logger::log(__FILE__, Logger::INFO, "Cerrando cliente");
    disconnect();
    Logger::log(__FILE__, Logger::INFO, "Cliente cerrado");
}

bool Client::connect() {
    Logger::log(__FILE__, Logger::INFO, "Intentando conexion con el servidor");
    if (file_exists(SERVER_TMP_FILE)) {
        queue = new MessageQueue<message_t>(SERVER_TMP_FILE, 'X');
        Logger::log(__FILE__, Logger::INFO, "Conectado");
        return true;
    }
    Logger::log(__FILE__, Logger::INFO, "Error de conexion con el servidor");
    return false;
}

bool Client::connected() {
    return queue != NULL;
}

void Client::disconnect() {
    Logger::log(__FILE__, Logger::INFO, "Desconectandose del servidor");
    free_queue();
}

int Client::make_request(int type, DatabaseRecord &record) {
    request.receiver_id = SERVER_ID;
    request.sender_id = getpid();
    request.message_type = type;
    message_fill_record(record.name, record.address, record.phone_number, &request);

#ifdef __DEBUG__
    Logger::log(__FILE__, Logger::DEBUG, "Enviando request al servidor. Tipo: " + to_string(type) + " Registro nombre " + record.name);
#endif
    if (queue->write_queue(request) == -1) {
        // Error
        free_queue();
#ifdef __DEBUG__
    Logger::log(__FILE__, Logger::DEBUG, "Error al enviar request. Registro nombre " + record.name);
#endif
        return SERVER_ERROR;
    }
#ifdef __DEBUG__
    Logger::log(__FILE__, Logger::DEBUG, "Esperando respuesta del servidor. Registro nombre " + record.name);
#endif
    if (queue->read_queue(getpid(), &request) == -1) {
        // Error
        free_queue();
#ifdef __DEBUG__
    Logger::log(__FILE__, Logger::DEBUG, "Error recibiendo respuesta del servidor. Registro nombre " + record.name);
#endif
        return SERVER_ERROR;
    }

#ifdef __DEBUG__
    Logger::log(__FILE__, Logger::DEBUG, "Respuesta del servidor recibida: " + to_string(request.message_type) + ". Registro nombre " + record.name);
#endif
    return request.message_type;
}

int Client::request_create(DatabaseRecord &record) {
#ifdef __DEBUG__
    Logger::log(__FILE__, Logger::DEBUG, "Procesando request: Create. Registro nombre " + record.name);
#endif
    return make_request(CREATE_RECORD, record);
}

int Client::request_update(DatabaseRecord &record) {
#ifdef __DEBUG__
    Logger::log(__FILE__, Logger::DEBUG, "Procesando request: Update. Registro nombre " + record.name);
#endif
    return make_request(UPDATE_RECORD, record);
}

int Client::request_retrieve(DatabaseRecord &record) {
#ifdef __DEBUG__
    Logger::log(__FILE__, Logger::DEBUG, "Procesando request: Retrieve. Registro nombre " + record.name);
#endif
    int ret = make_request(RETRIVE_RECORD, record);
    record.set_record(request.name, request.address, request.phone_number);
    return ret;
}

void Client::free_queue() {
    if (queue != NULL) {
        delete queue;
        queue = NULL;  //FIXME no me acuerdo si delete lo setea a NULL o simplemente lo deja como sea
        Logger::log(__FILE__, Logger::INFO, "Desconectado del servidor, cola de mensajes eliminada");
    }
}
