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
#include "logger.h"
#include "locknames.h"
#include "pipenames.h"

#include <cstdlib>
#include <sys/wait.h>

using std::string;

Call_Center::Call_Center(Semaphore &recepcionists_semaphore,Semaphore &max_requests_semaphore, Pipe& pipe) : recepcionist(recepcionists_semaphore),
                                                                          pipe_lock(REQUEST_PIPE_LOCK),
                                                                          internal_pipe(pipe),
                                                                          fifo_lock(REQUEST_FIFO_LOCK),
                                                                          fifo(REQUEST_PIPE),
                                                                          aviable_requests(max_requests_semaphore) {
    pipe_lock.lock();
    fifo.open_fifo();

}

void Call_Center::simulate_call(string request) {
    int pid = fork();
    if (pid == 0) {
        //TODO: do something
        sleep(2);
#ifdef __DEBUG__
		Logger::log(__FILE__,Logger::DEBUG,"Pedido cargado: "+request);
#endif
        int len = request.size();
        char *buffer = (char *) request.c_str();
        fifo.write_fifo(static_cast<void *>(&len), sizeof(int)); //TODO: esto me hace un poco de ruido
        fifo.write_fifo(static_cast<void *>(buffer), len);
        recepcionist.v(); //TODO: ver en que orden se deberia liberar esto
        exit(EXIT_SUCCESS);
    }
}

void Call_Center::accept_call(string request) {
    launched_process++;
    recepcionist.p();
    simulate_call(request);
}

void Call_Center::accept_calls() {
    static const int BUFFSIZE = 200;
    char buff[BUFFSIZE];
    char len_buff[sizeof(int)];
    while (internal_pipe.read_pipe(len_buff, sizeof(int)) > 0) {
        int len = *(int *) len_buff;
        if (len == 0) {
            break;
        }

        if (internal_pipe.read_pipe(buff, len) == 0) {
            //TODO: Error
        }

        buff[len] = '\0'; //Agrega fin de linea donde va

        string request = buff;

#ifdef __DEBUG__
        Logger::log(__FILE__,Logger::DEBUG,"Pedido Atendiendose: "+request);
#endif

        accept_call(request);

    }

    pipe_lock.release(); //El pipe ya esta vacio

    for (size_t i = 0; i < launched_process; i++) { //TODO: Por esto es que los semaforos no funcionan para esto.
        //TODO O en realidad hay que tirar un wait por proceso lanzado me parece independientemente de como lo hagamos
        wait(0); //Waits for all recepcionist to finish
    }

    int end = 0;
    fifo.write_fifo(static_cast<void *>(&end), sizeof(int));
#ifdef __DEBUG__
    Logger::log(__FILE__,Logger::DEBUG,"Atendidos todos los pedidos");
#endif

    fifo_lock.lock(); //Espero a que se vacie la cola
    fifo.close_fifo();
    fifo_lock.release();

    fifo.remove();
}

Call_Center::~Call_Center() {
}

