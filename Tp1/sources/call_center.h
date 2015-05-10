/**
 call_center.h

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
    Pipe internal_pipe;

    Lock_File fifo_lock;
    WriterFifo fifo;

private:
    void accept_call(std::string);

    void simulate_call(std::string);

public:
    Call_Center(Semaphore &recepcionists, Pipe pipe);

    void accept_calls();

    virtual ~Call_Center();
};

#endif /* CALL_CENTER_H_ */
