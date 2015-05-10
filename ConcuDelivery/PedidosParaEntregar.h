//
// Created by kimi on 09/05/15.
//

#ifndef CONCUDELIVERY_PEDIDOSPARAENTREGAR_H
#define CONCUDELIVERY_PEDIDOSPARAENTREGAR_H


#include "Semaforo.h"
#include "MemoriaCompartidaConcurrente.h"
#include <string>
#include "Pedido.h"

#include "FifoEscritura.h"
#include "FifoLectura.h"

using namespace std;

class PedidosParaEntregar {
private:
    Semaforo *semaforo;
//    MemoriaCompartidaConcurrente<unsigned long> *memoria;
    MemoriaCompartidaConcurrente<unsigned long> *pedidosEntregados;

    /** Lectura y Escritura para los Pedidos A Entregar **/
    FifoLectura *fifoLecPedidosAEntregar;
    FifoEscritura *fifoEscPedidosAEntregar;

    static PedidosParaEntregar *instance;
    static const string fileName;
    static const string memoriafileName;
    static const string pedidosFilename;

    /** fifo files **/
    static const string aEntregarFileName;

    PedidosParaEntregar();

    ~PedidosParaEntregar();

public:
    static PedidosParaEntregar *getInstance();

    static void destroy();

    void inicializarParaEscribir();

    void inicializarParaLeer();

    void finalizarParaEscribir();

    void finalizarParaLeer();

    int esperarNuevoPedidoParaEntregar();  // ejecutado solo por las cadetas
    int nuevoPedidoListo(Pedido &p); // ejecutado solo por la cocinera / horno
    Pedido* tomarPedidoParaEntregar();   // true si pudo tomar un nuevo pedido para entregar

    void marcarPedidoComoEntregado(Pedido &p);

    unsigned long cantidadDePedidosEntregados();

};


#endif //CONCUDELIVERY_PEDIDOSPARAENTREGAR_H
