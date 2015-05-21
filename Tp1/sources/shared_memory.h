#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <string.h>
#include <iostream>
#include <errno.h>

template<class T> class Shared_Memory {

private:
	int sharedm_id;
	T* data_pointer;

	int attached_process() const;

public:
	Shared_Memory();
	void create(const std::string &file, const char salt);
	void free();

	Shared_Memory(const std::string &file, const char salt);
	Shared_Memory(const Shared_Memory &origin);
	~Shared_Memory();
	Shared_Memory<T>& operator=(const Shared_Memory &origin);
	void write(const T &data);
	T read() const;
};

template<class T> Shared_Memory<T>::Shared_Memory() :
		sharedm_id(0), data_pointer(NULL) {
}

template<class T> void Shared_Memory<T>::create(const std::string &file,
		const char salt) {
	key_t key = ftok(file.c_str(), salt);
	shared_size = sm_size;
	if (key > 0) {
		this->sharedm_id = shmget(key, sizeof(T), 0644 | IPC_CREAT);

		if (this->sharedm_id > 0) {
			void* tmpPtr = shmat(this->sharedm_id, NULL, 0);
			if (tmpPtr != (void*) -1) {
				this->data_pointer = static_cast<T*>(tmpPtr);
			} else {
				std::string error_msg = std::string("Error en shmat(): ") + std::string(strerror(errno));
				throw error_msg;
			}
		} else {
			std::string error_msg = std::string("Error en shmget(): ") + std::string(strerror(errno));
			throw error_msg;
		}
	} else {
		std::string error_msg = std::string("Error en ftok(): ") + std::string(strerror(errno));
		throw error_msg;
	}
}

template<class T> void Shared_Memory<T>::free() {
	int errorDt = shmdt((void *) this->data_pointer);

	if (errorDt != -1) {
		if (this->attached_process() == 0) {
			shmctl(this->sharedm_id, IPC_RMID, NULL);
		}
	} else {
		std::string error_msg = std::string("Error en shmdt(): ") + std::string(strerror(errno));
		throw error_msg;
	}
}

template<class T> Shared_Memory<T>::Shared_Memory(const std::string &file, const char salt) : sharedm_id(0), data_pointer(NULL) {
	key_t key = ftok(file.c_str(), salt);

	if (key > 0) {
		this->sharedm_id = shmget(key, sizeof(T), 0644 | IPC_CREAT);

		if (this->sharedm_id > 0) {
			void* tmp_ptr = shmat(this->sharedm_id, NULL, 0);
			if (tmp_ptr != (void*) -1) {
				this->data_pointer = static_cast<T*>(tmp_ptr);
			} else {
				std::string error_msg = std::string("Error en shmat(): ") + std::string(strerror(errno));
				throw error_msg;
			}
		} else {
			std::string error_msg = std::string("Error en shmget(): ") + std::string(strerror(errno));
			throw error_msg;
		}
	} else {
		std::string error_msg = std::string("Error en ftok(): ") + std::string(strerror(errno));
		throw error_msg;
	}
}

template<class T> Shared_Memory<T>::Shared_Memory(const Shared_Memory &origin) : sharedm_id(origin.sharedm_id) {
	void* tmp_ptr = shmat(origin.sharedm_id, NULL, 0);

	if (tmp_ptr != (void*) -1) {
		this->data_pointer = static_cast<T*>(tmp_ptr);
	} else {
		std::string error_msg = std::string("Error en shmat(): ") + std::string(strerror(errno));
		throw error_msg;
	}
}

template<class T> Shared_Memory<T>::~Shared_Memory() {
	int error = shmdt(static_cast<void*>(this->data_pointer));

	if (error != -1) {
		if (this->attached_process() == 0) {
			shmctl(this->sharedm_id, IPC_RMID, NULL);
		}
	} else {
		std::cerr << "Error en shmdt(): " << strerror(errno) << std::endl;
	}
}

template<class T> Shared_Memory<T>& Shared_Memory<T>::operator=(const Shared_Memory &origin) {
	this->sharedm_id = origin.sharedm_id;
	void* tmp_ptr = shmat(this->sharedm_id, NULL, 0);

	if (tmp_ptr != (void*) -1) {
		this->data_pointer = static_cast<T*>(tmp_ptr);
	} else {
		std::string error_msg = std::string("Error en shmat(): ") + std::string(strerror(errno));
		throw error_msg;
	}

	return *this;
}

template<class T> void Shared_Memory<T>::write(const T &data) {
	*(this->data_pointer) = data;
}

template<class T> T Shared_Memory<T>::read() const {
	return *(this->data_pointer);
}

template<class T> int Shared_Memory<T>::attached_process() const {
	shmid_ds status;
	shmctl(this->sharedm_id, IPC_STAT, &status);
	return status.shm_nattch;
}

#endif
