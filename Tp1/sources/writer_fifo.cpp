#include "writer_fifo.h"
#include <string>

WriterFifo::WriterFifo(const std::string filename) : Fifo(filename) {
}

WriterFifo::~WriterFifo() {
}

void WriterFifo::open_fifo() {
	fd = open(name.c_str(), O_WRONLY);

    std::cout << "Open: cola " << std::to_string(fd) << " - " << name <<std::endl;

    if (fd == -1) {
		std::cerr << "Error al abrir el archivo para escritura" << std::endl;
	}
}

ssize_t WriterFifo::write_fifo(const void* buffer,const ssize_t buffsize) const {
	std::cout << "FD: " << std::to_string(fd) << std::endl;
	return write(fd, buffer, buffsize);
}
