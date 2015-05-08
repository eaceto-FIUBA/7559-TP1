//
// Created by Kimi on 10/04/15.
//

#include "Recepcionista.h"

Recepcionista::Recepcionista() {

}

Recepcionista::~Recepcionista() {

}

void Recepcionista::realizarTarea() {
    Logger::getInstance()->log(logDEBUG, "Esperando nuevo pedido...");
    if (PedidosPorAtender::getInstance()->esperarNuevoPedido() != 0) {
        Logger::getInstance()->log(logERROR, "ERROR AL ESPERAR NUEVO PEDIDO");
        assert(false); // error al realizar la espera!
    }

    if (PedidosPorAtender::getInstance()->tomarNuevoPedido()) {
        Logger::getInstance()->log(logDEBUG, "recepcionando nuevo pedido...");

    }
}