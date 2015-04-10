//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_PROCESO_H
#define CONCUDELIVERY_PROCESO_H


#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include "EventHandler.h"
#include "SIGINT_Handler.h"
#include "SignalHandler.h"

class Proceso: public EventHandler{

    enum EstadoProceso {
        INICIALIZADO,
        CORRIENDO,
        PARADO
    };

    private:
        EstadoProceso estadoProceso;
        SIGINT_Handler sigint_handler;
        virtual void run () = 0;

    public:
        Proceso();
        virtual ~Proceso();

        void iniciar();
        void parar();
};


#endif //CONCUDELIVERY_PROCESO_H
