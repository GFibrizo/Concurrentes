//
// Created by debbie on 11/05/15.
//

#include "supervisor.h"
#include <unistd.h>

Supervisor::Supervisor(Semaphore& cash_register_sem, Cash_Register& cash_reg) :
        cash_register_mutex(cash_register_sem), cash(cash_reg) {
}

Supervisor::~Supervisor() {
}

void Supervisor::check_cash_register() {
    while (true) {
        cash_register_mutex.p();
        if (cash.empty())
            //TODO: cambiar a lo que sea que haya que chequear
        cash_register_mutex.v();
        //TODO: cortar ejecucion
        sleep(2);
    }
}
