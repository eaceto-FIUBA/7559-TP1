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
    log(logDEBUG, "Esperando nuevo pedido...");
    if (PedidosPorAtender::getInstance()->esperarNuevoPedido() != 0) {
        this->log(logERROR, "ERROR AL ESPERAR NUEVO PEDIDO. - " + to_string(errno));
        cout << ">>>>>>>> FATAL ERROR: " << strerror(errno) << " <<<<<<<<" << endl;
        assert(false); // error al realizar la espera!
    }

    // 8. Cuando la cantidad pedidos que aun no fueron tomados ́
    // por las cocineras es mayor al doble de la cantidad de cocineras,
    // las recepcionistas dejan de atender los tel ́efonos hasta tant
    // o se procese al menos un pedido de los pendientes.


    Pedido *p = PedidosPorAtender::getInstance()->tomarNuevoPedido();
    this->log(logDEBUG, "Tomando nuevo pedido numero " + p->numero);
    PedidosParaCocinar::getInstance()->ingresarPedidoACocinar(*p);
    this->log(logDEBUG, "Nuevo pedido ingresado en cocina.");

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

void Recepcionista::destruirRecursos() {
    this->log(logDEBUG, "Recepcionista. Fin del trabajo.");

    // El logges es destruido por la clase Proceso

    PedidosPorAtender::getInstance()->destroy();
    PedidosParaCocinar::getInstance()->destroy();
}
