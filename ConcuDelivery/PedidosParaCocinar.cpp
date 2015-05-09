//
// Created by kimi on 09/05/15.
//

#include "PedidosParaCocinar.h"

#include "Constantes.h"

PedidosParaCocinar *PedidosParaCocinar::instance = NULL;
const string PedidosParaCocinar::fileName = SEMAFOROS_PATH + "PedidosParaCocinar" + SEMAFOROS_EXTENSION;
const string PedidosParaCocinar::memoriafileName = MEMORIA_PATH + "PedidosParaCocinar" + MEMORIA_EXTENSION;

PedidosParaCocinar::PedidosParaCocinar() {
    semaforo = new Semaforo(fileName, 0);
    memoria = new MemoriaCompartidaConcurrente<unsigned long>(memoriafileName, 'A');
}

PedidosParaCocinar::~PedidosParaCocinar() {
    semaforo->eliminar();
    delete semaforo;
    delete memoria;
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

int PedidosParaCocinar::esperarNuevoPedido() {
    return semaforo->p();
}

int PedidosParaCocinar::ingresarNuevoPedido() {
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

bool PedidosParaCocinar::tomarNuevoPedido() {
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