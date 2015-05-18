//
// Created by debbie on 11/05/15.
//

#include "supervisor.h"

#include <unistd.h>
#include <iostream>
#include "locknames.h"
#include "signal_handler.h"
#include "SIGINT_handler.h"
#include "logger.h"

using std::cout;
using std::endl;
using std::to_string;

Supervisor::Supervisor(Shared_Memory<float> &cash_register, float checking_interval) :
        register_lock(CASH_REGISTER_LOCK), cash_register(cash_register), checking_interval(checking_interval) {
}

Supervisor::~Supervisor() {
}

void Supervisor::start_checking_register() {
    SIGINT_Handler sigint_handler = SIGINT_Handler();
    SignalHandler::get_instance()->register_handler(SIGINT, &sigint_handler);
    while (sigint_handler.get_graceful_quit() == 0) {
        register_lock.lock();
        int register_amount = cash_register.read();
        if (register_amount == 0) {
            Logger::log(__FILE__, Logger::INFO, "La caja esta vacia");
        } else {
            Logger::log(__FILE__, Logger::INFO, "Hay en la caja: " + to_string(register_amount));
        }
        register_lock.release();
        sleep(checking_interval);
    }
}
