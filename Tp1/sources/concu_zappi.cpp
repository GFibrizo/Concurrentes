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

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::map;
using std::exit;

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
    config["Pedidos"] = 0;
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

void lanzar_cocineras() {
    return;
}

int launch_chefs(Semaphore &chefs) {

    Kitchen kitchen = Kitchen(chefs);
    int pid = fork();
    if (pid == 0) {
        kitchen.acept_orders();
        exit(EXIT_SUCCESS);
    }
    return pid;
}

int launch_call_center(Semaphore &recepcionists, Pipe &pipe) {

    Call_Center center = Call_Center(recepcionists, pipe);
    int pid = fork();
    if (pid == 0) {
        center.accept_calls();
        exit(EXIT_SUCCESS);
    }
    return pid;
}

void answer_calls(Pipe &pipe) {
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
	Logger::log(__FILE__, Logger::DEBUG, "Pedido aceptado: " + line);
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
    cout << "Fin recepcion de pedidos" << endl;
    Logger::log(__FILE__, Logger::INFO, "Cerrada recepcion de pedidos");
    pipe.close_pipe(); //TODO: ver esto cuando le pongamos el maximo a meter
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

    Semaphore chefs_semaphore = Semaphore("Recepcionist", config["Recepcionistas"]);
    int kitchen_pid = launch_chefs(chefs_semaphore);

    Pipe pipe = Pipe();
    Logger::log(__FILE__, Logger::INFO, "Inicia recepcion de pedidos");
    Semaphore recepcionists_semaphore = Semaphore("Recepcionist", config["Recepcionistas"]);
    int call_center_pid = launch_call_center(recepcionists_semaphore, pipe);

    answer_calls(pipe);
    waitpid(call_center_pid, 0, 0); //Wait call_center to finish

    waitpid(kitchen_pid, 0, 0); //Wait kitchen to finish

    Logger::close_logger();
    return 0;
}
