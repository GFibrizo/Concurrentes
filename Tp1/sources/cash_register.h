//
// Created by debbie on 11/05/15.
//

#ifndef CONCURRENTES_CASH_REGISTER_H
#define CONCURRENTES_CASH_REGISTER_H


class Cash_Register {
private:
    Shared_Memory cash;

public:
    Cash_Register();
    ~Cash_Register();
    bool empty();
    bool add_money(float money);
};


#endif //CONCURRENTES_CASH_REGISTER_H
