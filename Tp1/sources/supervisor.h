//
// Created by debbie on 11/05/15.
//

#ifndef CONCURRENTES_SUPERVISOR_H
#define CONCURRENTES_SUPERVISOR_H

#include "semaphore.h"
#include "cash_register.h"
#include "lock_file.h"


class Supervisor {

private:
    Lock_File register_lock;
    Cash_Register cash_register;

public:
    Supervisor(Semaphore& cash_register_sem, Cash_Register&cash_register);
    ~Supervisor();
    void check_cash_register();
};


#endif //CONCURRENTES_SUPERVISOR_H
