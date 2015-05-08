//
// Created by kimi on 08/05/15.
//

#include "PedidosPorAtender.h"
#include "Constantes.h"

PedidosPorAtender *PedidosPorAtender::instance = NULL;
const string PedidosPorAtender::fileName = SEMAFOROS_PATH + "PedidosPorAtender" + SEMAFOROS_EXTENSION;
const string PedidosPorAtender::memoriafileName = SEMAFOROS_PATH + "PedidosPorAtender" + MEMORIA_EXTENSION;

PedidosPorAtender::PedidosPorAtender() {
    semaforo = new Semaforo(fileName, 0);
    memoria = new MemoriaCompartidaConcurrente<unsigned long>(memoriafileName, 'A');
}

PedidosPorAtender::~PedidosPorAtender() {
    semaforo->eliminar();
    delete semaforo;
    delete memoria;
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

int PedidosPorAtender::ingresarNuevoPedido() {
    // incrementar cantidad de pedidos
    if (memoria->tomarLockManualmente()) {
        unsigned long cantDePedidos = memoria->leerInseguro();
        cantDePedidos++;
        memoria->escribirInseguro(cantDePedidos);
        memoria->liberarLockManualmente();
        return semaforo->v();
    }
    return -1;
}

bool PedidosPorAtender::tomarNuevoPedido() {
    if (memoria->tomarLockManualmente()) {
        bool tomado = false;

        unsigned long cantDePedidos = memoria->leerInseguro();
        if (cantDePedidos > 0) {
            tomado = true;
            cantDePedidos--;
            memoria->escribirInseguro(cantDePedidos);
        }

        memoria->liberarLockManualmente();
        return tomado;
    }
    return false;
}

