/*
 * PedidosParaHornear.cpp
 *
 *  Created on: May 9, 2015
 *      Author: giovanni
 */

#include "PedidosParaHornear.h"
#include <assert.h>
#include "Constantes.h"
#include "Logger.h"

PedidosParaHornear *PedidosParaHornear::instance = NULL;
const string PedidosParaHornear::fileName = SEMAFOROS_PATH + FIFO_A_HORNEAR + SEMAFOROS_EXTENSION;
const string PedidosParaHornear::memoriafileName = MEMORIA_PATH + FIFO_A_HORNEAR + MEMORIA_EXTENSION;
const string PedidosParaHornear::aHornearFileName = MEMORIA_PATH + FIFO_A_HORNEAR + FIFO_EXTENSION;

PedidosParaHornear::PedidosParaHornear() {

    system(("touch " + fileName).c_str());

    semaforo = new Semaforo(fileName, 0);
    memoria = new MemoriaCompartidaConcurrente<unsigned long>(memoriafileName, 'A');
    cant_hornos = 0;
    memoria->escribir(0);

    fifoLecPedidosAHornear =  new FifoLectura(aHornearFileName);
    fifoEscPedidosAHornear = new FifoEscritura(aHornearFileName);
}

PedidosParaHornear::~PedidosParaHornear() {
    semaforo->eliminar();
    delete semaforo;
    delete memoria;

    fifoEscPedidosAHornear->eliminar();

    delete fifoLecPedidosAHornear;
    delete fifoEscPedidosAHornear;
}


void PedidosParaHornear::setCantHornos(const unsigned int cant_hornos){
	this->cant_hornos = cant_hornos;
}

PedidosParaHornear *PedidosParaHornear::getInstance() {
    if (instance == NULL)
        instance = new PedidosParaHornear();
    return instance;
}

void PedidosParaHornear::destroy() {
    if (instance != NULL) {
        delete (instance);
        instance = NULL;
    }
}

void PedidosParaHornear::inicializarParaEscribir() {
    fifoEscPedidosAHornear->abrir();
}

void PedidosParaHornear::inicializarParaLeer() {
    fifoLecPedidosAHornear->abrir();
}

void PedidosParaHornear::finalizarParaEscribir() {
    fifoEscPedidosAHornear->cerrar();
}

void PedidosParaHornear::finalizarParaLeer() {
    fifoLecPedidosAHornear->cerrar();
}

int PedidosParaHornear::esperarNuevoPedido() {
    return semaforo->p();
}

int PedidosParaHornear::hornearPedido(Pedido &p) {
	
    Logger::getInstance()->log(logDEBUG, " [ PedidosParaHornear ] \t\t esperando lock escritura... ");
	if (memoria->tomarLockManualmente()) {

		unsigned long cantidad = memoria->leerInseguro();

		/** la cocinera se debe asegurar que haya un horno libre al ingresar la pizza*/
		if(cantidad < cant_hornos )
		{
			cantidad++;
			memoria->escribirInseguro(cantidad);

			Logger::getInstance()->log(logDEBUG,  " [ PedidosParaHornear ] \t\t pedidos en los hornos " + to_string(cantidad));

			ssize_t bytesEscritos = fifoEscPedidosAHornear->escribir( static_cast< void* >(&p), sizeof(p) ) ;
			assert(bytesEscritos - sizeof(Pedido) == 0);

			memoria->liberarLockManualmente();
			return semaforo->v();
		}
		else{
			memoria->liberarLockManualmente();
			Logger::getInstance()->log(logDEBUG,  " [ PedidosParaHornear ] \t\t no se pudo ingresar el pedido. Hornos ocupados.");
			return hornearPedido(p);
		}
	}

    Logger::getInstance()->log(logERROR, " [ PedidosParaHornear ] \t\t FALLO EL LOCK ESCRITURA");
    return -1;
}

Pedido* PedidosParaHornear::tomarNuevoPedido() {

    Logger::getInstance()->log(logDEBUG, " [ PedidosParaHornear ] \t\t tomando lock manualmente... ");
	if (memoria->tomarLockManualmente()) {
        unsigned long cantidad = memoria->leerInseguro();

        Logger::getInstance()->log(logDEBUG, " [ PedidosParaHornear ] \t\t cantidad " + to_string(cantidad));

        if (cantidad == 0) {
            Logger::getInstance()->log(logDEBUG, " [ PedidosParaHornear ] \t\t sin pedido luego de tomar lock ");
            memoria->liberarLockManualmente();
            return NULL;
        }

        cantidad--;

        Pedido* p = new Pedido();

        ssize_t bytesLeidos = fifoLecPedidosAHornear->leer( static_cast< void* >(p), sizeof(*p) ) ;
        assert(bytesLeidos - sizeof(Pedido) == 0);

        Logger::getInstance()->log(logDEBUG, " [ PedidosParaHornear] \t\t nuevo pedido tomado: " + to_string(p->numero));

        memoria->escribirInseguro(cantidad);

        memoria->liberarLockManualmente();

        return p;
	}
    
    Logger::getInstance()->log(logERROR, " [ PedidosParaHornear] \t\t FALLO EL LOCK ");
	return NULL;
}

