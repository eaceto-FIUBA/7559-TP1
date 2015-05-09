//
// Created by kimi on 08/05/15.
//

#ifndef CONCUDELIVERY_PEDIDOSPORATENDER_H
#define CONCUDELIVERY_PEDIDOSPORATENDER_H

#include "Semaforo.h"
#include "MemoriaCompartidaConcurrente.h"
#include <string>

using namespace std;

class PedidosPorAtender {

private:
    Semaforo *semaforo;
    MemoriaCompartidaConcurrente<unsigned long> *memoria;

    static PedidosPorAtender *instance;
    static const string fileName;
    static const string memoriafileName;

    PedidosPorAtender();

    ~PedidosPorAtender();

public:
    static PedidosPorAtender *getInstance();

    static void destroy();

    int esperarNuevoPedido();  // ejecutado solo por recepcionistas
    int ingresarNuevoPedido(); // ejecutado solo por la simulacion
    bool tomarNuevoPedido();   // true si pudo tomar un nuevo pedido

};


#endif //CONCUDELIVERY_PEDIDOSPORATENDER_H
