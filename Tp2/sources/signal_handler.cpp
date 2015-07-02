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

#include "signal_handler.h"

SignalHandler *SignalHandler::instance = NULL;
EventHandler *SignalHandler::handlers[NSIG];

SignalHandler::SignalHandler() {
}

SignalHandler *SignalHandler::get_instance() {
    if (instance == NULL) {
        instance = new SignalHandler();
    }
    return instance;
}

void SignalHandler::destroy() {
    if (instance != NULL) {
        delete(instance);
        instance = NULL;
    }
}

EventHandler *SignalHandler::register_handler(int signal_number, EventHandler *handler) {
    EventHandler *old_handler = SignalHandler::handlers[signal_number];
    SignalHandler::handlers[signal_number] = handler;

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = SignalHandler::dispatcher;
    sigemptyset(
            &action.sa_mask);    // inicializa la mascara de seniales a bloquear durante la ejecucion del handler como vacio
    sigaddset(&action.sa_mask, signal_number);
    sigaction(signal_number, &action, 0);    // cambiar accion de la senial

    return old_handler;
}

void SignalHandler::dispatcher(int signal_number) {
    if (SignalHandler::handlers[signal_number] != 0) {
        SignalHandler::handlers[signal_number]->handle_signal(signal_number);
    }
}

int SignalHandler::remove_handler(int signal_number) {
    SignalHandler::handlers[signal_number] = NULL;
    return 0;
}
