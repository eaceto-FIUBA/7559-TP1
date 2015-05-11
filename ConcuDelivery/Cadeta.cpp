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
    log(logDEBUG, "\t\tEsperando nuevo pedido para entregar...");
    if (PedidosParaEntregar::getInstance()->esperarNuevoPedidoParaEntregar() != 0) {

        if (errno == EINTR || getGracefulQuit() != 0 || this->getEstado() != Proceso::CORRIENDO) {
            return;
        }


        this->log(logERROR, "\tERROR AL ESPERAR NUEVO PEDIDO PARA ENTREGAR. - " + to_string(errno));
        cout << ">>>>>>>> FATAL ERROR: " << strerror(errno) << " <<<<<<<<" << endl;
        assert(false); // error al realizar la espera!
    }

    Pedido *p = PedidosParaEntregar::getInstance()->tomarPedidoParaEntregar();
    if (p != NULL) {
        this->log(logDEBUG, "\t\t{Pedido " + to_string(p->numero) + "} Tomando pedido para entregar y cobrar");
        int res = PedidosParaEntregar::getInstance()->marcarPedidoComoEntregado(*p);
        if (res >= 0) {
            this->log(logDEBUG, "\t\t{Pedido " + to_string(p->numero) + "} Entregado");
        }
        else {
            this->log(logERROR, "\t\t{Pedido " + to_string(p->numero) + "} ERROR al entregar pedido");
        }
    }
    else {
        this->log(logDEBUG, "\t\t{Pedido NULL +}");
    }
}

void Cadeta::inicializarProceso(unsigned long id) {
	PedidosParaEntregar::getInstance()->inicializarParaLeer();
}

string Cadeta::nombre() {
    return "Cadeta " + to_string(getID());
}


void Cadeta::destruirRecursos() {
    this->log(logDEBUG, "\t\tCadeta. Fin del trabajo.");

    PedidosParaEntregar::getInstance()->finalizarParaLeer();

    // PedidosParaEntregar::getInstance()->destroy();
}
