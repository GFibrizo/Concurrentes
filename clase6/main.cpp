#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;

void p(int sem_id) {
    struct sembuf operation;
    operation.sem_num = 0;    // numero de semaforo
    operation.sem_op = -1;    // restar 1 al semaforo
    operation.sem_flg = 0;

    semop(sem_id, &operation, 1);
}

void v(int sem_id) {
    struct sembuf operation;
    operation.sem_num = 0;    // numero de semaforo
    operation.sem_op = 1;     // sumar 1 al semaforo
    operation.sem_flg = 0;

    semop(sem_id, &operation, 1);
}

void w(int sem_id) {
    struct sembuf operation;
    operation.sem_num = 0;    // numero de semaforo
    operation.sem_op = 0;     // espera a que sea 0
    operation.sem_flg = 0;

    semop(sem_id, &operation, 1);
}

int main() {

    key_t key = ftok(__FILE__, 'a');
    int sem_id = semget(key, 1, 0666 | IPC_CREAT);

    union semnum {
        int val;
        struct semid_ds *buf;
        ushort *array;
    };

    semnum init;
    init.val = 0;
    semctl(sem_id, 0, SETVAL, init);

    pid_t pid = fork();
    if (pid) {
        //Padre
        int a = 0;
        while (true) {
            w(sem_id);
            ofstream myfile;
            myfile.open("a.txt", std::ofstream::out);
            myfile << a++ % 20;
            myfile.close();
            v(sem_id);
            v(sem_id); //Aumento en 2

        }
    } else {
        //Hijo
        int b = 0;
        while (true) {
            p(sem_id);
            ifstream myfile;
            myfile.open("a.txt", std::ifstream::in);
            myfile >> b;
            myfile.close();
            cout << b << endl;
            p(sem_id);
        }
    }
}