//
// Created by Kimi on 10/04/15.
//

#include "Recepcionista.h"


#include "PedidosPorAtender.h"
#include "PedidosParaCocinar.h"


Recepcionista::Recepcionista() {

}

Recepcionista::~Recepcionista() {

}

void Recepcionista::realizarTarea() {
    log(logDEBUG, "Esperando nuevo pedido...");
    if (PedidosPorAtender::getInstance()->esperarNuevoPedido() != 0) {
        this->log(logERROR, "ERROR AL ESPERAR NUEVO PEDIDO. - " + to_string(errno));
        cout << ">>>>>>>> FATAL ERROR: " << strerror(errno) << " <<<<<<<<" << endl;
        assert(false); // error al realizar la espera!
    }

    if (PedidosPorAtender::getInstance()->tomarNuevoPedido()) {
        this->log(logDEBUG, "Tomando Nuevo pedido.");
        PedidosParaCocinar::getInstance()->ingresarNuevoPedido();
        this->log(logDEBUG, "Nuevo pedido ingresado en cocina.");
    }
}

string Recepcionista::nombre() {
    return "Recepcionista " + to_string(getID());
}

void Recepcionista::destruirRecursos() {
    this->log(logDEBUG, "Recepcionista. Fin del trabajo.");

    // El logges es destruido por la clase Proceso

    PedidosPorAtender::getInstance()->destroy();
    PedidosParaCocinar::getInstance()->destroy();
}