/**
 Copyright 2015
 Debora Martin debbie1new.world@gmail.com
 Gaston Martinez gaston.martinez.90@gmail.com
 Bruno Merlo Schurmann bruno290@gmail.com

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

#ifndef DELIVERY_H_
#define DELIVERY_H_

#include <stddef.h>
#include <string>

#include "lock_file.h"
#include "semaphore.h"
#include "reader_fifo.h"
#include "event_handler.h"
#include "signal_handler.h"
#include "shared_memory.h"
#include "locknames.h"
#include "pipenames.h"

class Delivery {

private:
    size_t launched_process = 0;
    Semaphore cadets;

    Shared_Memory<int> *ovens;
    Semaphore free_ovens;
    Semaphore occupied_ovens;
    Lock_File ovens_lock;

    ReaderFifo finished_fifo;

    Lock_File cash_register_lock;
    Shared_Memory<float> cash_register;

private:
    void simulate_delivery(int oven_number);

    void make_delivery(int oven_number);

public:
    Delivery(Semaphore &cadets_semaphore, Shared_Memory<int> *ovens, Semaphore &free_ovens_semaphore,
             Semaphore &occupied_ovens_semaphore, Shared_Memory<float> &cash_register);

    void start_deliveries();

private:
    class DeliverySIGINTHandler : public EventHandler {

    private:
        Semaphore occupied_ovens;
        ReaderFifo finished_fifo;

    public:
        DeliverySIGINTHandler(Semaphore &occupied_ovens, ReaderFifo &finished_fifo);

        int handle_signal(int signal_number);
    };

};

#endif /* DELIVERY_H_ */
