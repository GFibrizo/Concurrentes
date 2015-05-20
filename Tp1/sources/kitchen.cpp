/**
 kitchen.cpp

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

#include <cstdlib>
#include <sys/wait.h>

#include "kitchen.h"
#include "logger.h"
#include "times.h"
#include "shared_memory_names.h"

using std::string;
using std::to_string;

float generate_cooking_time() {
    static bool seeded = false;
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    return OVEN_MIN_TIME + (rand() / (RAND_MAX / (OVEN_MAX_TIME - OVEN_MIN_TIME)));
}

void Kitchen::simulate_cook(int order) {
    int pid = fork();
    if (pid == 0) {
        //TODO: do something

#ifdef __DEBUG__
		Logger::log(__FILE__,Logger::DEBUG,"Pedido levantado, amasando: "+to_string(order));
#endif
        sleep(COOKING_TIME);

        int total_ovens = ovens.get_ovens_number();
        Shared_Memory<int> ovens_memory(OVENS_SM, 'b', total_ovens);
        ovens.ask_use_permission();
        int oven_number = 0;
        while (ovens_memory.read() >= 0 && oven_number < total_ovens) {
            oven_number++;
        }
        ovens_memory.write(order);
        ovens.cook(oven_number, generate_cooking_time()); //FIXME reveer cuando esten los hornos

#ifdef __DEBUG__
		Logger::log(__FILE__,Logger::DEBUG,"Al horno: "+to_string(order));
#endif
        chefs.v(); //TODO: ver en que orden se deberia liberar esto
        exit(EXIT_SUCCESS);
    }
}


void Kitchen::accept_order(int order) {
    launched_process++;
    chefs.p();
    simulate_cook(order);
}

void Kitchen::accept_orders() {
    int order;
    while (requests_fifo.read_fifo(static_cast<void*>(&order), sizeof(int)) > 0) {
        max_requests.v();
        if (order == 0) {
            break;
        }

        accept_order(order);
    }

    requests_fifo.close_fifo();
    request_fifo_lock.release();


    for (size_t i = 0; i < launched_process; i++) {
        wait(0); //Waits for all chefs to finish
    }

#ifdef __DEBUG__
		Logger::log(__FILE__,Logger::DEBUG,"Amasados todos los pedidos");
#endif

}

Kitchen::Kitchen(Semaphore &chefs_semaphore, Semaphore &max_requests_semaphore, OvenSet &ovenSet) :
        chefs(chefs_semaphore), request_fifo_lock(REQUEST_FIFO_LOCK),
        requests_fifo(REQUEST_PIPE), max_requests(max_requests_semaphore),
        ovens(ovenSet) {
    request_fifo_lock.lock();
    requests_fifo.open_fifo();
    launched_process = 0;
}
