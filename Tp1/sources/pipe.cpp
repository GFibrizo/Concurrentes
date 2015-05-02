#include "pipe.h"

Pipe::Pipe() :
		lectura(true), escritura(true) {
	pipe(this->descriptores);
	/*fcntl ( this->descriptors[0],F_SETFL,O_NONBLOCK );
	 fcntl ( this->descriptors[1],F_SETFL,O_NONBLOCK );*/
}

Pipe::~Pipe() {
}

void Pipe::set_mode(const int modo) {
	if (modo == READ) {
		close(this->descriptores[1]);
		this->escritura = false;

	} else if (modo == WRITE) {
		close(this->descriptores[0]);
		this->lectura = false;
	}
}

ssize_t Pipe::write_pipe(const void* dato, int datoSize) {
	if (this->lectura == true) {
		close(this->descriptores[0]);
		this->lectura = false;
	}

	return write(this->descriptores[1], dato, datoSize);
}

ssize_t Pipe::read_pipe(void* buffer, const int buffSize) {
	if (this->escritura == true) {
		close(this->descriptores[1]);
		this->escritura = false;
	}

	return read(this->descriptores[0], buffer, buffSize);
}

int Pipe::get_read_fd() const {
	if (this->lectura == true)
		return this->descriptores[0];
	else
		return -1;
}

int Pipe::get_write_fd() const {
	if (this->escritura == true)
		return this->descriptores[1];
	else
		return -1;
}

void Pipe::close_pipe() {
	if (this->lectura == true) {
		close(this->descriptores[0]);
		this->lectura = false;
	}

	if (this->escritura == true) {
		close(this->descriptores[1]);
		this->escritura = false;
	}
}
