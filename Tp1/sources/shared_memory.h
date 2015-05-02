#ifndef MEMORIACOMPARTIDA2_H_
#define SHAREDMEMORY

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
	void create(const std::string& archivo, const char letra);
	void free();

	Shared_Memory(const std::string& archivo, const char letra);
	Shared_Memory(const Shared_Memory& origen);
	~Shared_Memory();
	Shared_Memory<T>& operator=(const Shared_Memory& origen);
	void write(const T& dato);
	T read() const;
};

template<class T> Shared_Memory<T>::Shared_Memory() :
		sharedm_id(0), data_pointer(NULL) {
}

template<class T> void Shared_Memory<T>::create(const std::string& archivo,
		const char letra) {
	key_t clave = ftok(archivo.c_str(), letra);

	if (clave > 0) {
		this->sharedm_id = shmget(clave, sizeof(T), 0644 | IPC_CREAT);

		if (this->sharedm_id > 0) {
			void* tmpPtr = shmat(this->sharedm_id, NULL, 0);
			if (tmpPtr != (void*) -1) {
				this->data_pointer = static_cast<T*>(tmpPtr);
			} else {
				std::string mensaje = std::string("Error en shmat(): ")
						+ std::string(strerror(errno));
				throw mensaje;
			}
		} else {
			std::string mensaje = std::string("Error en shmget(): ")
					+ std::string(strerror(errno));
			throw mensaje;
		}
	} else {
		std::string mensaje = std::string("Error en ftok(): ")
				+ std::string(strerror(errno));
		throw mensaje;
	}
}

template<class T> void Shared_Memory<T>::free() {
	int errorDt = shmdt((void *) this->data_pointer);

	if (errorDt != -1) {
		int procAdosados = this->attached_process();
		if (procAdosados == 0) {
			shmctl(this->sharedm_id, IPC_RMID, NULL);
		}
	} else {
		std::string mensaje = std::string("Error en shmdt(): ")
				+ std::string(strerror(errno));
		throw mensaje;
	}
}

template<class T> Shared_Memory<T>::Shared_Memory(const std::string& archivo,
		const char letra) :
		sharedm_id(0), data_pointer(NULL) {
	key_t clave = ftok(archivo.c_str(), letra);

	if (clave > 0) {
		this->sharedm_id = shmget(clave, sizeof(T), 0644 | IPC_CREAT);

		if (this->sharedm_id > 0) {
			void* tmpPtr = shmat(this->sharedm_id, NULL, 0);
			if (tmpPtr != (void*) -1) {
				this->data_pointer = static_cast<T*>(tmpPtr);
			} else {
				std::string mensaje = std::string("Error en shmat(): ")
						+ std::string(strerror(errno));
				throw mensaje;
			}
		} else {
			std::string mensaje = std::string("Error en shmget(): ")
					+ std::string(strerror(errno));
			throw mensaje;
		}
	} else {
		std::string mensaje = std::string("Error en ftok(): ")
				+ std::string(strerror(errno));
		throw mensaje;
	}
}

template<class T> Shared_Memory<T>::Shared_Memory(const Shared_Memory& origen) :
		sharedm_id(origen.sharedm_id) {
	void* tmpPtr = shmat(origen.sharedm_id, NULL, 0);

	if (tmpPtr != (void*) -1) {
		this->data_pointer = static_cast<T*>(tmpPtr);
	} else {
		std::string mensaje = std::string("Error en shmat(): ")
				+ std::string(strerror(errno));
		throw mensaje;
	}
}

template<class T> Shared_Memory<T>::~Shared_Memory() {
	int errorDt = shmdt(static_cast<void*>(this->data_pointer));

	if (errorDt != -1) {
		int procAdosados = this->attached_process();
		if (procAdosados == 0) {
			shmctl(this->sharedm_id, IPC_RMID, NULL);
		}
	} else {
		std::cerr << "Error en shmdt(): " << strerror(errno) << std::endl;
	}
}

template<class T> Shared_Memory<T>& Shared_Memory<T>::operator=(
		const Shared_Memory& origen) {
	this->sharedm_id = origen.sharedm_id;
	void* tmpPtr = shmat(this->sharedm_id, NULL, 0);

	if (tmpPtr != (void*) -1) {
		this->data_pointer = static_cast<T*>(tmpPtr);
	} else {
		std::string mensaje = std::string("Error en shmat(): ")
				+ std::string(strerror(errno));
		throw mensaje;
	}

	return *this;
}

template<class T> void Shared_Memory<T>::write(const T& dato) {
	*(this->data_pointer) = dato;
}

template<class T> T Shared_Memory<T>::read() const {
	return *(this->data_pointer);
}

template<class T> int Shared_Memory<T>::attached_process() const {
	shmid_ds estado;
	shmctl(this->sharedm_id, IPC_STAT, &estado);
	return estado.shm_nattch;
}

#endif
