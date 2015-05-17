//
// Created by debbie on 12/05/15.
//

#include <stdlib.h>
#include <sys/wait.h>

#include "oven_set.h"
#include "shared_memory_names.h"
#include "logger.h"

using std::string;

OvenSet::OvenSet(int ovens_number, Semaphore &free_ovens_sem, Semaphore &occupied_ovens_sem) :
        ovens_sem("ovens", ovens_number),
        free_ovens_semaphore(free_ovens_sem),
        occupied_ovens_semaphore(occupied_ovens_sem),
        finished_fifo_lock(FINISHED_FIFO_LOCK),
        finished_fifo(FINISHED_FIFO) {
    for (int i = 0; i < ovens_number; i++) {
        free_ovens.push_back(i);
    }
}

OvenSet::~OvenSet() {
}

void OvenSet::start_ovens() {
    finished_fifo.open_fifo();
}

void OvenSet::close_ovens() {
    //TODO ver: occupied_ovens_semaphore.w(); //wait for all ovens to finish
    //int end = -1;
    //finished_fifo.write_fifo(static_cast<void *>(&end), sizeof(int));
    finished_fifo_lock.lock(); //Espero a que terminen de usar la fifo las chicas del delivery
    finished_fifo.close_fifo();
    finished_fifo.remove();

    wait(0);

    finished_fifo_lock.release();
}

void OvenSet::cook(string pizza, float time) {
    free_ovens_semaphore.p();
    occupied_ovens_semaphore.v();

    int n_oven = free_ovens.front();
    free_ovens.pop_front();

    ovens[n_oven] = pizza;

    int pid = fork();
    if (pid == 0) {
        finished_fifo.open_fifo();
        sleep(time);
        finished_fifo.write_fifo(static_cast<void *>(&n_oven), sizeof(int));
#ifdef __DEBUG__
        Logger::log(__FILE__,Logger::DEBUG,"Coccion finalizada: "+pizza+" en horno: "+std::to_string(n_oven));
#endif
        finished_fifo.close_fifo(); //Cierro el fifo
        exit(EXIT_SUCCESS);
    }
}

string OvenSet::remove(int n_oven) {
    free_ovens_semaphore.v();

    string pizza(ovens[n_oven]);
    free_ovens.push_back(n_oven);

#ifdef __DEBUG__
        Logger::log(__FILE__,Logger::DEBUG,"Pizza retirada: "+pizza);
#endif

    occupied_ovens_semaphore.p();
    return pizza;
}


