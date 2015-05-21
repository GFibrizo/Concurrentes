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
#include "times.h"

#define  MIN_PAYMENT 50
#define  MAX_PAYMENT 100

using std::string;
using std::to_string;

float generate_deliver_time() {
    static bool seeded = false;
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    return DELIVERY_MIN_TIME + (rand() / (RAND_MAX / (DELIVERY_MAX_TIME - DELIVERY_MIN_TIME)));
}

int generate_payment_amount() {
    static bool seeded = false;
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    return MIN_PAYMENT + (rand() / (RAND_MAX / (MAX_PAYMENT - MIN_PAYMENT)));
}

Delivery::Delivery(Semaphore &cadets_semaphore, Shared_Memory<int> *ovens, Semaphore &free_ovens_semaphore,
                   Semaphore &occupied_ovens_semaphore, Shared_Memory<float> &cash_register)
        : cadets(cadets_semaphore),
          ovens(ovens),
          free_ovens(free_ovens_semaphore),
          occupied_ovens(occupied_ovens_semaphore),
          ovens_lock(OVEN_LOCK),
          finished_fifo_lock(FINISHED_FIFO_LOCK),
          finished_fifo(FINISHED_FIFO),
          cash_register_lock(CASH_REGISTER_LOCK),
          cash_register(cash_register) {
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
        ovens_lock.lock();
        int order = ovens[oven_number].read();
        ovens[oven_number].write(0);
        ovens_lock.release();
#ifdef __DEBUG__
	    Logger::log(__FILE__, Logger::DEBUG, "Sacada del horno "+to_string(oven_number)+": "+to_string(order));
#endif
        occupied_ovens.p();
        free_ovens.v();

        float deliver_time = generate_deliver_time();
        sleep(deliver_time);

        int payment = generate_payment_amount();
#ifdef __DEBUG__
	    Logger::log(__FILE__, Logger::DEBUG, "Se entrego "+to_string(order)+". Tiempo: "+to_string(deliver_time)+". Pago: "+to_string(payment));
#endif

        cash_register_lock.lock();
        cash_register.write(cash_register.read() + payment);
        cash_register_lock.release();
#ifdef __DEBUG__
	    Logger::log(__FILE__, Logger::DEBUG, "Se deja en la caja: "+to_string(payment));
#endif
        cadets.v();
        exit(EXIT_SUCCESS);
    }
}

void Delivery::start_deliveries() {
    DeliverySIGINTHandler sigint_handler(occupied_ovens, finished_fifo);
    SignalHandler::get_instance()->register_handler(SIGINT, &sigint_handler);

    int oven_number = 0;
    //char *buffer = (char *) &oven_number;
    while (finished_fifo.read_fifo(static_cast<void*>(&oven_number), sizeof(int)) > 0) {
        make_delivery(oven_number);

    }
    launched_process++;  // El hijo que hace occupied_ovens.w()

    finished_fifo.close_fifo();
    finished_fifo_lock.release();

    for (size_t i = 0; i < launched_process; i++) {
        wait(0); // Espera que terminen todas las entregas
    }
}

Delivery::DeliverySIGINTHandler::DeliverySIGINTHandler(Semaphore &occupied_ovens, ReaderFifo &finished_fifo)
        : occupied_ovens(occupied_ovens), finished_fifo(finished_fifo) {
}

int Delivery::DeliverySIGINTHandler::handle_signal(int signal_number) {
    if (signal_number == SIGINT) {
#ifdef __DEBUG__
    Logger::log(__FILE__, Logger::DEBUG, "Notificado al delivery que solo queda lo que esta en el horno");
#endif
        // Bloqueo de SIGINT
        sigset_t blocking_set;
        sigemptyset(&blocking_set);
        sigaddset(&blocking_set, SIGINT);
        sigprocmask(SIG_BLOCK, &blocking_set, NULL);
        // Graceful Quit
        int pid = fork();
        if (pid == 0) { //Hijo
            occupied_ovens.w(); //Espera que no haya hornos en uso
#ifdef __DEBUG__
    Logger::log(__FILE__, Logger::DEBUG, "No quedan mas pizzas en el horno");
#endif
            exit(EXIT_SUCCESS);
        }
        return 0;
    }
    return -1;
}
