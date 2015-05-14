/**
 concu_zappi.cpp

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

#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdlib.h>

#include "call_center.h"
#include "logger.h"
#include "kitchen.h"
#include "locknames.h"
#include "delivery.h"
#include "pipenames.h"
#include "cash_register.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::map;

/*Funcion para cargar la configuracion del programa en un map*/
void load_configurations(map<string, int> &config) {
    ifstream file;
    file.open("config.cfg", std::fstream::in);
#ifdef __DEBUG__
	Logger::log(__FILE__, Logger::DEBUG, "Configuracion inicial");
#endif

    while (file.good()) {
        int amount = 0;
        string concept;
        file >> amount >> concept;
        if (config.count(concept) == 0) {
            continue;
        }
        config[concept] = amount;

#ifdef __DEBUG__
		stringstream output;
		output << concept << " cantidad: " << amount;
		Logger::log(__FILE__, Logger::INFO, output.str());
#endif
    }
}

void initialize_configurations(map<string, int> &config) {
    config["Recepcionistas"] = 0;
    config["Cocineras"] = 0;
    config["Cadetas"] = 0;
    config["Hornos"] = 0;
}

/*Funcion que devuelve si todos los campos fueron correctamente configurados*/
bool is_full_configured(map<string, int> config) {
    for (std::map<string, int>::iterator it = config.begin();
         it != config.end(); ++it) {
        if (it->second <= 0) {
            stringstream output;
            output << it->first
            << " no tiene un valor asignado o es menor o igual a 0 ";
            Logger::log(__FILE__, Logger::ERROR, output.str());
            Logger::log(__FILE__, Logger::ERROR, "Configuracion fallida");
            return false;
        }
    }
    return true;
}

int launch_call_center(Semaphore &recepcionists,Semaphore &max_requests_semaphore, Pipe &pipe) {

    int pid = fork();
    if (pid == 0) {
        Call_Center center = Call_Center(recepcionists,max_requests_semaphore, pipe);
        center.accept_calls();
        exit(EXIT_SUCCESS);
    }
    return pid;
}

int launch_chefs(Semaphore &chefs, Semaphore& max_requests_semaphore) {

    int pid = fork();
    if (pid == 0) {
        Kitchen kitchen = Kitchen(chefs,max_requests_semaphore);
        kitchen.accept_orders();
        exit(EXIT_SUCCESS);
    }
    return pid;
}

int launch_delivery(Semaphore &cadets, Semaphore &occupied_ovens_semaphore, Cash_Register &cash_register) {
    int pid = fork();
    if (pid == 0) {
        Delivery delivery = Delivery(cadets, occupied_ovens_semaphore, cash_register);
        delivery.start_deliveries();
        exit(EXIT_SUCCESS);
    }
    return pid;
}

void answer_calls(Pipe &pipe,Semaphore& max_requests_semaphore) {
    string line;
    cout << "Pedido: ";
    while (getline(cin, line)) {
        int size = line.size();
        ssize_t wrote = pipe.write_pipe(static_cast<const void *>(&size), sizeof(int));

        if (wrote <= 0) {
            break; //TODO: Ver bien que hacer en este caso.
        }

        wrote = pipe.write_pipe(static_cast<const void *>(line.c_str()), size);

        if (wrote == line.size()) {
#ifdef __DEBUG__
	Logger::log(__FILE__, Logger::DEBUG, "Pedido atendido: " + line);
#endif
        } else {
            cout << ">>Telefono ocupado<<" << endl;
#ifdef __DEBUG__
	Logger::log(__FILE__, Logger::DEBUG,
			"Pedido rechazado: " + line + " [Telefono ocupado]");
#endif
        }
        cout << "Pedido: ";
    }

    int size = 0;
    pipe.write_pipe(static_cast<const void *>(&size), sizeof(int));

    cout << "Fin recepcion de pedidos" << endl;

    Logger::log(__FILE__, Logger::INFO, "Cerrada recepcion de pedidos");

    Lock_File pipe_lock = Lock_File(REQUEST_PIPE_LOCK);

    pipe_lock.lock();
    pipe.close_pipe();
    pipe_lock.release();
}

int main(int argc, char **argv) {
    Logger::open_logger("run_log.log"); //TODO: Agregar opccion para sobreescribir

    Logger::log(__FILE__, Logger::INFO, "Inicio configuracion");
    map<string, int> config;

    initialize_configurations(config);
    load_configurations(config);

    if (not is_full_configured(config)) {
        Logger::log(__FILE__, Logger::ERROR, "Configuracion fallida");
        Logger::close_logger();
        return 1;
    }
    Logger::log(__FILE__, Logger::INFO, "Configuracion exitosa");


    Semaphore recepcionists_semaphore = Semaphore("Recepcionist", config["Recepcionistas"]);
    Semaphore chefs_semaphore = Semaphore("Chefs", config["Cocineras"]);
    Semaphore max_requests_semaphore = Semaphore("Max_Requests", config["Cocineras"]*2);
    Semaphore cadets_semaphore = Semaphore("Cadets", config["Cadetas"]);
    Semaphore free_ovens_semaphore = Semaphore("Free Ovens", config["Hornos"]);  // Cocina -> Hornos
    Semaphore occupied_ovens_semaphore = Semaphore("Occupied Ovens", 0);  // Hornos -> Delivery

    Pipe pipe = Pipe();
    int call_center_pid = launch_call_center(recepcionists_semaphore,max_requests_semaphore, pipe);
    int kitchen_pid = launch_chefs(chefs_semaphore,max_requests_semaphore);
    int delivery_pid = 0;
    try {
        Cash_Register cash_register = Cash_Register();
        delivery_pid = launch_delivery(cadets_semaphore, occupied_ovens_semaphore, cash_register);
    }catch (std::string e){
        cout <<  e << endl;
        exit(EXIT_FAILURE);
    }
    //FIXME: Sacarlo cuando esten los hornos
    WriterFifo fifo_hornos = WriterFifo(FINISHED_FIFO);
    fifo_hornos.open_fifo();
    ////////////////////////////////////////

    Logger::log(__FILE__, Logger::INFO, "Inicia recepcion de pedidos");
    answer_calls(pipe,max_requests_semaphore);

    waitpid(call_center_pid, 0, 0);  // espera que termine call_center
    waitpid(kitchen_pid, 0, 0);  // espera que termine kitchen
    kill(delivery_pid, SIGINT);  //TODO: manejo de errores?
    waitpid(delivery_pid, 0, 0);  // espera que termine delivery
    fifo_hornos.close_fifo();  //FIXME: Sacarlo cuando esten los hornos
    fifo_hornos.remove();  //FIXME: Sacarlo cuando esten los hornos, o ponerlo en delivery

    //TODO: Ver bien donde ponerlo
    recepcionists_semaphore.remove();
    chefs_semaphore.remove();
    cadets_semaphore.remove();
    free_ovens_semaphore.remove();
    occupied_ovens_semaphore.remove();

    Logger::close_logger();
    return 0;
}
