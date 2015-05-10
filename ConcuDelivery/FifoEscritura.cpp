#include "FifoEscritura.h"
#include <iostream>
#include <string.h>

FifoEscritura::FifoEscritura(const std::string nombre) : Fifo(nombre) {
}

FifoEscritura::~FifoEscritura() {
}

void FifoEscritura::abrir() {
	fd = open(nombre.c_str(), O_WRONLY);
	if (fd == -1) {
		std::cout << "open error: " << std::to_string(errno) << " " << strerror(errno) << std::endl;
	}
}

ssize_t FifoEscritura::escribir(const void *buffer, const ssize_t buffsize) const {
	return write(fd, buffer, buffsize);
}
