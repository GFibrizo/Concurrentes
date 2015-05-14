//
// Created by debbie on 11/05/15.
//

#include "supervisor.h"

#include <unistd.h>
#include "locknames.h"
#include "logger.h"

Supervisor::Supervisor(Cash_Register& cash_register) :
        register_lock(CASH_REGISTER_LOCK), cash_register(cash_register), continue_checking(false) {
}

Supervisor::~Supervisor() {
}

void Supervisor::check_cash_register() {
    continue_checking = true;

    while (continue_checking) {
        register_lock.lock();
        if (cash_register.empty()) {
#ifdef __DEBUG__
            //Logger::log(__FILE__,Logger::DEBUG,"Caja registradora vacia");
#endif
        }
        register_lock.release();
        sleep(100);
    }
}

void Supervisor::stop() {
    continue_checking = false; //FIXME: algo mejor que esto
}
