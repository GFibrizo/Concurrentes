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

typedef struct flock s_lock;

using std::cout;
using std::endl;
using std::rand;
using std::srand;

int main (int argc,char** argv){
	s_lock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_END;
		
	int fd = open(argv[1],'r');
	fcntl(fd,F_GETLK,&lock);

	if (F_UNLCK == lock.l_type){
		cout << lock.l_pid << endl;
	}else{
		fcntl(fd,F_SETLK,&lock);
		sleep(100);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
	}

	close(fd);
}