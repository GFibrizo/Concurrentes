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

#include "shared_memory.h"
#include "call_center.h"
#include "logger.h"
#include "kitchen.h"
#include "locknames.h"
#include "delivery.h"
#include "supervisor.h"
#include "shared_memory_names.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::map;
using std::stoi;

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
    config["Intervalo_Supervisora"] = 0;
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

int launch_call_center(Semaphore &recepcionists, Semaphore &max_requests_semaphore, Pipe &pipe) {

    int pid = fork();
    if (pid == 0) {
        Call_Center center = Call_Center(recepcionists, max_requests_semaphore, pipe);
        center.accept_calls();
        exit(EXIT_SUCCESS);
    }
    return pid;
}

int launch_chefs(Semaphore &chefs, Semaphore &max_requests_semaphore, Shared_Memory<int> *ovens, Semaphore &free_ovens_semaphore, Semaphore &occupied_ovens_semaphore) {

    int pid = fork();
    if (pid == 0) {
        Kitchen kitchen = Kitchen(chefs, max_requests_semaphore, ovens,free_ovens_semaphore,occupied_ovens_semaphore);
        kitchen.accept_orders();
        exit(EXIT_SUCCESS);
    }
    return pid;
}

int launch_delivery(Semaphore &cadets, Shared_Memory<int> *ovens, Semaphore &occupied_ovens_semaphore,
                    Shared_Memory<float> &cash_register) {
    int pid = fork();
    if (pid == 0) {
        Delivery delivery = Delivery(cadets, ovens, occupied_ovens_semaphore, cash_register);
        delivery.start_deliveries();
        exit(EXIT_SUCCESS);
    }
    return pid;
}

int launch_supervisor(Shared_Memory<float> &cash_register, float checking_interval) {
    int pid = fork();
    if (pid == 0) {
        Supervisor supervisor = Supervisor(cash_register, checking_interval);
        supervisor.start_checking_register();
        exit(EXIT_SUCCESS);
    }
    return pid;
}

void initialize_ovens(Shared_Memory<int> *ovens, int ovens_size) {
    for (int i = 0; i < ovens_size; i++) {
        ovens[i].create(__FILE__, i);
        ovens[i].write(0);
    }
}

void answer_calls(Pipe &pipe) {
    string line;
    cout << "Pedido: ";
    while (getline(cin, line)) {
        int order = stoi(line);
        ssize_t wrote = pipe.write_pipe(static_cast<void *>(&order), sizeof(int));

        if (wrote <= 0) {
            break; //TODO: Ver bien que hacer en este caso.
        }

        if (wrote == sizeof(int)) {
#ifdef __DEBUG__
	Logger::log(__FILE__, Logger::DEBUG, "Llamado entrante: " + line);
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

int main() {
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
    Semaphore max_requests_semaphore = Semaphore("Max_Requests", config["Cocineras"] * 2);
    Semaphore cadets_semaphore = Semaphore("Cadets", config["Cadetas"]);
    Semaphore free_ovens_semaphore = Semaphore("Free Ovens", config["Hornos"]);  // Cocina -> Hornos
    Semaphore occupied_ovens_semaphore = Semaphore("Occupied Ovens", 0);  // Hornos -> Delivery

    Pipe pipe = Pipe();
    Shared_Memory<int> *ovens = new Shared_Memory<int>[config["Hornos"]];
    initialize_ovens(ovens, config["Hornos"]);

    Shared_Memory<float> cash_register = Shared_Memory<float>();

    int call_center_pid = launch_call_center(recepcionists_semaphore, max_requests_semaphore, pipe);
    int kitchen_pid = launch_chefs(chefs_semaphore, max_requests_semaphore, ovens,free_ovens_semaphore,occupied_ovens_semaphore);

    int delivery_pid = 0;
    int supervisor_pid = 0;
    try {
        //Cash_Register cash_register = Cash_Register();
        cash_register.create(CASH_REGISTER_SM, 'a');
        delivery_pid = launch_delivery(cadets_semaphore, ovens, occupied_ovens_semaphore, cash_register);
        supervisor_pid = launch_supervisor(cash_register, config["Intervalo_Supervisora"]);
    } catch (std::string e) {
        cout << e << endl;
        exit(EXIT_FAILURE);
    }

    Logger::log(__FILE__, Logger::INFO, "Inicia recepcion de pedidos");
    answer_calls(pipe);

    waitpid(call_center_pid, 0, 0);  // espera que termine call_center
#ifdef __DEBUG__
    Logger::log(__FILE__,Logger::DEBUG,"Cerrado call center. Todas las empleadas se retiraron");
#endif
    waitpid(kitchen_pid, 0, 0);  // espera que termine kitchen
#ifdef __DEBUG__
    Logger::log(__FILE__,Logger::DEBUG,"Cerrada cocina. Todos las cocineras se retiraron");
#endif
    //ovens.close_ovens();
    kill(delivery_pid, SIGINT);  // mata al delivery
    waitpid(delivery_pid, 0, 0);  // espera que termine delivery
#ifdef __DEBUG__
    Logger::log(__FILE__,Logger::DEBUG,"Cerrado el delivery. Todas las empleadas se retiraron");
#endif
    kill(supervisor_pid, SIGINT);  // mata al supervisor
    waitpid(supervisor_pid, 0, 0);  // espera al supervisor
#ifdef __DEBUG__
    Logger::log(__FILE__,Logger::DEBUG,"Cerrada oficina. La supervisora se ha retirado");
#endif

    //TODO: Ver bien donde ponerlo
    recepcionists_semaphore.remove();
    chefs_semaphore.remove();
    max_requests_semaphore.remove();
    cadets_semaphore.remove();
    free_ovens_semaphore.remove();
    occupied_ovens_semaphore.remove();

    cash_register.free();

    Logger::close_logger();
    return 0;
}
