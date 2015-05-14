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
    std::vector<Shared_Memory<std::string>*> ovens;
    std::list<std::string> ready_pizzas;

    Semaphore occupied_ovens;
    Lock_File finished_fifo_lock;  // Creo que no había que usarlo acá, lo agregué por las dudas. - Bruno
    WriterFifo finished_fifo;

public:
    OvenSet(int ovens_number, Semaphore &occupied_ovens_semaphore);  //TODO: Agregar lo que haga falta para la cocina
    ~OvenSet();
    void start_ovens();
    void cook(std::string pizza, float time);
    std::string remove();
    void close_ovens();
};


#endif //CONCURRENTES_OVEN_H
