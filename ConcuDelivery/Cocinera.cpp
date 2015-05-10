
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
    log(logDEBUG, "Esperando nuevo pedido para cocinar...");
    if (PedidosParaCocinar::getInstance()->esperarPedidoACocinar() != 0) {
        this->log(logERROR, "ERROR AL ESPERAR NUEVO PEDIDO PARA COCINAR. - " + to_string(errno));
        cout << ">>>>>>>> FATAL ERROR: " << strerror(errno) << " <<<<<<<<" << endl;
        assert(false); // error al realizar la espera!
    }


    Pedido *p = PedidosParaCocinar::getInstance()->tomarPedidoACocinar();
    this->log(logDEBUG, "Tomando pedido para cocinar numero " + p->numero);
    PedidosParaHornear::getInstance()->hornearPedido(*p);
    this->log(logDEBUG, "Nuevo pedido ingresado en horno.");

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

}

string Cocinera::nombre() {
    return "Cocinera " + to_string(getID());
}

void Cocinera::destruirRecursos() {

}
