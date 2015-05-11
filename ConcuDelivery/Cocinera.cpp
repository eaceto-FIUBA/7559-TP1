
#include "Cocinera.h"

#include "PedidosParaCocinar.h"
#include "PedidosParaHornear.h"

Cocinera::Cocinera() : Proceso() {

}

Cocinera::~Cocinera() {

}

void Cocinera::realizarTarea() {

    // Gio: considerar enunciado del TP:
    // 7. Para preparar un pedido, la cocinera debe amasar la mezcla, esperar un horno libre y poner a
    //    cocinar la pizza.

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

//    /** Tomar pedido para cocinar **/
//    if (PedidosParaCocinar::getInstance()->tomarPedidoACocinar()) {
//    	//TODO
//        this->log(logDEBUG, "Tomando Nuevo pedido para cocinar.");
//
//        /** Esperar PedidosParaHornear disponible**/
//        //TODO Lock si no hay hornos disponibles
//
//        /** AgregarPedido para Hornear **/
//        PedidosParaHornear::getInstance()->hornearPedido();
//        this->log(logDEBUG, "Nuevo pedido listo para entregar.");
//    }

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

    PedidosParaCocinar::getInstance()->destroy();
    PedidosParaHornear::getInstance()->destroy();
}
