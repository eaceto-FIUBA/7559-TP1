
#include "Horno.h"

#include "PedidosParaHornear.h"
#include "PedidosParaEntregar.h"

Horno::Horno() {

}

Horno::~Horno() {

}

void Horno::realizarTarea() {

    log(logDEBUG, "\t\tEsperando pedido para hornear...");
    if (PedidosParaHornear::getInstance()->esperarNuevoPedido() != 0) {

        if (errno == EINTR || getGracefulQuit() != 0 || this->getEstado() != Proceso::CORRIENDO) {
            return;
        }

        this->log(logERROR, "\t\tERROR AL ESPERAR NUEVO PEDIDO PARA HORNEAR. - " + to_string(errno));
        cout << ">>>>>>>> FATAL ERROR: " << strerror(errno) << " <<<<<<<<" << endl;
        assert(false); // error al realizar la espera!
    }

    /** Tomar pedido para hornear **/
    Pedido *p = PedidosParaHornear::getInstance()->tomarNuevoPedido();
    if (p != NULL) {
        this->log(logDEBUG, "\t\t{Pedido " + to_string(p->numero) + "} ingresando al horno");
        int res = PedidosParaEntregar::getInstance()->nuevoPedidoListo(*p);
        if (res >= 0) {
            this->log(logDEBUG, "\t\t{Pedido " + to_string(p->numero) + "} cocinado. listo para entregar.");
        }
        else {
            this->log(logERROR, "\t\t{Pedido " + to_string(p->numero) + "} ERROR al cocinar pedido.");
        }
        delete p;
        p = NULL;
    }
    else {
        this->log(logDEBUG, "\t\t{Pedido NULL +}");
    }

}

void Horno::inicializarProceso(unsigned long id) {
	PedidosParaHornear::getInstance()->inicializarParaLeer();
    PedidosParaEntregar::getInstance()->inicializarParaEscribir();
}

string Horno::nombre() {
    return "Horno " + to_string(getID());
}

void Horno::destruirRecursos() {

    this->log(logDEBUG, "\t\tHorno. Fin del trabajo.");

    PedidosParaHornear::getInstance()->finalizarParaLeer();
    PedidosParaEntregar::getInstance()->finalizarParaEscribir();

}
