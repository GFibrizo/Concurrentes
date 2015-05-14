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

    bool continue_checking;

public:
    Supervisor(Cash_Register&cash_register);
    ~Supervisor();
    void check_cash_register();
    void stop();
};


#endif //CONCURRENTES_SUPERVISOR_H
