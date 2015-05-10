#ifndef WRITERFIFO_H_
#define WRITERFIFO_H_

#include "fifo.h"

class WriterFifo : public Fifo {
public:
	WriterFifo(const std::string filename);
	~WriterFifo();

	void open_fifo();
	ssize_t write_fifo(const void* buffer,const ssize_t buffsize) const;
};

#endif /* WRITERFIFO_H_ */
