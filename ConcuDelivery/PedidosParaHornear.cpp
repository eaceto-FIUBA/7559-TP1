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
const string PedidosParaHornear::fileName = SEMAFOROS_PATH + "PedidosParaHornear" + SEMAFOROS_EXTENSION;
const string PedidosParaHornear::aHornearFileName = MEMORIA_PATH + FIFO_A_HORNEAR + FIFO_EXTENSION;

PedidosParaHornear::PedidosParaHornear() {
    semaforo = new Semaforo(fileName, 0);
//    memoria = new MemoriaCompartidaConcurrente<unsigned long>(memoriafileName, 'A');
    cant_hornos = 0;

    fifoLecPedidosAHornear =  new FifoLectura(aHornearFileName);
    fifoEscPedidosAHornear = new FifoEscritura(aHornearFileName);
}

PedidosParaHornear::~PedidosParaHornear() {
    semaforo->eliminar();
    delete semaforo;
//    delete memoria;

    fifoLecPedidosAHornear->cerrar();
    fifoEscPedidosAHornear->cerrar();

    fifoEscPedidosAHornear->eliminar();
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

int PedidosParaHornear::esperarNuevoPedido() {
    return semaforo->p();
}

int PedidosParaHornear::hornearPedido(Pedido &p) {
	//TODO
	//Lock if (memoria->size() < cant_hornos)

    // incrementar cantidad de pedidos
    fifoEscPedidosAHornear->abrir();
	ssize_t bytesEscritos = fifoEscPedidosAHornear->escribir( static_cast< void* >(&p), sizeof(p) ) ;
	assert(bytesEscritos - sizeof(Pedido) == 0);

    return semaforo->v();
}

Pedido* PedidosParaHornear::tomarNuevoPedido() {

	Pedido* p = new Pedido();
	fifoLecPedidosAHornear->abrir();
	ssize_t bytesLeidos = fifoLecPedidosAHornear->leer( static_cast< void* >(p), sizeof(*p) ) ;
	assert(bytesLeidos - sizeof(Pedido) == 0);

	return p;
}
