//
// server
//
// Copyright 2015 Bruno Merlo Schurmann bruno290@gmail.com 
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

#ifndef SERVER_H
#define SERVER_H


#include "queue.h"
#include "message.h"

class Server {
private:
    int connections_count = 0;
    MessageQueue<message_t>* queue;
    message_t current_request;
    message_t current_response;  // Esto por si hacemos en 2 pasos, procesar y enviar. Si el procesar envia, no hace falta

public:
    Server(const std::string &queue_filename);
    ~Server();
    int get_request();
    int process_request();
    int send_response();
private:
    int process_query();
    int process_add();
};


#endif //SERVER_H
