#include "server.h"

int main() {

    Server server = Server();
    int status = 0;

    while (status != -1) {
        status = server.process_next_request();
    }

    return 0;
}