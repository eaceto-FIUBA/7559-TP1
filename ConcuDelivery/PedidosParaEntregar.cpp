
#include "PedidosParaEntregar.h"

#include "Constantes.h"
#include <assert.h>
#include "Logger.h"

PedidosParaEntregar *PedidosParaEntregar::instance = NULL;
const string PedidosParaEntregar::fileName = SEMAFOROS_PATH + FIFO_A_ENTREGAR + SEMAFOROS_EXTENSION;
const string PedidosParaEntregar::memoriafileName = MEMORIA_PATH + FIFO_A_ENTREGAR + MEMORIA_EXTENSION;
const string PedidosParaEntregar::pedidosFilename = MEMORIA_PATH + "PedidosEntregados" + MEMORIA_EXTENSION;
const string PedidosParaEntregar::aEntregarFileName = MEMORIA_PATH + FIFO_A_ENTREGAR + FIFO_EXTENSION;

PedidosParaEntregar::PedidosParaEntregar() {
    semaforo = new Semaforo(fileName, 0);

    semaforoEntregados = new Semaforo(SEMAFOROS_PATH + "PedidosEntregados" + SEMAFOROS_EXTENSION, 'A');

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

    Logger::getInstance()->log(logDEBUG, " [ PedidosParaEntregar ] \t\t esperando lock escritura... ");
	if (memoria->tomarLockManualmente()) {
		unsigned long cantidad = memoria->leerInseguro();
		cantidad++;
		memoria->escribirInseguro(cantidad);

		ssize_t bytesEscritos = fifoEscPedidosAEntregar->escribir( static_cast< void* >(&p), sizeof(p) ) ;
		assert(bytesEscritos - sizeof(Pedido) == 0);

        memoria->liberarLockManualmente();
        return semaforo->v();
	}

    Logger::getInstance()->log(logERROR, " [ PedidosParaEntregar ] \t\t FALLO EL LOCK ESCRITURA");
    return -1;
}

Pedido* PedidosParaEntregar::tomarPedidoParaEntregar() {

    Logger::getInstance()->log(logDEBUG, " [ PedidosParaEntregar ] \t\t tomando lock manualmente... ");
	if (memoria->tomarLockManualmente()) {
        unsigned long cantidad = memoria->leerInseguro();
        Logger::getInstance()->log(logDEBUG, " [ PedidosParaEntregar] \t\t cantidad " + to_string(cantidad));
        if (cantidad == 0) {
            Logger::getInstance()->log(logDEBUG, " [ PedidosParaEntregar ] \t\t sin pedido luego de tomar lock ");
            memoria->liberarLockManualmente();
            return NULL;
        }

        cantidad--;

        Pedido* p = new Pedido();

        ssize_t bytesLeidos = fifoLecPedidosAEntregar->leer( static_cast< void* >(p), sizeof(*p) ) ;
        assert(bytesLeidos - sizeof(Pedido) == 0);

        Logger::getInstance()->log(logDEBUG,
                                   " [ PedidosParaEntregar ] \t\t nuevo pedido tomado: " + to_string(p->numero));

        memoria->escribirInseguro(cantidad);

        memoria->liberarLockManualmente();

        return p;
	}
    Logger::getInstance()->log(logERROR, " [ PedidosParaEntregar ] \t\t FALLO EL LOCK ");
	return NULL;
}

int PedidosParaEntregar::marcarPedidoComoEntregado(Pedido &p) {
    if (pedidosEntregados->tomarLockManualmente()) {
        unsigned long cant = pedidosEntregados->leerInseguro();
        cant++;
        pedidosEntregados->escribirInseguro(cant);
        pedidosEntregados->liberarLockManualmente();
        semaforoEntregados->v();
        return 0;
    }
    return -1;
}



unsigned long PedidosParaEntregar::cantidadDePedidosEntregados() {
    return pedidosEntregados->leer();
}
