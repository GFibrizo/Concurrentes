//
// Created by debbie on 12/05/15.
//

#include <stdlib.h>
#include "oven_set.h"
#include "shared_memory_names.h"
#include "logger.h"

using std::string;

OvenSet::OvenSet(int ovens_number, Semaphore &free_ovens_sem, Semaphore &occupied_ovens_sem) :
        ovens_sem("ovens", ovens_number),
        ready_ovens(),
        free_ovens_semaphore(free_ovens_sem),
        occupied_ovens_semaphore(occupied_ovens_sem),
        finished_fifo_lock(FINISHED_FIFO_LOCK),
        finished_fifo(FINISHED_FIFO) {
    for (int i = 0; i < ovens_number; i++) {
        ovens.push_back(new Shared_Memory<string *>(OVENS_SM, i));
        free_ovens.push_back(i);
    }
}

OvenSet::~OvenSet() {
}

void OvenSet::start_ovens() {
    finished_fifo.open_fifo();
}

void OvenSet::close_ovens() {
    int end = -1;
    finished_fifo.write_fifo(static_cast<void *>(&end), sizeof(int));
    finished_fifo_lock.lock(); //Espero a que terminen de usar la fifo las chicas del delivery
    finished_fifo.close_fifo();
    finished_fifo.remove();
    finished_fifo_lock.release();

    ovens.clear();

}

void OvenSet::cook(string pizza, float time) {
    free_ovens_semaphore.p();
    occupied_ovens_semaphore.v();

    int n_oven = free_ovens.front();
    string *new_pizza = new string(pizza);
    ovens[n_oven]->write(new_pizza);
    free_ovens.pop_front();

    int pid = fork();
    if (pid == 0) {
        sleep(time);
        ready_ovens.push_back(n_oven);
        finished_fifo.write_fifo(static_cast<void *>(&n_oven), sizeof(int));
#ifdef __DEBUG__
        Logger::log(__FILE__,Logger::DEBUG,"Coccion finalizada: "+pizza+" en horno: "+std::to_string(n_oven));
#endif
        exit(EXIT_SUCCESS);
    }
}

string OvenSet::remove() {
    while (ready_ovens.empty()) {
        sleep(1);
    }
    free_ovens_semaphore.v();

    int n_oven = ready_ovens.front();

    string *pizza = ovens[n_oven]->read();
    string pizza_copy = *pizza;
    delete pizza;

    ready_ovens.pop_front();
    free_ovens.push_back(n_oven);

#ifdef __DEBUG__
        Logger::log(__FILE__,Logger::DEBUG,"Pizza retirada: "+pizza_copy);
#endif

    occupied_ovens_semaphore.p();
    return pizza_copy;
}


