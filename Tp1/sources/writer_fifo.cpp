#include "writer_fifo.h"

WriterFifo::WriterFifo(const std::string name) : Fifo(name) {
}

WriterFifo::~WriterFifo() {
}

void WriterFifo::open_fifo() {
	fd = open(name.c_str(), O_WRONLY);
}

ssize_t WriterFifo::write_fifo(const void* buffer,const ssize_t buffsize) const {
	return write(fd, buffer, buffsize);
}
