//
// Created by debbie on 11/05/15.
//

#include <string>

#include "cash_register.h"
#include "shared_memory_names.h"

using std::string;

Cash_Register::Cash_Register() : cash(Shared_Memory<float>(CASH_REGISTER_SM, 'a')) {
}

Cash_Register::~Cash_Register() {
}

bool Cash_Register::is_empty() {
    return (cash.read() == 0);
}

bool Cash_Register::add_money(float money) {
    if (money < 0)
        return false; //Se puede cambiar por manejo de errores
    cash.write(money);
    return  true;
}

float Cash_Register::get_amount() {
    return cash.read();
}