//
// Created by kimi on 09/05/15.
//

#include "PedidosParaEntregar.h"

#include "Constantes.h"

PedidosParaEntregar *PedidosParaEntregar::instance = NULL;
const string PedidosParaEntregar::fileName = SEMAFOROS_PATH + "PedidosPorEntregar" + SEMAFOROS_EXTENSION;
const string PedidosParaEntregar::memoriafileName = MEMORIA_PATH + "PedidosPorEntregar" + MEMORIA_EXTENSION;
const string PedidosParaEntregar::pedidosFilename = MEMORIA_PATH + "PedidosEntregados" + MEMORIA_EXTENSION;

PedidosParaEntregar::PedidosParaEntregar() {
    semaforo = new Semaforo(fileName, 0);
    memoria = new MemoriaCompartidaConcurrente<unsigned long>(memoriafileName, 'A');
    pedidosEntregados = new MemoriaCompartidaConcurrente<unsigned long>(pedidosFilename, 'A');
}

PedidosParaEntregar::~PedidosParaEntregar() {
    semaforo->eliminar();
    delete semaforo;
    delete memoria;
}

PedidosParaEntregar *PedidosParaEntregar::getInstance() {
    if (instance == NULL)
        instance = new PedidosParaEntregar();
    return instance;
}

void PedidosParaEntregar::destroy() {
    if (instance != NULL) {
        delete (instance);
        instance = NULL;
    }
}

int PedidosParaEntregar::esperarNuevoPedidoParaEntregar() {
    return semaforo->p();
}

int PedidosParaEntregar::nuevoPedidoListo() {
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

bool PedidosParaEntregar::tomarPedidoParaEntregar() {
    if (memoria->tomarLockManualmente()) {
        bool tomado = false;

        unsigned long cantDePedidos = memoria->leerInseguro();
        if (cantDePedidos > 0) {
            tomado = true;
            cantDePedidos--;
            memoria->escribirInseguro(cantDePedidos);

            unsigned long entregados = pedidosEntregados->leer();
            entregados++;
            pedidosEntregados->escribir(entregados);
        }

        memoria->liberarLockManualmente();
        return tomado;
    }
    return false;
}

unsigned long PedidosParaEntregar::cantidadDePedidosEntregados() {
    return pedidosEntregados->leer();
}