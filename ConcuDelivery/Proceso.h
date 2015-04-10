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

using namespace std;

class Proceso: public EventHandler{

    enum EstadoProceso {
        INICIALIZADO,
        CORRIENDO,
        PARADO
    };

private:
    EstadoProceso estadoProceso;
    SIGINT_Handler sigint_handler;
    void correr();
    bool seguirCorriendo();
    virtual void realizarTarea() = 0;
    virtual string nombre() { return "Proceso"; }

public:
    Proceso();
    virtual ~Proceso();

    void iniciar();
    void parar();

    string descripcion();
};


#endif //CONCUDELIVERY_PROCESO_H
