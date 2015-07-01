#include "server.h"

int main() {

    Server server = Server();

    //while (true){
    server.process_next_request();
    //}

    server.stop();
    return 0;
}