/**
 Copyright 2015
 Debora Martin debbie1new.world@gmail.com
 Gaston Martinez gaston.martinez.90@gmail.com
 Bruno Merlo Schurmann bruno290@gmail.com

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses
*/

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
    register_lock.lock();
    int register_amount = cash_register.read();
    if (register_amount == 0) {
        Logger::log(__FILE__, Logger::INFO, "Cerrada la caja. La caja esta vacia");
    } else {
        Logger::log(__FILE__, Logger::INFO, "Cerrada la caja. Saldo: " + to_string(register_amount));
    }
    register_lock.release();
}
