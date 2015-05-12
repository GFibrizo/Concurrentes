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


class OvenSet {
private:
    Semaphore ovens_sem;
    std::vector<Shared_Memory<std::string>*> ovens;
    std::list<std::string> ready_pizzas;

public:
    OvenSet(int ovens_number);
    ~OvenSet();
    void cook(std::string pizza, float time);
    std::string remove();
};


#endif //CONCURRENTES_OVEN_H
