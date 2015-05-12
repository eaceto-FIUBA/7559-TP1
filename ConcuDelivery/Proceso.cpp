//
// Created by Kimi on 10/04/15.
//

#include "Proceso.h"

bool Proceso::seguirCorriendo() {
    return sigint_handler.getGracefulQuit() == 0 && estadoProceso == CORRIENDO;
}

Proceso::EstadoProceso Proceso::getEstado() {
    return estadoProceso;
}

void Proceso::correr() {
    estadoProceso = CORRIENDO;

    while (seguirCorriendo()) {
        realizarTarea();
    }

    this->log(logDEBUG, "Se ha detenido el proceso " + to_string(getpid()));
}

Proceso::Proceso() {
}

unsigned long Proceso::getID() {
    return this->id;
}

pid_t Proceso::iniciar(unsigned long id) {
    Logger* log = Logger::getInstance();
    log->log(logDEBUG, "Creando proceso " + to_string(id));

    pid_t idProceso = fork();
    if (idProceso == 0) {
        this->id = id;

        idProceso = getpid(); // el proceso hijo obtiene su pid
        
        this->inicializarProceso(id);

        this->log(logDEBUG, "Proceso " + to_string(this->id) + " creado con PID: " + to_string(idProceso));

        this->log(logDEBUG, "Registrando SignalHandler");

        SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

        this->log(logDEBUG, "Corriendo");
        this->correr();

        this->log(logDEBUG, "Proceso detenido. Saliendo...");

        SignalHandler::getInstance()->removerHandler(SIGINT);
        SignalHandler::destruir();

        this->destruirRecursos();

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

void Proceso::log(LogLevel level, string message) {
    Logger::getInstance()->log(level, "[ " + this->nombre() + " ]\t" + message);
}