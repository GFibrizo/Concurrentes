#include "writer_fifo.h"

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
	return write(fd, buffer, buffsize);
}
