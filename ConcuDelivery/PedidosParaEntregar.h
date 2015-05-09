//
// Created by kimi on 09/05/15.
//

#ifndef CONCUDELIVERY_PEDIDOSPARAENTREGAR_H
#define CONCUDELIVERY_PEDIDOSPARAENTREGAR_H

#include "Semaforo.h"
#include "MemoriaCompartidaConcurrente.h"
#include <string>

using namespace std;

class PedidosParaEntregar {
private:
    Semaforo *semaforo;
    MemoriaCompartidaConcurrente<unsigned long> *memoria;
    MemoriaCompartidaConcurrente<unsigned long> *pedidosEntregados;

    static PedidosParaEntregar *instance;
    static const string fileName;
    static const string memoriafileName;
    static const string pedidosFilename;

    PedidosParaEntregar();

    ~PedidosParaEntregar();

public:
    static PedidosParaEntregar *getInstance();

    static void destroy();

    int esperarNuevoPedidoParaEntregar();  // ejecutado solo por las cadetas
    int nuevoPedidoListo(); // ejecutado solo por la cocinera / horno
    bool tomarPedidoParaEntregar();   // true si pudo tomar un nuevo pedido para entregar

    unsigned long cantidadDePedidosEntregados();

};


#endif //CONCUDELIVERY_PEDIDOSPARAENTREGAR_H
