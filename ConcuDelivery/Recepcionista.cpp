//
// Created by Kimi on 10/04/15.
//

#include "Recepcionista.h"


#include "PedidosPorAtender.h"
#include "PedidosParaCocinar.h"


Recepcionista::Recepcionista(unsigned long cocinerasDisponibles) : Proceso() {
    this->cantDeCocinerasDisponibles = cocinerasDisponibles;
}

Recepcionista::~Recepcionista() {

}

void Recepcionista::realizarTarea() {
    log(logDEBUG, "\tEsperando nuevo pedido...");
    if (PedidosPorAtender::getInstance()->esperarNuevoPedido() != 0) {

        if (errno == EINTR || getGracefulQuit() != 0 || this->getEstado() != Proceso::CORRIENDO) {
            return;
        }

        this->log(logERROR, "\tERROR AL ESPERAR NUEVO PEDIDO. - " + to_string(errno));
        cout << ">>>>>>>> FATAL ERROR: " << strerror(errno) << " <<<<<<<<" << endl;
        assert(false); // error al realizar la espera!
    }

    // 8. Cuando la cantidad pedidos que aun no fueron tomados ́
    // por las cocineras es mayor al doble de la cantidad de cocineras,
    // las recepcionistas dejan de atender los tel ́efonos hasta tant
    // o se procese al menos un pedido de los pendientes.


    Pedido *p = PedidosPorAtender::getInstance()->tomarNuevoPedido();
    if (p != NULL) {
        this->log(logDEBUG, "\t{Pedido " + to_string(p->numero) + "} tomando pedido");
        int res = PedidosParaCocinar::getInstance()->ingresarPedidoACocinar(*p);
        if (res >= 0) {
            this->log(logDEBUG, "\t{Pedido " + to_string(p->numero) + "} ingresado a la cocina");
        }
        else {
            this->log(logERROR, "\t{Pedido " + to_string(p->numero) + "} ERROR al ingresar a la cocina");
        }
    }
    else {
        //this->log(logDEBUG, "\t{Pedido NULL}");
    }
//    unsigned long cantDePedidosParaCocinar =
//            PedidosParaCocinar::getInstance()->cantidadDePedidosParaCocinar();


//    if (cantDePedidosParaCocinar <= 2 * cantDeCocinerasDisponibles) {
//        if (PedidosPorAtender::getInstance()->tomarNuevoPedido()) {
//            this->log(logDEBUG, "Tomando Nuevo pedido.");
//            PedidosParaCocinar::getInstance()->ingresarPedidoACocinar();
//            this->log(logDEBUG, "Nuevo pedido ingresado en cocina.");
//        }
//    }
}

string Recepcionista::nombre() {
    return "Recepcionista " + to_string(getID());
}

void Recepcionista::inicializarProceso(unsigned long id) {
    PedidosPorAtender::getInstance()->inicializarParaLeer();
    PedidosParaCocinar::getInstance()->inicializarParaEscribir();
}

void Recepcionista::destruirRecursos() {
    this->log(logDEBUG, "\tRecepcionista. Fin del trabajo.");

    PedidosPorAtender::getInstance()->finalizarParaLeer();
    PedidosParaCocinar::getInstance()->finalizarParaEscribir();

    // PedidosPorAtender::getInstance()->destroy();
    // PedidosParaCocinar::getInstance()->destroy();
}
