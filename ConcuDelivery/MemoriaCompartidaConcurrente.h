#ifndef CONCUDELIVERY_MEMORIACOMPARTIDACONCURRENTE_H
#define CONCUDELIVERY_MEMORIACOMPARTIDACONCURRENTE_H

#include "MemoriaCompartida2.h"
#include "LockFile.h"

template<class T>
class MemoriaCompartidaConcurrente {

private:
    MemoriaCompartida2<T> *buffer = NULL;
    char letra = 0;
    LockFile* lockFile;

public:
    MemoriaCompartidaConcurrente(const std::string &archivo, const char letra);

    ~MemoriaCompartidaConcurrente();

    void escribir(const T &dato);
    T leer() const;

    void escribirInseguro(const T &dato);
    T leerInseguro() const;

    bool tomarLockManualmente () ;
    bool liberarLockManualmente ();

};

template<class T>
bool MemoriaCompartidaConcurrente<T>::tomarLockManualmente ()  {
    return (lockFile->tomarLock() == 0);
}

template<class T>
bool MemoriaCompartidaConcurrente<T>::liberarLockManualmente () {
    lockFile->liberarLock();
    return true;
}

template<class T>
void MemoriaCompartidaConcurrente<T>::escribirInseguro(const T &dato) {
    buffer->escribir(dato);
}

template<class T>
T MemoriaCompartidaConcurrente<T>::leerInseguro() const {
    T dato = buffer->leer();
    return dato;
}

template<class T>
void MemoriaCompartidaConcurrente<T>::escribir(const T &dato) {
    if (lockFile->tomarLock() == 0) {

        buffer->escribir(dato);

        lockFile->liberarLock();
    }
    else {
        std::cerr << "Proceso: " << getpid() << ": Error al abrir el archivo de locks: " << lockFile->nombreArchivo() <<
        " para escritura sobre Memoria Compartida";
    }
}

template<class T>
T MemoriaCompartidaConcurrente<T>::leer() const {
    T dato;
    if (lockFile->tomarLock() == 0) {

        dato = buffer->leer();

        lockFile->liberarLock();
    }
    else {
        std::cerr << "Proceso: " << getpid() << ": Error al abrir el archivo de locks: " << lockFile->nombreArchivo() <<
        " para lectura sobre Memoria Compartida";
    }

    return dato;
}

template<class T>
MemoriaCompartidaConcurrente<T>::MemoriaCompartidaConcurrente(const std::string &archivo, const char letra) {
    lockFile = new LockFile(archivo);
    buffer = new MemoriaCompartida2<T>(archivo, letra);
}

template<class T>
MemoriaCompartidaConcurrente<T>::~MemoriaCompartidaConcurrente() {
    buffer->liberar();
    delete buffer;
    delete lockFile;
}

#endif //CONCUDELIVERY_MEMORIACOMPARTIDACONCURRENTE_H
