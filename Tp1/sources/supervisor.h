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

#ifndef REGISTER_SUPERVISOR_H
#define REGISTER_SUPERVISOR_H

#include "lock_file.h"
#include "shared_memory.h"

class Supervisor {

private:
    Lock_File register_lock;
    Shared_Memory<float> cash_register;
    float checking_interval;

public:
    Supervisor(Shared_Memory<float> &cash_register, float checking_interval);

    ~Supervisor();

    void start_checking_register();
};


#endif /* REGISTER_SUPERVISOR_H */
