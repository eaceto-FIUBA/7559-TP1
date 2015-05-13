
#include "Cocinera.h"

#include "PedidosParaCocinar.h"
#include "PedidosParaHornear.h"

Cocinera::Cocinera() : Proceso() {

}

Cocinera::~Cocinera() {

}

void Cocinera::realizarTarea() {

    /** Esperar pedido para cocinar **/
    log(logDEBUG, "\t\tEsperando nuevo pedido para cocinar...");
    if (PedidosParaCocinar::getInstance()->esperarPedidoACocinar() != 0) {

        if (errno == EINTR || getGracefulQuit() != 0 || this->getEstado() != Proceso::CORRIENDO) {
            return;
        }

        this->log(logERROR, "\t\tERROR AL ESPERAR NUEVO PEDIDO PARA COCINAR. - " + to_string(errno));
        cout << ">>>>>>>> FATAL ERROR: " << strerror(errno) << " <<<<<<<<" << endl;
        assert(false); // error al realizar la espera!
    }

    /** Tomar pedido para cocinar **/
    Pedido *p = PedidosParaCocinar::getInstance()->tomarPedidoACocinar();
    if (p != NULL) {
        this->log(logDEBUG, "\t\t{Pedido " + to_string(p->numero) + "} tomado para cocinar");
        int res = PedidosParaHornear::getInstance()->hornearPedido(*p);
        if (res >= 0) {
            this->log(logDEBUG, "\t\t{Pedido " + to_string(p->numero) + "} ingresado al horno");
        }
        else {
            this->log(logERROR, "\t\t{Pedido " + to_string(p->numero) + "} ERROR al ingresarlo al horno");
        }
    }
    else {
        this->log(logDEBUG, "\t\t{Pedido NULL +}");
    }

}

void Cocinera::inicializarProceso(unsigned long id) {
    PedidosParaCocinar::getInstance()->inicializarParaLeer();
    PedidosParaHornear::getInstance()->inicializarParaEscribir();
}

string Cocinera::nombre() {
    return "Cocinera " + to_string(getID());
}

void Cocinera::destruirRecursos() {

    this->log(logDEBUG, "\t\tCocinera. Fin del trabajo.");

    PedidosParaCocinar::getInstance()->finalizarParaLeer();
    PedidosParaHornear::getInstance()->finalizarParaEscribir();

}
