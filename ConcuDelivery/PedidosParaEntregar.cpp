
#include "PedidosParaEntregar.h"
#include "Constantes.h"
#include <assert.h>

PedidosParaEntregar *PedidosParaEntregar::instance = NULL;
const string PedidosParaEntregar::fileName = SEMAFOROS_PATH + "PedidosPorEntregar" + SEMAFOROS_EXTENSION;
//const string PedidosParaEntregar::memoriafileName = MEMORIA_PATH + "PedidosPorEntregar" + MEMORIA_EXTENSION;
const string PedidosParaEntregar::pedidosFilename = MEMORIA_PATH + "PedidosEntregados" + MEMORIA_EXTENSION;
const string PedidosParaEntregar::aEntregarFileName = MEMORIA_PATH + FIFO_A_ENTREGAR + FIFO_EXTENSION;

PedidosParaEntregar::PedidosParaEntregar() {
    semaforo = new Semaforo(fileName, 0);
//    memoria = new MemoriaCompartidaConcurrente<unsigned long>(memoriafileName, 'A');

    pedidosEntregados = new MemoriaCompartidaConcurrente<unsigned long>(pedidosFilename, 'A');

    fifoLecPedidosAEntregar =  new FifoLectura(aEntregarFileName);
    fifoEscPedidosAEntregar = new FifoEscritura(aEntregarFileName);
}

PedidosParaEntregar::~PedidosParaEntregar() {
    semaforo->eliminar();
    delete semaforo;
    delete pedidosEntregados;

    fifoLecPedidosAEntregar->cerrar();
    fifoEscPedidosAEntregar->cerrar();

    fifoEscPedidosAEntregar->eliminar();
}

PedidosParaEntregar *PedidosParaEntregar::getInstance() {
    if (instance == NULL)
        instance = new PedidosParaEntregar();
    return instance;
}

void PedidosParaEntregar::inicializarParaEscribir() {
    fifoEscPedidosAEntregar->abrir();
}

void PedidosParaEntregar::inicializarParaLeer() {
    fifoLecPedidosAEntregar->abrir();
}

void PedidosParaEntregar::finalizarParaEscribir() {
    fifoEscPedidosAEntregar->cerrar();
}

void PedidosParaEntregar::finalizarParaLeer() {
    fifoLecPedidosAEntregar->cerrar();
}

void PedidosParaEntregar::destroy() {
    if (instance != NULL) {
        delete (instance);
        instance = NULL;
    }
}

int PedidosParaEntregar::esperarNuevoPedidoParaEntregar() {
    return semaforo->p();
}

int PedidosParaEntregar::nuevoPedidoListo(Pedido &p) {
    // incrementar cantidad de pedidos

    fifoEscPedidosAEntregar->abrir();
	ssize_t bytesEscritos = fifoEscPedidosAEntregar->escribir( static_cast< void* >(&p), sizeof(p) ) ;
	assert(bytesEscritos - sizeof(Pedido) == 0);


    return semaforo->v();
}

Pedido* PedidosParaEntregar::tomarPedidoParaEntregar() {

	Pedido* p = new Pedido();
    fifoLecPedidosAEntregar->abrir();
	ssize_t bytesLeidos = fifoLecPedidosAEntregar->leer( static_cast< void* >(p), sizeof(*p) ) ;
	assert(bytesLeidos - sizeof(Pedido) == 0);

	//TODO Facturar pedido en memoria compartida

    return p;

}

void PedidosParaEntregar::marcarPedidoComoEntregado(Pedido &p) {
    if (pedidosEntregados->tomarLockManualmente()) {
        unsigned long cant = pedidosEntregados->leerInseguro();
        cant++;
        pedidosEntregados->escribirInseguro(cant);
        pedidosEntregados->liberarLockManualmente();
    }
}

unsigned long PedidosParaEntregar::cantidadDePedidosEntregados() {
    return pedidosEntregados->leer();
}
