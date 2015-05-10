#ifndef LOCKFILE_H_
#define LOCKFILE_H_

#include <unistd.h>
#include <fcntl.h>
#include <string>

class Lock_File {

private:
	struct flock fl;
	int fd;
	std::string nombre;

public:
	Lock_File(const std::string nombre);
	~Lock_File();

	int lock();
	int release();
	ssize_t write_lock(const void* buffer, const ssize_t buffsize) const;
};

#endif /* LOCKFILE_H_ */
