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

    int read_queue(const int type, T *buffer) const;

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
int MessageQueue<T>::read_queue(const int type, T *buffer) const {
    return msgrcv(this->id, static_cast<void *>(buffer), sizeof(T) - sizeof(long), type, 0);
}

#endif //QUEUE_H
