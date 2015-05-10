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

using std::string;

void Kitchen::simulate_cook(std::string pizza) {
    int pid = fork();
    if (pid == 0) {
        //TODO: do something
        sleep(2);
#ifdef __DEBUG__
		Logger::log(__FILE__,Logger::DEBUG,"Cocinando: "+pizza);
#endif
        chefs.v(); //TODO: ver en que orden se deberia liberar esto
        exit(EXIT_SUCCESS);
    }
}


void Kitchen::accept_order(std::string pizza) {
    launched_process++;
    chefs.p();
    simulate_cook(pizza);
}

void Kitchen::acept_orders() {
    static const int BUFFSIZE = 200;
    char buff[BUFFSIZE];
    char len_buff[sizeof(int)];
    while (fifo.read_fifo(len_buff, sizeof(int)) > 0) {
        int *len = (int *) len_buff;
        if (*len == 0) {
            break;
        }

        if (fifo.read_fifo(buff, *len) == 0) {
            //TODO: Error
        }

        buff[*len] = '\0'; //Agrega fin de linea donde va

        string pizza = buff;
        accept_order(pizza);
    }

    fifo.close_fifo();

    for (size_t i = 0; i < launched_process; i++) {
        wait(0); //Waits for all chefs to finish
    }
}

Kitchen::Kitchen(Semaphore &chefs_semaphore) : chefs(chefs_semaphore), fifo("/tmp/PedidosAceptados") {
    fifo.open_fifo();
}
