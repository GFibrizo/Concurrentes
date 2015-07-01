//
// client
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
//

#ifndef CLIENT_H
#define CLIENT_H


#include "message.h"
#include "queue.h"
#include "database_record.h"

class Client {
private:
    MessageQueue<message_t> *queue;

    message_t request;

public:
    Client();

    bool connect();

    bool connected();

    void disconnect();

    int request_create(DatabaseRecord &record);

    int request_update(DatabaseRecord &record);

    int request_retrieve(DatabaseRecord &record);

    ~Client();

private:
    int make_request(int type, DatabaseRecord &record);

    void free_queue();
};


#endif //CLIENT_H
