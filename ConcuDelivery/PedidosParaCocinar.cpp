//
// Created by kimi on 09/05/15.
//

#include "PedidosParaCocinar.h"
#include <assert.h>
#include "Constantes.h"

PedidosParaCocinar *PedidosParaCocinar::instance = NULL;
const string PedidosParaCocinar::fileName = SEMAFOROS_PATH + "PedidosParaCocinar" + SEMAFOROS_EXTENSION;
const string PedidosParaCocinar::aCocinarFileName = MEMORIA_PATH + FIFO_A_COCINAR + FIFO_EXTENSION;

PedidosParaCocinar::PedidosParaCocinar() {
    semaforo = new Semaforo(fileName, 0);
    //memoria = new MemoriaCompartidaConcurrente<unsigned long>(memoriafileName, 'A');

    fifoLecPedidosACocinar =  new FifoLectura(aCocinarFileName);
    fifoEscPedidosACocinar = new FifoEscritura(aCocinarFileName);



}

PedidosParaCocinar::~PedidosParaCocinar() {
    semaforo->eliminar();
    delete semaforo;

    fifoLecPedidosACocinar->cerrar();
    fifoEscPedidosACocinar->cerrar();

	fifoEscPedidosACocinar->eliminar();

    //delete memoria;
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

int PedidosParaCocinar::ingresarPedidoACocinar(Pedido &p) {
    // incrementar cantidad de pedidos
	fifoEscPedidosACocinar->abrir();
	ssize_t bytesEscritos = fifoEscPedidosACocinar->escribir( static_cast< void* >(&p), sizeof(p) ) ;
	assert(bytesEscritos - sizeof(Pedido) == 0);

    return semaforo->v();
//    return -1;
}

Pedido* PedidosParaCocinar::tomarPedidoACocinar() {

	Pedido *p = new Pedido();
    fifoLecPedidosACocinar->abrir();
	ssize_t bytesLeidos = fifoLecPedidosACocinar->leer( static_cast< void* >(p), sizeof(*p) ) ;
	assert(bytesLeidos - sizeof(Pedido) == 0);

    return p;
}

//unsigned long PedidosParaCocinar::cantidadDePedidosParaCocinar() {
//    return memoria->leer();
//}
