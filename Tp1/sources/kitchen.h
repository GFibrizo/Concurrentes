/**
 kitchen.h

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

#ifndef KITCHEN_H
#define KITCHEN_H

#include <stddef.h>
#include <string>

#include "pipe.h"
#include "lock_file.h"
#include "semaphore.h"
#include "reader_fifo.h"
#include "oven_set.h"

class Kitchen {

private:
    Semaphore chefs;
    size_t launched_process;

    Lock_File request_fifo_lock;
    ReaderFifo requests_fifo;

    Semaphore max_requests;

    OvenSet& ovens;

private:
    void accept_order(int order);
    void simulate_cook(int order);

public:
    Kitchen(Semaphore &,Semaphore&, OvenSet&);
    void accept_orders();
};

#endif /* KITCHEN_H */
