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
        free_ovens_semaphore(free_ovens_sem),
        occupied_ovens_semaphore(occupied_ovens_sem),
        finished_fifo_lock(FINISHED_FIFO_LOCK),
        finished_fifo(FINISHED_FIFO),
        ovens_number(ovens_number) {
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

void OvenSet::ask_use_permission(){
    free_ovens_semaphore.p();
    occupied_ovens_semaphore.v();
}

void OvenSet::cook(int n_oven, float time) {
    int pid = fork();
    if (pid == 0) {
        finished_fifo.open_fifo();
        sleep(time);
        finished_fifo.write_fifo(static_cast<void *>(&n_oven), sizeof(int));
#ifdef __DEBUG__
        Logger::log(__FILE__,Logger::DEBUG,"Coccion finalizada en horno: "+std::to_string(n_oven));
#endif
        finished_fifo.close_fifo(); //Cierro el fifo
        exit(EXIT_SUCCESS);
    }
}

void OvenSet::turn_off_ovens(){
    //TODO: free shared memory
}

void OvenSet::remove(int oven_number) {
#ifdef __DEBUG__
        Logger::log(__FILE__,Logger::DEBUG,"Pizza retirada de horno: "+oven_number);
#endif

    free_ovens_semaphore.v();
    occupied_ovens_semaphore.p();
}

int OvenSet::get_ovens_number() {
    return ovens_number;
}