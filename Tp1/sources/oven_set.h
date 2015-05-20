//
// Created by debbie on 12/05/15.
//

#ifndef CONCURRENTES_OVEN_H
#define CONCURRENTES_OVEN_H

#include <string>
#include <list>
#include <map>
#include <unistd.h>

#include "semaphore.h"
#include "shared_memory.h"
#include "writer_fifo.h"
#include "lock_file.h"
#include "locknames.h"
#include "pipenames.h"

#define INVALID_PIZZA -1

class OvenSet {
private:
    Semaphore& free_ovens_semaphore;
    Semaphore& occupied_ovens_semaphore;

    Lock_File finished_fifo_lock;  //TODO: Creo que no había que usarlo acá, lo agregué por las dudas. - Bruno
    WriterFifo finished_fifo;

    const int ovens_number;

public:
    OvenSet(int ovens_number, Semaphore &free_ovens_sem, Semaphore &occupied_ovens_sem);

    ~OvenSet();

    void start_ovens();

    void ask_use_permission();

    void cook(int oven_number, float time);

    void remove(int oven_number);

    void turn_off_ovens();

    void close_ovens();

    int get_ovens_number();
};


#endif //CONCURRENTES_OVEN_H
