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

#include <sys/wait.h>
#include "logger.h"

using std::string;

Call_Center::Call_Center(size_t recepcionists, Pipe &pipe) : internal_pipe(pipe) {
    recepcionist = recepcionists;
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
    if (recepcionist > 0) {
        recepcionist--;
        simulate_call(request);
        return;
    }
    wait(0);
    simulate_call(request);
}

void Call_Center::accept_calls() {
    internal_pipe.set_mode(Pipe::READ);
    char *buff = new char[200];
    internal_pipe.read_pipe(buff, 200);
    //string str(buff);
    Logger::log(__FILE__, Logger::INFO, "Entro al pipe");
    //while(str != "-"){
    accept_call("MUZZA");
    //	internal_pipe.read_pipe(buff,200);
    //	string str(buff);
    //}
    Logger::log(__FILE__, Logger::INFO, "Salio del pipe");

}

Call_Center::~Call_Center() {
}

