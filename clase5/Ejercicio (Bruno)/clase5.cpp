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
    
    int fd = open(argv[1], O_RDONLY);
    
    int pid = fork();
    if (pid == 0) {
        // Hijo
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        cout << "Hijo: Lockeando archivo." << endl;
        fcntl(fd, F_SETLK, &lock);
        sleep(10);
        lock.l_type = F_UNLCK;
        cout << "Hijo: Desbloqueando archivo." << endl;
        fcntl(fd, F_SETLK, &lock);
    } else {
        // Padre
        sleep(5);
        lock.l_type = F_UNLCK;
        cout << "Padre: Testeando archivo." << endl;
        fcntl(fd, F_GETLK, &lock);
        if (lock.l_type == F_UNLCK) {
            cout << "No hay un lock en el archivo." << endl;
        } else {
            cout << "Hay un lock en el archivo." << endl;
        }
    }
    
    wait(0);
    
    close(fd);
    
    return 0;
}
