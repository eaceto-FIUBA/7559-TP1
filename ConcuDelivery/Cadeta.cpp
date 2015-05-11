//
// Created by Kimi on 10/04/15.
//

#include "Cadeta.h"

#include "PedidosParaEntregar.h"

Cadeta::Cadeta() : Proceso() {

}

Cadeta::~Cadeta() {

}

void Cadeta::realizarTarea() {

	/** Espera señal de pedido listo para entregar **/
    log(logDEBUG, "Esperando nuevo pedido para entregar...");
    if (PedidosParaEntregar::getInstance()->esperarNuevoPedidoParaEntregar() != 0) {
        this->log(logERROR, "ERROR AL ESPERAR NUEVO PEDIDO PARA ENTREGAR. - " + to_string(errno));
        cout << ">>>>>>>> FATAL ERROR: " << strerror(errno) << " <<<<<<<<" << endl;
        assert(false); // error al realizar la espera!
    }

    Pedido *p = PedidosParaEntregar::getInstance()->tomarPedidoParaEntregar();
    if (p != NULL) {
        this->log(logDEBUG, "Tomando pedido para entregar y cobrar numero " + to_string(p->numero));
        PedidosParaEntregar::getInstance()->marcarPedidoComoEntregado(*p);
    }
}

void Cadeta::inicializarProceso(unsigned long id) {
	PedidosParaEntregar::getInstance()->inicializarParaLeer();
}

string Cadeta::nombre() {
    return "Cadeta " + to_string(getID());
}


void Cadeta::destruirRecursos() {
    this->log(logDEBUG, "Cadeta. Fin del trabajo.");

    PedidosParaEntregar::getInstance()->finalizarParaLeer();

    PedidosParaEntregar::getInstance()->destroy();
}
