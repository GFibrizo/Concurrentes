#include "server.h"

int main() {

    Server server = Server();

    //while (true){
    server.get_request();
    //}

    server.stop();
    return 0;
}