#include "fifo.h"

Fifo::Fifo(const std::string filename) : name(filename), fd(-1) {
	int ret = mknod(static_cast<const char *>(filename.c_str()), S_IFIFO | 0666, 0);

	if (ret != 0) {
		int err = errno;
		// Se ignora el EEXIST
		if (err != EEXIST) {
			std::cerr << "Error al crear la fifo " << filename << std::endl;
			if (err == EACCES) {
				std::cerr << "No se poseen permisos para acceder a directorios o crear archivos (EACCES)" << std::endl;
			} else {
				std::cerr << "Numero de error (errno): " << err << std::endl;
			}
		}
	}
}

Fifo::~Fifo() {
}

void Fifo::close_fifo() {
	std::cout << "Cerrada: cola " << std::to_string(fd) <<std::endl;
	close(fd);
	fd = -1;
}

void Fifo::remove() const {
	unlink(name.c_str());
}
