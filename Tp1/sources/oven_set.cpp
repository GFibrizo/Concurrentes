//
// Created by debbie on 12/05/15.
//

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
        ovens.push_back(new Shared_Memory<string*>(OVENS_SM, i));
        free_ovens.push_back(i);
    }
}

OvenSet::~OvenSet() {
}

void OvenSet::start_ovens() {
    //finished_fifo_lock.lock();
    finished_fifo.open_fifo();
}

void OvenSet::close_ovens() {
    ovens.clear();
    finished_fifo.close_fifo();
    finished_fifo.remove();
}

void OvenSet::cook(string pizza, float time) {
    free_ovens_semaphore.p();

    int n_oven = free_ovens.front();
    string* new_pizza = new string(pizza);
    ovens[n_oven]->write(new_pizza);
    free_ovens.pop_front();

    int pid = fork();
    if (pid == 0) {
        sleep(time);
        ready_ovens.push_back(n_oven);
        finished_fifo.write_fifo(static_cast<void *>(&n_oven), sizeof(int));
        exit(EXIT_SUCCESS);
    }
}

string OvenSet::remove() {
    occupied_ovens_semaphore.p();
    while (ready_ovens.empty()) {
        sleep(1);
    }
    free_ovens_semaphore.v();

    int n_oven = ready_ovens.front();

    string* pizza = ovens[n_oven]->read();
    string pizza_copy = *pizza;
    delete pizza;

    ready_ovens.pop_front();
    free_ovens.push_back(n_oven);

    Logger::log(__FILE__, Logger::DEBUG, "termina cocinar: " + pizza_copy);
    occupied_ovens_semaphore.v();

    return pizza_copy;
}


