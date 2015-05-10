//
// Created by kimi on 08/05/15.
//

#include "PedidosPorAtender.h"
#include "Constantes.h"
#include <assert.h>

PedidosPorAtender *PedidosPorAtender::instance = NULL;
const string PedidosPorAtender::fileName = SEMAFOROS_PATH + "PedidosPorAtender" + SEMAFOROS_EXTENSION;

const string PedidosPorAtender::aAtenderFileName = MEMORIA_PATH + FIFO_A_ATENDER + FIFO_EXTENSION;

PedidosPorAtender::PedidosPorAtender() {
    semaforo = new Semaforo(fileName, 0);
//    memoria = new MemoriaCompartidaConcurrente<unsigned long>(memoriafileName, 'A');

    fifoLecPedidosAAtender = new FifoLectura(aAtenderFileName);
    fifoEscPedidosAAtender = new FifoEscritura(aAtenderFileName);



//    fifoEscPedidosAAtender->abrir();

}

PedidosPorAtender::~PedidosPorAtender() {
    semaforo->eliminar();
    delete semaforo;
//    delete memoria;

    fifoLecPedidosAAtender->cerrar();
    fifoEscPedidosAAtender->cerrar();

    fifoEscPedidosAAtender->eliminar();

    delete fifoLecPedidosAAtender;
    delete fifoEscPedidosAAtender;
}

PedidosPorAtender *PedidosPorAtender::getInstance() {
    if (instance == NULL)
        instance = new PedidosPorAtender();
    return instance;
}

void PedidosPorAtender::destroy() {
    if (instance != NULL) {
        delete (instance);
        instance = NULL;
    }
}

int PedidosPorAtender::esperarNuevoPedido() {
    return semaforo->p();
}

int PedidosPorAtender::ingresarNuevoPedido(Pedido &p) {
    // incrementar cantidad de pedidos
	fifoEscPedidosAAtender->abrir();
	ssize_t bytesEscritos = fifoEscPedidosAAtender->escribir( static_cast< void* >(&p), sizeof(p) ) ;
	assert(bytesEscritos - sizeof(Pedido) == 0);

	return semaforo->v();
}

Pedido* PedidosPorAtender::tomarNuevoPedido() {

	Pedido *p = new Pedido();
    fifoLecPedidosAAtender->abrir();
	ssize_t bytesLeidos = fifoLecPedidosAAtender->leer( static_cast< void* >(p), sizeof(*p) ) ;
	assert(bytesLeidos - sizeof(Pedido) == 0);

    return p;
}

