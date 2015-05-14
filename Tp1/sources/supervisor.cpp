//
// Created by debbie on 11/05/15.
//

#include "supervisor.h"

#include <unistd.h>
#include "locknames.h"

Supervisor::Supervisor(Semaphore& cash_register_sem, Cash_Register& cash_register) :
        register_lock(CASH_REGISTER_LOCK), cash_register(cash_register) {
}

Supervisor::~Supervisor() {
}

void Supervisor::check_cash_register() {
    while (true) {
        register_lock.lock();
        if (cash_register.empty()) {
#ifdef __DEBUG__
            Logger::log(__FILE__,Logger::INFO,"Caja registradora vacia");
#endif
        }
        register_lock.release();
        //TODO: cortar ejecucion
        sleep(2);
    }
}
