//
// Created by kimi on 08/05/15.
//

#ifndef CONCUDELIVERY_PEDIDOSPORATENDER_H
#define CONCUDELIVERY_PEDIDOSPORATENDER_H

#include "Semaforo.h"
#include "MemoriaCompartidaConcurrente.h"
#include <string>
#include "Pedido.h"

#include "FifoEscritura.h"
#include "FifoLectura.h"


using namespace std;

class PedidosPorAtender {

private:
    Semaforo *semaforo;
    MemoriaCompartidaConcurrente<unsigned long> *memoria;
    FifoLectura *fifoLectura;
    FifoEscritura *fifoEscritura;


    static PedidosPorAtender *instance;
    static const string fileName;
    static const string memoriafileName;

    PedidosPorAtender();

    ~PedidosPorAtender();

public:
    static PedidosPorAtender *getInstance();

    static void destroy();

    int esperarNuevoPedido();  // ejecutado solo por recepcionistas
    int ingresarNuevoPedido(Pedido p); // ejecutado solo por la simulacion
    bool tomarNuevoPedido();   // true si pudo tomar un nuevo pedido

};


#endif //CONCUDELIVERY_PEDIDOSPORATENDER_H
