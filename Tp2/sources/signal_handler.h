//
// Copyright 2015
// Bruno Merlo Schurmann bruno290@gmail.com
// Gaston Martinez gaston.martinez.90@gmail.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses
//

#ifndef SIGNALHANDLER_H_
#define SIGNALHANDLER_H_

#include <signal.h>
#include <stdio.h>
#include <memory.h>

#include "event_handler.h"

class SignalHandler {

private:
    static SignalHandler *instance;
    static EventHandler *handlers[NSIG];

    SignalHandler(void);

    static void dispatcher(int signal_number);

public:
    static SignalHandler *get_instance();

    static void destroy();

    EventHandler *register_handler(int signal_number, EventHandler *handler);

    int remove_handler(int signal_number);

};

#endif /* SIGNALHANDLER_H_ */
