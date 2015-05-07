//
// Created by Kimi on 10/04/15.
//

#include "Recepcionista.h"

Recepcionista::Recepcionista() {
    pedidosPorAtender = new MemoriaCompartidaConcurrente<unsigned long>(".pedidosSiendoAtendidos.sh",'A');
}

Recepcionista::~Recepcionista() {
    delete pedidosPorAtender;
}

bool Recepcionista::puedoProcesarNuevoPedido() {
    return true; // false si la cocina no puede aceptar mas pedidos
}

// SIMULACION: Tiempo de atención de pedido
void Recepcionista::procesarNuevoPedido() {
    Logger::getInstance()->log(logDEBUG,"Atendiendo nuevo pedido...");
    long time_in_usec = std::rand() % 350; //350msec max
    usleep(time_in_usec * 1000);
}

void Recepcionista::realizarTarea() {
    cout << "TAREA Recepcionista" << endl;

    bool nuevoPedido = false;

    if (pedidosPorAtender->tomarLockManualmente()) {
        unsigned long pedidos = pedidosPorAtender->leerInseguro();
        if (pedidos > 0 && puedoProcesarNuevoPedido()) {
            pedidosPorAtender->escribirInseguro(pedidos -1);
            pedidosPorAtender->liberarLockManualmente();
            nuevoPedido = true;
        }
    }

    if (nuevoPedido) {
        procesarNuevoPedido();
    }
}