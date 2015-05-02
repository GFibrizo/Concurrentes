#ifndef PIPE_H_
#define PIPE_H_

#include <unistd.h>
#include <fcntl.h>

class Pipe {

private:
	int descriptores[2];
	bool lectura;
	bool escritura;

public:
	static const int READ = 0;
	static const int WRITE = 1;

	Pipe();
	~Pipe();

	void set_mode(const int modo);

	ssize_t write_pipe(const void* dato, const int datoSize);
	ssize_t read_pipe(void* buffer, const int buffSize);

	int get_read_fd() const;
	int get_write_fd() const;

	void close_pipe();
};

#endif /* PIPE_H_ */
