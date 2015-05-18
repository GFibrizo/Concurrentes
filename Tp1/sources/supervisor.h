//
// Created by debbie on 11/05/15.
//

#ifndef REGISTER_SUPERVISOR_H
#define REGISTER_SUPERVISOR_H

#include "lock_file.h"
#include "shared_memory.h"
#include "cash_register.h"

class Supervisor {

private:
    Lock_File register_lock;
    Shared_Memory<float> cash_register;
    float checking_interval;

public:
    Supervisor(Shared_Memory<float> &cash_register, float checking_interval);

    ~Supervisor();

    void start_checking_register();
};


#endif /* REGISTER_SUPERVISOR_H */
