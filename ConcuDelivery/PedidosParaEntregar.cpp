
#include "PedidosParaEntregar.h"
#include "Constantes.h"
#include <assert.h>

PedidosParaEntregar *PedidosParaEntregar::instance = NULL;
const string PedidosParaEntregar::fileName = SEMAFOROS_PATH + FIFO_A_ENTREGAR + SEMAFOROS_EXTENSION;
const string PedidosParaEntregar::memoriafileName = MEMORIA_PATH + FIFO_A_ENTREGAR + MEMORIA_EXTENSION;
const string PedidosParaEntregar::pedidosFilename = MEMORIA_PATH + "PedidosEntregados" + MEMORIA_EXTENSION;
const string PedidosParaEntregar::aEntregarFileName = MEMORIA_PATH + FIFO_A_ENTREGAR + FIFO_EXTENSION;

PedidosParaEntregar::PedidosParaEntregar() {
    semaforo = new Semaforo(fileName, 0);
    memoria = new MemoriaCompartidaConcurrente<unsigned long>(memoriafileName, 'A');
    memoria->escribir(0);

    pedidosEntregados = new MemoriaCompartidaConcurrente<unsigned long>(pedidosFilename, 'A');
    pedidosEntregados->escribir(0);

    fifoLecPedidosAEntregar =  new FifoLectura(aEntregarFileName);
    fifoEscPedidosAEntregar = new FifoEscritura(aEntregarFileName);
}

PedidosParaEntregar::~PedidosParaEntregar() {
    semaforo->eliminar();
    delete semaforo;
    delete memoria;
    delete pedidosEntregados;

    fifoEscPedidosAEntregar->eliminar();

    delete fifoLecPedidosAEntregar;
    delete fifoEscPedidosAEntregar;
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
	if (memoria->tomarLockManualmente()) {
		unsigned long cantidad = memoria->leerInseguro();
		cantidad++;
		memoria->escribirInseguro(cantidad);

		ssize_t bytesEscritos = fifoEscPedidosAEntregar->escribir( static_cast< void* >(&p), sizeof(p) ) ;
		assert(bytesEscritos - sizeof(Pedido) == 0);

        memoria->liberarLockManualmente();
        return semaforo->v();
	}
    return -1;
}

Pedido* PedidosParaEntregar::tomarPedidoParaEntregar() {
	if (memoria->tomarLockManualmente()) {
        unsigned long cantidad = memoria->leerInseguro();

        if (cantidad == 0) {
            memoria->liberarLockManualmente();
            return NULL;
        }

        cantidad--;

        Pedido* p = new Pedido();

        ssize_t bytesLeidos = fifoLecPedidosAEntregar->leer( static_cast< void* >(p), sizeof(*p) ) ;
        assert(bytesLeidos - sizeof(Pedido) == 0);
        memoria->escribirInseguro(cantidad);

        memoria->liberarLockManualmente();

        return p;
	}
	return NULL;
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
