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

using std::cout;
using std::endl;
using std::rand;
using std::srand;

