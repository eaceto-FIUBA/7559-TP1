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
//    MemoriaCompartidaConcurrente<unsigned long> *memoria;

    /** Lectura y Escritura para los Pedidos A Atender **/
    FifoLectura *fifoLecPedidosAAtender;
    FifoEscritura *fifoEscPedidosAAtender;

    static PedidosPorAtender *instance;
    static const string fileName;
//    static const string memoriafileName;

    /** fifo files **/
    static const string aAtenderFileName;

    PedidosPorAtender();

    ~PedidosPorAtender();

public:
    static PedidosPorAtender *getInstance();

    static void destroy();

    int esperarNuevoPedido();  // ejecutado solo por recepcionistas
    int ingresarNuevoPedido(Pedido &p); // ejecutado solo por la simulacion
    Pedido* tomarNuevoPedido();   // true si pudo tomar un nuevo pedido

};


#endif //CONCUDELIVERY_PEDIDOSPORATENDER_H
