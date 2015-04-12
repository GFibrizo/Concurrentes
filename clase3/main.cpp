// Escribir un programa que establezca una mascara para una o mas señales en el proceso. 
// Observar el valor de la mascara en el S.O. (/proc)
// Sigprocmask()
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

using std::cout;
using std::endl;
using std::rand;
using std::srand;

int main (int argc,char** argv){

	sigset_t a;
	sigemptyset(&a);

	sigaddset(&a,SIGINT);

	sigprocmask(SIG_BLOCK,&a,0);


	cout << getpid() << endl;

	while (true){sleep(1);}

}