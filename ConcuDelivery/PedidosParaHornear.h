/*
 * PedidosParaHornear.h
 *
 *  Created on: May 9, 2015
 *      Author: giovanni
 */

#ifndef PEDIDOSPARAHORNEAR_H_
#define PEDIDOSPARAHORNEAR_H_

#include "Semaforo.h"
//#include "MemoriaCompartidaConcurrente.h"
#include "Pedido.h"
#include "FifoEscritura.h"
#include "FifoLectura.h"
#include <string>

using namespace std;

class PedidosParaHornear {
private:
    Semaforo *semaforo;
//    MemoriaCompartidaConcurrente<unsigned long> *memoria;

    /** Lectura y Escritura para los Pedidos A Hornear **/
    FifoLectura *fifoLecPedidosAHornear;
    FifoEscritura *fifoEscPedidosAHornear;

    static PedidosParaHornear *instance;
    static const string fileName;
    static const string memoriafileName;
    unsigned int cant_hornos;

    /** fifo files **/
    static const string aHornearFileName;

	PedidosParaHornear();
	~PedidosParaHornear();

public:
    static PedidosParaHornear *getInstance();

    void setCantHornos(const unsigned int cant_hornos);

    static void destroy();

    int esperarNuevoPedido();  // ejecutado solo por horno
    int hornearPedido(Pedido &p); // ejecutado solo por cocinera
    Pedido* tomarNuevoPedido();   // true si pudo tomar un nuevo pedido para hornear
};

#endif /* PEDIDOSPARAHORNEAR_H_ */
