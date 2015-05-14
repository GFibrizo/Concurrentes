//
// Created by debbie on 11/05/15.
//

#ifndef CONCURRENTES_CASH_REGISTER_H
#define CONCURRENTES_CASH_REGISTER_H

#include "shared_memory.h"

class Cash_Register {
private:
    Shared_Memory<float> cash;

public:
    Cash_Register();
    ~Cash_Register();
    bool is_empty();
    bool add_money(float money);
    float get_amount();
};


#endif //CONCURRENTES_CASH_REGISTER_H
