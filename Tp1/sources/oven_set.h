//
// Created by debbie on 12/05/15.
//

#ifndef CONCURRENTES_OVEN_H
#define CONCURRENTES_OVEN_H

#include <string>
#include <list>
#include <vector>
#include <unistd.h>

#include "semaphore.h"
#include "shared_memory.h"
#include "writer_fifo.h"
#include "lock_file.h"
#include "locknames.h"
#include "pipenames.h"


class OvenSet {
private:
    Semaphore ovens_sem;
    std::vector<Shared_Memory<std::string*>*> ovens;
    std::list<int> free_ovens;
    std::list<int> ready_ovens;

    Semaphore free_ovens_semaphore;
    Semaphore occupied_ovens_semaphore;
    Lock_File finished_fifo_lock;  // Creo que no había que usarlo acá, lo agregué por las dudas. - Bruno
    WriterFifo finished_fifo;

public:
    OvenSet(int ovens_number, Semaphore &free_ovens_sem, Semaphore &occupied_ovens_sem);
    ~OvenSet();
    void start_ovens();
    void cook(std::string pizza, float time);
    std::string remove();
    void close_ovens();
};


#endif //CONCURRENTES_OVEN_H
