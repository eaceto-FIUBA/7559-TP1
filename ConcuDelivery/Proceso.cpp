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
    parar();
}

Proceso::Proceso() {
    idProceso = 0;
    estadoProceso = INICIALIZADO;
}

pid_t Proceso::iniciar() {
    if (idProceso != 0) return false;
    if (estadoProceso == CORRIENDO) return false;

    idProceso = fork();
    cout << "idProcess " << to_string(idProceso) << endl;
    if (idProceso == 0) {
        idProceso = getpid(); // el proceso hijo obtiene su pid

        cout << "Proceso iniciado. " << this->descripcion() << endl;

        SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

        // Poner al proceso a correr
        correr();

        //TODO eliminar los recursos del proceso

        //Termino el proeso
        exit(0);
    }
    return idProceso;
}

bool Proceso::parar() {
    SignalHandler::getInstance()->removerHandler(SIGINT);
    estadoProceso = PARADO;
    return true;
};

string Proceso::descripcion() { return this->nombre() + "[" + to_string(getpid()) + "]"; }

Proceso::~Proceso() {
    SignalHandler::destruir();
}