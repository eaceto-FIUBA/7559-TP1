/*
 * PedidosParaHornear.cpp
 *
 *  Created on: May 9, 2015
 *      Author: giovanni
 */

#include "PedidosParaHornear.h"
#include <assert.h>
#include "Constantes.h"

PedidosParaHornear *PedidosParaHornear::instance = NULL;
const string PedidosParaHornear::fileName = SEMAFOROS_PATH + FIFO_A_HORNEAR + SEMAFOROS_EXTENSION;
const string PedidosParaHornear::memoriafileName = MEMORIA_PATH + FIFO_A_HORNEAR + MEMORIA_EXTENSION;
const string PedidosParaHornear::aHornearFileName = MEMORIA_PATH + FIFO_A_HORNEAR + FIFO_EXTENSION;

PedidosParaHornear::PedidosParaHornear() {
    semaforo = new Semaforo(fileName, 0);
    memoria = new MemoriaCompartidaConcurrente<unsigned long>(memoriafileName, 'A');
    cant_hornos = 0;

    fifoLecPedidosAHornear =  new FifoLectura(aHornearFileName);
    fifoEscPedidosAHornear = new FifoEscritura(aHornearFileName);
}

PedidosParaHornear::~PedidosParaHornear() {
    semaforo->eliminar();
    delete semaforo;
    delete memoria;

    fifoEscPedidosAHornear->eliminar();

    delete fifoLecPedidosAHornear;
    delete fifoEscPedidosAHornear;
}


void PedidosParaHornear::setCantHornos(const unsigned int cant_hornos){
	this->cant_hornos = cant_hornos;
}

PedidosParaHornear *PedidosParaHornear::getInstance() {
    if (instance == NULL)
        instance = new PedidosParaHornear();
    return instance;
}

void PedidosParaHornear::destroy() {
    if (instance != NULL) {
        delete (instance);
        instance = NULL;
    }
}

void PedidosParaHornear::inicializarParaEscribir() {
    fifoEscPedidosAHornear->abrir();
}

void PedidosParaHornear::inicializarParaLeer() {
    fifoLecPedidosAHornear->abrir();
}

void PedidosParaHornear::finalizarParaEscribir() {
    fifoEscPedidosAHornear->cerrar();
}

void PedidosParaHornear::finalizarParaLeer() {
    fifoLecPedidosAHornear->cerrar();
}

int PedidosParaHornear::esperarNuevoPedido() {
    return semaforo->p();
}

int PedidosParaHornear::hornearPedido(Pedido &p) {
	//TODO
	//Lock if (memoria->size() < cant_hornos)

	if (memoria->tomarLockManualmente()) {
		unsigned long cantidad = memoria->leerInseguro();
		cantidad++;
		memoria->escribirInseguro(cantidad);

		ssize_t bytesEscritos = fifoEscPedidosAHornear->escribir( static_cast< void* >(&p), sizeof(p) ) ;
		assert(bytesEscritos - sizeof(Pedido) == 0);

        memoria->liberarLockManualmente();
        return semaforo->v();
	}
    return -1;
}

Pedido* PedidosParaHornear::tomarNuevoPedido() {

	if (memoria->tomarLockManualmente()) {
        unsigned long cantidad = memoria->leerInseguro();

        if (cantidad == 0) {
            memoria->liberarLockManualmente();
            return NULL;
        }

        cantidad--;

        Pedido* p = new Pedido();

        ssize_t bytesLeidos = fifoLecPedidosAHornear->leer( static_cast< void* >(p), sizeof(*p) ) ;
        assert(bytesLeidos - sizeof(Pedido) == 0);

        memoria->escribirInseguro(cantidad);

        memoria->liberarLockManualmente();

        return p;
	}
	return NULL;
}

