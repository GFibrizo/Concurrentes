#ifndef READERFIFO_H_
#define READERFIFO_H_

#include "fifo.h"

class ReaderFifo : public Fifo {
public:
	ReaderFifo(const std::string filename);
	~ReaderFifo();

	void open_fifo();
	ssize_t read_fifo(void* buffer, const ssize_t buffsize) const;
};

#endif /* READERFIFO_H_ */
