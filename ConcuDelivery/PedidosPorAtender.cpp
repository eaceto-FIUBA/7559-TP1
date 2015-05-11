//
// Created by kimi on 08/05/15.
//

#include "PedidosPorAtender.h"
#include "Constantes.h"
#include <assert.h>

PedidosPorAtender *PedidosPorAtender::instance = NULL;
const string PedidosPorAtender::fileName = SEMAFOROS_PATH + FIFO_A_ATENDER + SEMAFOROS_EXTENSION;
const string PedidosPorAtender::memoriafileName = MEMORIA_PATH + FIFO_A_ATENDER + MEMORIA_EXTENSION;
const string PedidosPorAtender::aAtenderFileName = MEMORIA_PATH + FIFO_A_ATENDER + FIFO_EXTENSION;

PedidosPorAtender::PedidosPorAtender() {
    semaforo = new Semaforo(fileName, 0);
    memoria = new MemoriaCompartidaConcurrente<unsigned long>(memoriafileName, 'A');
    memoria->escribir(0);

    fifoLecPedidosAAtender = new FifoLectura(aAtenderFileName);
    fifoEscPedidosAAtender = new FifoEscritura(aAtenderFileName);
}

PedidosPorAtender::~PedidosPorAtender() {
    semaforo->eliminar();
    delete semaforo;
    delete memoria;

    fifoEscPedidosAAtender->eliminar();

    delete fifoLecPedidosAAtender;
    delete fifoEscPedidosAAtender;
}

void PedidosPorAtender::inicializarParaEscribir() {
    fifoEscPedidosAAtender->abrir();
}

void PedidosPorAtender::inicializarParaLeer() {
    fifoLecPedidosAAtender->abrir();
}

void PedidosPorAtender::finalizarParaEscribir() {
    fifoEscPedidosAAtender->cerrar();
}

void PedidosPorAtender::finalizarParaLeer() {
    fifoLecPedidosAAtender->cerrar();
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

    if (memoria->tomarLockManualmente()) {
        unsigned long cantidad = memoria->leerInseguro();
        cantidad++;
        memoria->escribirInseguro(cantidad);

        ssize_t bytesEscritos = fifoEscPedidosAAtender->escribir(static_cast< void * >(&p), sizeof(p));
        assert(bytesEscritos - sizeof(Pedido) == 0);

        memoria->liberarLockManualmente();
        return semaforo->v();
    }
    return -1;
}

Pedido* PedidosPorAtender::tomarNuevoPedido() {

    if (memoria->tomarLockManualmente()) {
        unsigned long cantidad = memoria->leerInseguro();

        if (cantidad == 0) {
            memoria->liberarLockManualmente();
            return NULL;
        }

        cantidad--;

        Pedido *p = new Pedido();

        ssize_t bytesLeidos = fifoLecPedidosAAtender->leer(static_cast< void * >(p), sizeof(*p));
        assert(bytesLeidos - sizeof(Pedido) == 0);

        memoria->escribirInseguro(cantidad);

        memoria->liberarLockManualmente();

        return p;
    }
    return NULL;
}

