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
#include "database.h"

class Server {
private:
    MessageQueue<message_t>* queue;
    Database *database;

public:
    Server();

    ~Server();

private:
    void get_request();

    int process_request();

    int send_response();

    int process_query();

    int process_add();

    int process_update();

};


#endif //SERVER_H
