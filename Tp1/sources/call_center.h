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

#ifndef CALL_CENTER_H_
#define CALL_CENTER_H_

#include <stddef.h>
#include <string>

#include "pipe.h"
#include "lock_file.h"
#include "semaphore.h"
#include "writer_fifo.h"

class Call_Center {

private:
    size_t launched_process = 0;
    Semaphore recepcionist;

    Lock_File pipe_lock;
    Pipe internal_pipe;

    Lock_File fifo_lock;
    WriterFifo fifo;

    Semaphore aviable_requests;

private:
    void accept_call(int order);
    void simulate_call(int order);

public:
    Call_Center(Semaphore &recepcionists_semaphore, Semaphore &max_requests_semaphore, Pipe &pipe);

    void accept_calls();
    virtual ~Call_Center();
};

#endif /* CALL_CENTER_H_ */