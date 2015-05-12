//
// Created by debbie on 11/05/15.
//

#include "Cash_Register.h"
#include "shared_memory_names.h"

Cash_Register::Cash_Register() :
    cash(Shared_Memory(CASH_REGISTER_SM)){
}

Cash_Register::~Cash_Register() {
}

bool Cash_Register::empty() {
    return (cash.read() == 0);
}

bool CashRegister::add_money(float money) {
    if (money < 0)
        return false; //Se puede cambiar por manejo de errores
    cash.write(money);
}
