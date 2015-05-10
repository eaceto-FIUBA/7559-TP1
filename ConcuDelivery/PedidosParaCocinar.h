//
// Created by kimi on 09/05/15.
//

#ifndef CONCUDELIVERY_PEDIDOSPARACOCINAR_H
#define CONCUDELIVERY_PEDIDOSPARACOCINAR_H

#include "Semaforo.h"
//#include "MemoriaCompartidaConcurrente.h"
#include "FifoEscritura.h"
#include "FifoLectura.h"
#include "Pedido.h"

#include <string>

using namespace std;

class PedidosParaCocinar {


private:
    Semaforo *semaforo;
    //MemoriaCompartidaConcurrente<unsigned long> *memoria;

    /** Lectura y Escritura para los Pedidos A Cocinar **/
    FifoLectura *fifoLecPedidosACocinar;
    FifoEscritura *fifoEscPedidosACocinar;

    static PedidosParaCocinar *instance;
    static const string fileName;
    static const string memoriafileName;

    /** fifo files **/
    static const string aCocinarFileName;

    PedidosParaCocinar();

    ~PedidosParaCocinar();

public:
    static PedidosParaCocinar *getInstance();

    static void destroy();

    int esperarPedidoACocinar();  // ejecutado solo por cocinera
    int ingresarPedidoACocinar(Pedido &p); // ejecutado solo por recepcionista
    Pedido* tomarPedidoACocinar();   // true si pudo tomar un nuevo pedido para cocinar

//    unsigned long cantidadDePedidosParaCocinar();

};


#endif //CONCUDELIVERY_PEDIDOSPARACOCINAR_H
