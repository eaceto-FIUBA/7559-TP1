//
// Created by kimi on 09/05/15.
//

#include "PedidosParaCocinar.h"
#include <assert.h>
#include "Constantes.h"

PedidosParaCocinar *PedidosParaCocinar::instance = NULL;
const string PedidosParaCocinar::fileName = SEMAFOROS_PATH + FIFO_A_COCINAR + SEMAFOROS_EXTENSION;
const string PedidosParaCocinar::memoriafileName = MEMORIA_PATH + FIFO_A_COCINAR + MEMORIA_EXTENSION;
const string PedidosParaCocinar::aCocinarFileName = MEMORIA_PATH + FIFO_A_COCINAR + FIFO_EXTENSION;

PedidosParaCocinar::PedidosParaCocinar() {
    semaforo = new Semaforo(fileName, 0);
    memoria = new MemoriaCompartidaConcurrente<unsigned long>(memoriafileName, 'A');
    memoria->escribir(0);

    fifoLecPedidosACocinar =  new FifoLectura(aCocinarFileName);
    fifoEscPedidosACocinar = new FifoEscritura(aCocinarFileName);
}

PedidosParaCocinar::~PedidosParaCocinar() {
    semaforo->eliminar();
    delete semaforo;
    delete memoria;

	fifoEscPedidosACocinar->eliminar();

    delete fifoLecPedidosACocinar;
    delete fifoEscPedidosACocinar;

}

PedidosParaCocinar *PedidosParaCocinar::getInstance() {
    if (instance == NULL)
        instance = new PedidosParaCocinar();
    return instance;
}

void PedidosParaCocinar::destroy() {
    if (instance != NULL) {
        delete (instance);
        instance = NULL;
    }
}

int PedidosParaCocinar::esperarPedidoACocinar() {
    return semaforo->p();
}

void PedidosParaCocinar::inicializarParaEscribir() {
    fifoEscPedidosACocinar->abrir();
}

void PedidosParaCocinar::inicializarParaLeer() {
    fifoLecPedidosACocinar->abrir();
}

void PedidosParaCocinar::finalizarParaEscribir() {
    fifoEscPedidosACocinar->cerrar();
}

void PedidosParaCocinar::finalizarParaLeer() {
    fifoLecPedidosACocinar->cerrar();
}

int PedidosParaCocinar::ingresarPedidoACocinar(Pedido &p) {
	if (memoria->tomarLockManualmente()) {
		unsigned long cantidad = memoria->leerInseguro();
		cantidad++;
		memoria->escribirInseguro(cantidad);

		ssize_t bytesEscritos = fifoEscPedidosACocinar->escribir( static_cast< void* >(&p), sizeof(p) ) ;
		assert(bytesEscritos - sizeof(Pedido) == 0);

        memoria->liberarLockManualmente();
        return semaforo->v();
	}
    return -1;
}

Pedido* PedidosParaCocinar::tomarPedidoACocinar() {

	if (memoria->tomarLockManualmente()) {
        unsigned long cantidad = memoria->leerInseguro();

        if (cantidad == 0) {
            memoria->liberarLockManualmente();
            return NULL;
        }

        cantidad--;

        Pedido *p = new Pedido();

        ssize_t bytesLeidos = fifoLecPedidosACocinar->leer( static_cast< void* >(p), sizeof(*p) ) ;
        assert(bytesLeidos - sizeof(Pedido) == 0);

        memoria->escribirInseguro(cantidad);

        memoria->liberarLockManualmente();

        return p;
	}

	return NULL;
}



//unsigned long PedidosParaCocinar::cantidadDePedidosParaCocinar() {
//    return memoria->leer();
//}
