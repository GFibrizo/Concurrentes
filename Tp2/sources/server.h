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

#ifndef SERVER_H
#define SERVER_H

#include "queue.h"
#include "message.h"
#include "database.h"
#include "event_handler.h"
#include "signal_handler.h"
#include "logger.h"

class Server : public EventHandler {
private:
    MessageQueue<message_t> *queue;
    Database *database;

    message_t current_request;
    message_t current_response;

public:
    Server();

    ~Server();

    int process_next_request();

    void stop();

private:

    int get_request();

    int handle_request(DatabaseRecord &record);

    int send_response(DatabaseRecord &record, int request_status);

    int handle_get(DatabaseRecord &record);

    int handle_create(DatabaseRecord &record);

    int handle_update(DatabaseRecord &record);

    int handle_signal(int signal_number);
};


#endif //SERVER_H
