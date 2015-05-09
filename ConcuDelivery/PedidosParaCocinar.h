//
// Created by kimi on 09/05/15.
//

#ifndef CONCUDELIVERY_PEDIDOSPARACOCINAR_H
#define CONCUDELIVERY_PEDIDOSPARACOCINAR_H

#include "Semaforo.h"
#include "MemoriaCompartidaConcurrente.h"
#include <string>

using namespace std;

class PedidosParaCocinar {


private:
    Semaforo *semaforo;
    MemoriaCompartidaConcurrente<unsigned long> *memoria;

    static PedidosParaCocinar *instance;
    static const string fileName;
    static const string memoriafileName;

    PedidosParaCocinar();

    ~PedidosParaCocinar();

public:
    static PedidosParaCocinar *getInstance();

    static void destroy();

    int esperarNuevoPedido();  // ejecutado solo por cocinera
    int ingresarNuevoPedido(); // ejecutado solo por recepcionista
    bool tomarNuevoPedido();   // true si pudo tomar un nuevo pedido para cocinar


};


#endif //CONCUDELIVERY_PEDIDOSPARACOCINAR_H
