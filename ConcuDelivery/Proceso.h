//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_PROCESO_H
#define CONCUDELIVERY_PROCESO_H

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

#include "EventHandler.h"
#include "SIGINT_Handler.h"
#include "SignalHandler.h"
#include "Logger.h"

#include "Semaforo.h"
#include "MemoriaCompartida2.h"

using namespace std;

#define kFILE_SEMAFORO_PEDIDOS_POR_ATENDER  "pedidosPorAtender.smf"
#define kFILE_SEMAFORO_PEDIDOS_RECIBIDOS  "pedidosRecibidos.smf"


class Proceso: public SIGINT_Handler {

    enum EstadoProceso {
        INICIALIZADO = 0,
        CORRIENDO = 1,
        PARADO = 2
    };

private:
    unsigned long id;
    SIGINT_Handler sigint_handler;
    EstadoProceso estadoProceso;
    void correr();
    bool seguirCorriendo();
    virtual void realizarTarea() = 0;
    virtual string nombre() = 0;

protected:
    void log(LogLevel level, string message);

public:
    Proceso();
    virtual ~Proceso();

    pid_t iniciar(unsigned long id);
    static bool parar(pid_t pid);

    virtual void destruirRecursos() = 0;

    unsigned long getID();

    string descripcion();
};


#endif //CONCUDELIVERY_PROCESO_H
