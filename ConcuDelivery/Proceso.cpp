//
// Created by Kimi on 10/04/15.
//

#include "Proceso.h"

bool Proceso::seguirCorriendo() {
    return sigint_handler.getGracefulQuit() == 0 && estadoProceso == CORRIENDO;
}

void Proceso::correr() {
    estadoProceso = CORRIENDO;

    Logger* log = Logger::getInstance();
    while (seguirCorriendo()) {
        realizarTarea();
    }

    log->log(logDEBUG,"Se ha detenido el proceso " + to_string(getpid()));
}

Proceso::Proceso() {

}

pid_t Proceso::iniciar() {
    Logger* log = Logger::getInstance();
    log->log(logDEBUG,"Creando proceso");

    pid_t idProceso = fork();
    if (idProceso == 0) {
        idProceso = getpid(); // el proceso hijo obtiene su pid
        cout << "idProcess " << to_string(idProceso) << endl;

        log->log(logDEBUG,"Proceso creado con PID: " + to_string(idProceso));

        log->log(logDEBUG,"Registrando SignalHandler");

        SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

        log->log(logDEBUG,"Corriendo");
        this->correr();

        log->log(logDEBUG,"Proceso detenido. Saliendo...");

        SignalHandler::getInstance()->removerHandler(SIGINT);
        SignalHandler::destruir();
        Logger::destroy();

        exit ( 0 );
    }
    return idProceso;
}

bool Proceso::parar(pid_t pid) {
    return kill(pid, SIGINT) == 0;
};

string Proceso::descripcion() { return this->nombre() + "[" + (this->estadoProceso == INICIALIZADO ? "INICIALIZADO" : to_string(getpid())) + "]"; }

Proceso::~Proceso() {
    SignalHandler::destruir();
}