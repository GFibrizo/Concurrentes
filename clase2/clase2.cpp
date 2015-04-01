#include <iostream>
#include <time.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using std::cout;
using std::endl;
using std::rand;
using std::srand;

int main (int argc,char** argv){
	struct shminfo a;

	shmctl(0,IPC_INFO,(shmid_ds*)&a);

	cout << a.shmmin << "B" << endl;
	cout << a.shmmax/1024 << "KB" << endl;
	cout << a.shmmax/(1024*1024) << "MB" << endl;
	cout << a.shmmax/(1024*1024*1024) << "GB" << endl;


	return 0;
}