#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string>

class Semaphore {

private:
    int id;
    int initial_value;

    int initialize() const;

public:
    Semaphore(const std::string &name, const int initial_value);

    ~Semaphore();

    int p() const;  // decrementa
    int v() const;  // incrementa
    int w() const;  // espera a que sea 0
    void remove() const;
};

#endif /* SEMAPHORE_H_ */
