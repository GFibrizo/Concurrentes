#include "semaphore.h"

Semaphore::Semaphore(const std::string& name, const int initial_value)
		: initial_value(initial_value) {
	key_t key = ftok(name.c_str(), 'a');
	this->id = semget(key, 1, 0666 | IPC_CREAT);

	this->initialize();
}

Semaphore::~Semaphore() {
}

int Semaphore::initialize() const {

	union semnum {
		int val;
		struct semid_ds* buf;
		ushort* array;
	};

	semnum init;
	init.val = this->initial_value;
	int res = semctl(this->id, 0, SETVAL, init);
	return res;
}

int Semaphore::p() const {

	struct sembuf operation;

	operation.sem_num = 0;	// numero de semaforo
	operation.sem_op  = -1;	// restar 1 al semaforo
	operation.sem_flg = SEM_UNDO;

	return semop(this->id, &operation, 1); //FIXME
}

int Semaphore::v() const {

	struct sembuf operation;

	operation.sem_num = 0;	// numero de semaforo
	operation.sem_op  = 1;	// sumar 1 al semaforo
	operation.sem_flg = SEM_UNDO;

	return semop(this->id,&operation, 1); //FIXME
}

void Semaphore::remove() const {
	semctl(this->id, 0, IPC_RMID);
}
