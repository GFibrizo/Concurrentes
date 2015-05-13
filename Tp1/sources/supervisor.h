//
// Created by debbie on 11/05/15.
//

#ifndef CONCURRENTES_SUPERVISOR_H
#define CONCURRENTES_SUPERVISOR_H

#include "semaphore.h"
#include "cash_register.h"


class Supervisor {

private:
    Semaphore cash_register_mutex;
    Cash_Register  cash;

public:
    Supervisor(Semaphore& cash_register_sem, Cash_Register& cash_reg);
    ~Supervisor();
    void check_cash_register();
};


#endif //CONCURRENTES_SUPERVISOR_H