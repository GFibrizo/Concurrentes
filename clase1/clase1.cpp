#include <iostream>
#include <time.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>

using std::cout;
using std::endl;
using std::rand;
using std::srand;
//using std::fork;

long int rndom(){
	srand(getpid());
	long int result = rand()%30+1;
	return result;
}

int main (int argc,char** argv){
	int hijos = atoi(argv[1]);

	cout << hijos << endl;

	for (int i=0;i<hijos;i++){
		int pid = fork();

		if(pid ==0){
			unsigned int s =rndom();
			sleep(s);
			cout << "exited after ->" << s << endl;
			exit(0);
		}

	}
	
	for (int i=0;i<hijos;i++){
		wait(NULL);
		cout << "Kiled " << i+1 << endl;
	}

	return 0;
}