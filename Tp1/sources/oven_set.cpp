//
// Created by debbie on 12/05/15.
//

#include "oven_set.h"
#include "shared_memory_names.h"

using std::string;

OvenSet::OvenSet(int ovens_number) :
    ovens_sem("ovens", ovens_number), ready_pizzas() {
    for (int i = 0; i < ovens_number; i++)
        ovens.push_back(new Shared_Memory<string>(OVENS_SM, i));
}

OvenSet::~OvenSet() {
    ovens.clear();
}

void OvenSet::cook(string pizza, float time) {
    int n_oven = ovens_sem.p();
    ovens[n_oven]->write(pizza);

    int pid = fork();
    if (pid == 0) {
        sleep(time);
        ready_pizzas.push_back(ovens[n_oven]->read());
        exit(EXIT_SUCCESS);
    }
}

string OvenSet::remove() {
    while (ready_pizzas.empty()) {
        sleep(1);
    }
    string pizza = ready_pizzas.front();
    ready_pizzas.pop_front();
    ovens_sem.v();
    return pizza;
}


