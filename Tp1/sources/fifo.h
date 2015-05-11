#ifndef FIFO_H_
#define FIFO_H_

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

class Fifo {

public:
	Fifo(const std::string filename);
	virtual ~Fifo();
	virtual void open_fifo() = 0;
	void close_fifo();
	void remove() const;

protected:
	std::string name;
	int fd;
};

#endif /* FIFO_H_ */
