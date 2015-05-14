//
// Created by debbie on 12/05/15.
//

#include "oven_set.h"
#include "shared_memory_names.h"

using std::string;

OvenSet::OvenSet(int ovens_number, Semaphore &occupied_ovens_semaphore) : ovens_sem("ovens", ovens_number),
                                                                          ready_pizzas(),
                                                                          occupied_ovens(occupied_ovens_semaphore),
                                                                          finished_fifo_lock(FINISHED_FIFO_LOCK),
                                                                          finished_fifo(FINISHED_FIFO) {
    for (int i = 0; i < ovens_number; i++)
        ovens.push_back(new Shared_Memory<string>(OVENS_SM, i));
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
    int n_oven = ovens_sem.p();
    occupied_ovens.v();
    ovens[n_oven]->write(pizza);

    int pid = fork();
    if (pid == 0) {
        sleep(time);
        ready_pizzas.push_back(ovens[n_oven]->read());
        finished_fifo.write_fifo(static_cast<void *>(&n_oven), sizeof(int));
        exit(EXIT_SUCCESS);
    }
}

string OvenSet::remove() {
    while (ready_pizzas.empty()) {
        sleep(1);
    }
    string pizza = ready_pizzas.front();
    ready_pizzas.pop_front();
    ovens_sem.v();
    occupied_ovens.p();
    return pizza;
}


