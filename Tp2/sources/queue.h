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

#ifndef QUEUE_H
#define QUEUE_H

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string>

template<class T>
class MessageQueue {
private:
    key_t key;
    int id;

public:
    MessageQueue(const std::string &filename, const char salt);

    ~MessageQueue();

    int write_queue(const T &data) const;

    ssize_t read_queue(const int type, T *buffer) const;

    int free_queue() const;
};

template<class T>
MessageQueue<T>::MessageQueue(const std::string &filename, const char salt) {
    this->key = ftok(filename.c_str(), salt);
    if (this->key == -1)
        perror("Error en ftok");

    this->id = msgget(this->key, 0777 | IPC_CREAT);
    if (this->id == -1)
        perror("Error en msgget");
}

template<class T>
MessageQueue<T>::~MessageQueue() { }

template<class T>
int MessageQueue<T>::free_queue() const {
    return msgctl(this->id, IPC_RMID, NULL);
}

template<class T>
int MessageQueue<T>::write_queue(const T &data) const {
    return msgsnd(this->id, static_cast<const void *>(&data), sizeof(T) - sizeof(long), 0);
}

template<class T>
ssize_t MessageQueue<T>::read_queue(const int type, T *buffer) const {
    return msgrcv(this->id, static_cast<void *>(buffer), sizeof(T) - sizeof(long), type, 0);
}

#endif //QUEUE_H
