/**
 call_center.cpp

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

#include "call_center.h"

#include <cstdlib>
#include <sys/wait.h>
#include "logger.h"

using std::string;

Call_Center::Call_Center(Semaphore &recepcionists_semaphore, Pipe pipe) : recepcionist(recepcionists_semaphore),
                                                                          internal_pipe(pipe) {
}

void Call_Center::simulate_call(string request) {
    int pid = fork();
    if (pid == 0) {
        //TODO: do something
        sleep(10);
#ifdef __DEBUG__
		Logger::log(__FILE__,Logger::INFO,"Pedido: "+request);
#endif
        exit(EXIT_SUCCESS);
    } else {
        return;
    }
}

void Call_Center::accept_call(string request) {
    launched_process++;
    recepcionist.p();
    simulate_call(request);
    recepcionist.v();
}

void Call_Center::accept_calls() {
    static const int BUFFSIZE = 200;
    char buff[BUFFSIZE];
    char len_buff[sizeof(int)];
    while (internal_pipe.read_pipe(len_buff, sizeof(int)) > 0) {
        int *len = (int *) len_buff;
        if (*len == 0) {
            break;
        }

        if (internal_pipe.read_pipe(buff, *len) == 0) {
            //TODO: Error
        }

        buff[*len] = '\0'; //Agrega fin de linea donde va

        string request = buff;
        Logger::log(__FILE__, Logger::INFO, "Salio del pipe:" + request);
        accept_call(request);
    }

    for (size_t i = 0; i < launched_process; i++) { //FIXME: Por esto es que los semaforos no funcionan para esto.
        // FIXME O en realidad hay que tirar un wait por proceso lanzado me parece independientemente de como lo hagamos
        wait(0); //Waits for all recepcionist to finish
    }
}

Call_Center::~Call_Center() {
}

