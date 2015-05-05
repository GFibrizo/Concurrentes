#include "fifo.h"

Fifo::Fifo(const std::string name) : name(name), fd(-1) {
	mknod(static_cast<const char*>(name.c_str()), S_IFIFO | 0666, 0);
}

Fifo::~Fifo() {
}

void Fifo::close_fifo() {
	close(fd);
	fd = -1;
}

void Fifo::remove() const {
	unlink(name.c_str());
}
