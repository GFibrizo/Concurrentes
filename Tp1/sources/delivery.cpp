/**
 delivery.cpp

 Copyright 2015 Gaston Martinez Gaston.martinez.90@gmail.com

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

#include "delivery.h"

#include <cstdlib>
#include <sys/wait.h>

#include "logger.h"
#include "pipenames.h"
#include "locknames.h"

using std::string;
using std::to_string;

Delivery::Delivery(Semaphore &cadets_semaphore) : cadets(cadets_semaphore),
                                                  finished_fifo_lock(FINISHED_FIFO_LOCK),
                                                  finished_fifo(FINISHED_FIFO) {
    finished_fifo_lock.lock();
    finished_fifo.open_fifo();
    launched_process = 0;
}

void Delivery::make_delivery(int oven_number) {
    launched_process++;
    cadets.p();
    simulate_delivery(oven_number);
}

void Delivery::simulate_delivery(int oven_number) {
    int pid = fork();
    if (pid == 0) {
        //TODO: Ir al horno y sacar la pizza
        sleep(2);  //FIXME
        string pizza = "test pizza";  //FIXME
#ifdef __DEBUG__
	    Logger::log(__FILE__,Logger::DEBUG,"Sacada del horno: "+pizza);
#endif
        //TODO: Tiempo de entrega y monto random
        int deliver_time = 2;
        int payment = 50;
#ifdef __DEBUG__
	    Logger::log(__FILE__,Logger::DEBUG,"Se entrego "+pizza+". Tiempo: "+to_string(deliver_time)+". Pago: "+to_string(payment));
#endif
        //TODO: Dejar pago en la caja
#ifdef __DEBUG__
	    Logger::log(__FILE__,Logger::DEBUG,"Se deja en la caja: "+to_string(payment));
#endif
        cadets.v();
        exit(EXIT_SUCCESS);
    }
}

void Delivery::start_deliveries() {
    char buffer[sizeof(int)];
    while (true) {
        if (finished_fifo.read_fifo(buffer, sizeof(int))) {
            //TODO: error
        }
        int oven_number = *(int*)buffer;
        if (oven_number == -1) {
            break;
        }

        make_delivery(oven_number);
    }

    finished_fifo.close_fifo();
    finished_fifo_lock.release();


    for (size_t i = 0; i < launched_process; i++) {
        wait(0); // Espera que terminen todas las entregas
    }
}