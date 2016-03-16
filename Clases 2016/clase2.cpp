#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <map>

using std::cout;
using std::endl;
using std::rand;
using std::map;

long int rndom() {
    srand(getpid());
    long int result = rand() % 30 + 1;
    return result;
}

void child_process(int *data_pointer, int i) {
    unsigned int s = rndom();

    cout << "Sleep " << s << endl;
    data_pointer[i] = s;
    sleep(s);

    exit(0);
}

int main(int argc, char **argv) {
    int hijos = atoi(argv[1]);

    cout << hijos << endl;

    map <int, int> process = map<int, int>();

    key_t key = ftok(__FILE__, '1');
    int sharedmId = shmget(key, sizeof(int) * hijos, 0644 | IPC_CREAT);
    void *tmpPtr = shmat(sharedmId, NULL, 0);
    int *data_pointer = static_cast<int *>(tmpPtr);

    for (int i = 0; i < hijos; i++) {
        int pid = fork();

        if (pid == 0) {
            child_process(data_pointer, i);
        }
        else {
            process[pid] = i;
        }

    }

    for (int i = 0; i < hijos; i++) {
        pid_t pid = wait(NULL);
        int slept = data_pointer[process[pid]];
        cout << "Kiled " << pid << " after " << slept << endl;
    }

    shmctl(sharedmId, IPC_RMID, NULL);
    return 0;
}