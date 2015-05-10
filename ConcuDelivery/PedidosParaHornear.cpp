/*
 * PedidosParaHornear.cpp
 *
 *  Created on: May 9, 2015
 *      Author: giovanni
 */

#include "PedidosParaHornear.h"

#include "Constantes.h"

PedidosParaHornear *PedidosParaHornear::instance = NULL;
const string PedidosParaHornear::fileName = SEMAFOROS_PATH + "PedidosParaHornear" + SEMAFOROS_EXTENSION;
const string PedidosParaHornear::memoriafileName = MEMORIA_PATH + "PedidosParaHornear" + MEMORIA_EXTENSION;

PedidosParaHornear::PedidosParaHornear() {
    semaforo = new Semaforo(fileName, 0);
    memoria = new MemoriaCompartidaConcurrente<unsigned long>(memoriafileName, 'A');
    cant_hornos = 0;
}

PedidosParaHornear::~PedidosParaHornear() {
    semaforo->eliminar();
    delete semaforo;
    delete memoria;
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

int PedidosParaHornear::hornearPedido() {
	//TODO
	//Lock if (memoria->size() < cant_hornos)

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

bool PedidosParaHornear::tomarNuevoPedido() {
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
