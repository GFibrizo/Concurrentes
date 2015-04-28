/*
	Escribir un programa en C o C++ que reciba un archivo como argumento y muestre si tiene o no locks, de 
	que tipo y el pid del proceso que lo tiene.
	Usar y revisar el man de flock y fcntl.
*/

#include <iostream>
#include <time.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <fcntl.h>

using std::cout;
using std::endl;

int main(int argc, char** argv) {
    struct flock lock;
    
    int fd;
    
    int pid = fork();
    if (pid == 0) {
        // Hijo
        cout << "Hijo: pid " << getpid() << endl;
        fd = open(argv[1], O_RDWR);
        char type = argv[2][0];
        if (type == '1') {
            lock.l_type = F_RDLCK;
        } else if (type == '2') {
            lock.l_type = F_WRLCK;
        } else {
            lock.l_type = F_UNLCK;
        }
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        if (lock.l_type != F_UNLCK) {
            cout << "Hijo: Lockeando archivo." << endl;
        }
        fcntl(fd, F_SETLK, &lock);
        sleep(5);
        lock.l_type = F_UNLCK;
        cout << "Hijo: Desbloqueando archivo." << endl;
        fcntl(fd, F_SETLK, &lock);
        close(fd);
    } else {
        // Padre
        sleep(2);
        fd = open(argv[1], O_RDONLY);
        lock.l_type = F_WRLCK;
        cout << "Padre: Testeando archivo." << endl;
        fcntl(fd, F_GETLK, &lock);
        if (lock.l_type == F_UNLCK) {
            cout << "No hay un lock en el archivo." << endl;
        } else {
            cout << "Hay un lock en el archivo." << endl;
            cout << "Tipo: ";
            if (lock.l_type == F_RDLCK) {
                cout << "Lectura";
            } else {
                cout << "Escritura";
            }
            cout << endl;
            cout << "Proceso: " << lock.l_pid << endl;
        }
        close(fd);
    }
    
    wait(0);
    
    return 0;
}
