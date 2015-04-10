//
// Created by Kimi on 10/04/15.
//

#include "Proceso.h"

bool Proceso::seguirCorriendo() {
    return sigint_handler.getGracefulQuit() == 0 && estadoProceso == CORRIENDO;
}

void Proceso::correr() {
    estadoProceso = CORRIENDO;
    while (seguirCorriendo()) {
        realizarTarea();
    }
    estadoProceso = PARADO;
}

Proceso::Proceso() {
    estadoProceso = INICIALIZADO;
}

void Proceso::iniciar() {
    if (estadoProceso == CORRIENDO) return;

    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    correr();
}

void Proceso::parar() {
    if (estadoProceso == PARADO) return;

    SignalHandler::getInstance()->removerHandler(SIGINT);
    estadoProceso = PARADO;
};

string Proceso::descripcion() {
    return this->nombre() + "[" + to_string(getpid()) + "]";
}

Proceso::~Proceso() {
    SignalHandler::destruir();
}