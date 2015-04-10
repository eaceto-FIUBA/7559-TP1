//
// Created by Kimi on 10/04/15.
//

#include "Proceso.h"

void Proceso::run() {
    estadoProceso = CORRIENDO;
    while (sigint_handler.getGracefulQuit() == 0 && estadoProceso == CORRIENDO) {

    }
    estadoProceso = PARADO;
}

Proceso::Proceso() {
    estadoProceso = INICIALIZADO;
}

void Proceso::iniciar() {
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );
    run();
}

void Proceso::parar() {
    SignalHandler::getInstance()->removerHandler( SIGINT );
    estadoProceso = PARADO;
};

Proceso::~Proceso() {

}