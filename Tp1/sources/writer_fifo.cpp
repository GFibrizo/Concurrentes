#include "writer_fifo.h"

#include <stdlib.h>
#include <string.h>

WriterFifo::WriterFifo(const std::string filename) : Fifo(filename) {
}

WriterFifo::~WriterFifo() {
}

void WriterFifo::open_fifo() {
	fd = open(name.c_str(), O_WRONLY);

    if (fd == -1) {
		std::cerr << "Error al abrir el archivo para escritura" << std::endl;
	}
}

ssize_t WriterFifo::write_fifo(const void* buffer,const ssize_t buffsize) const {
	ssize_t result = write(fd, buffer, buffsize);

    if (result < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    return result;
}
