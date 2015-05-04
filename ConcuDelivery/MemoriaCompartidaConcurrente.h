//
// Created by kimi on 03/05/15.
//

#ifndef CONCUDELIVERY_MEMORIACOMPARTIDACONCURRENTE_H
#define CONCUDELIVERY_MEMORIACOMPARTIDACONCURRENTE_H

#include "LockFile.h"
#include "MemoriaCompartida2.h"

template<class T>
class MemoriaCompartidaConcurrente {

private:
    MemoriaCompartida2<T> *buffer = NULL;
    char letra = 0;
    LockFile *lockFile;

public:
    MemoriaCompartidaConcurrente(const std::string &archivo, const char letra);

    ~MemoriaCompartidaConcurrente();

    void escribir(const T &dato);
    T leer() const;

    bool tomarLockManualmente();

    void liberarLockManualmente();

    void escribirInseguro(const T &dato);

    T leerInseguro() const;
};

template<class T>
void MemoriaCompartidaConcurrente<T>::escribir(const T &dato) {
    if (lockFile->tomarLock() == 0) {

        escribirInseguro(dato);

        lockFile->liberarLock();
    }
    else {
        std::cerr << "Proceso: " << getpid() << ": Error al abrir el archivo de locks: " << lockFile->nombreArchivo() <<
        " para escritura sobre Memoria Compartida" << endl;
    }
}

template<class T>
T MemoriaCompartidaConcurrente<T>::leer() const {
    T dato;
    if (lockFile->tomarLock() == 0) {

        dato = leerInseguro();

        lockFile->liberarLock();
    }
    else {
        std::cerr << "Proceso: " << getpid() << ": Error al abrir el archivo de locks: " << lockFile->nombreArchivo() <<
        " para lectura sobre Memoria Compartida" << endl;
    }

    return dato;
}


template<class T>
void MemoriaCompartidaConcurrente<T>::escribirInseguro(const T &dato) {
    this->buffer->escribir(dato);
}

template<class T>
T MemoriaCompartidaConcurrente<T>::leerInseguro() const {
    T dato = this->buffer->leer();
    return dato;
}

template<class T>
bool MemoriaCompartidaConcurrente<T>::tomarLockManualmente() {
    return lockFile->tomarLock() == 0;
}

template<class T>
void MemoriaCompartidaConcurrente<T>::liberarLockManualmente() {
    lockFile->liberarLock();
}

template<class T>
MemoriaCompartidaConcurrente<T>::MemoriaCompartidaConcurrente(const std::string &archivo, const char letra) {
    lockFile = new LockFile(archivo);
    buffer = new MemoriaCompartida2<T>(archivo, letra);
}

template<class T>
MemoriaCompartidaConcurrente<T>::~MemoriaCompartidaConcurrente() {
    delete buffer;
    delete lockFile;
}

#endif //CONCUDELIVERY_MEMORIACOMPARTIDACONCURRENTE_H
