#include <iostream>
#include <getopt.h>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>

// Include de objectos del modelo
#include "Recepcionista.h"
#include "Cocinera.h"
#include "Cadeta.h"
#include "Supervisora.h"
#include "Horno.h"

#include "Pedido.h"
#include "PedidosParaCocinar.h"
#include "PedidosPorAtender.h"
#include "PedidosParaEntregar.h"
#include "PedidosParaHornear.h"

using namespace std;

#define kAppVersion "0.2"

#define kDefaultRecepcionistasCount 5
#define kDefaultCadetasCount        2
#define kDefaultCocinerasCount      1
#define kDefaultHornosCount         2
#define kDefaultSimulacionCount     10

#define kCLINoAargument         0
#define kCLIRequiredArgument    1
#define kCLIOptionalArgument    2

#define kDebugModeON            true
#define kDebugModeOFF           false
#define kDefaultDebugMode       kDebugModeOFF

bool            debugMode               = kDefaultDebugMode;
long recepcionistasCount = kDefaultRecepcionistasCount;
long cadetasCount = kDefaultCadetasCount;
long hornosCount = kDefaultHornosCount;
long cocinerasCount = kDefaultCocinerasCount;

unsigned long simulacionCount = kDefaultSimulacionCount;

void print_version() {
    cout << "ConcuDelivery v" << kAppVersion << endl << endl;
    cout << "[87761]\tOpromolla, Giovanni\t\tgiopromolla@gmail.com " << endl;
    cout << "[84316]\tAceto, Ezequiel\t\tezequiel.aceto@gmail.com  " << endl;
}


void printHelp() {
    cout << endl;
    cout << "Usage: ConcuDelivery <parameter>"  << endl;
    cout << endl;
    cout << "where <parameter> may be one or more of: " << endl;
    cout << "\tr (recepcionista)\t\t[int] Number of process with type 'recepcionistas'" << endl;
    cout << "\tc (cocineras)\t\t\t[int] Number of process with type 'cocineras'" << endl;
    cout << "\to (hornos)\t\t\t[int] Number of process with type 'hornos'" << endl;
    cout << "\ta (cadetas)\t\t\t[int] Number of process with type 'cadetas'" << endl;
    cout << endl;
    cout << "ConcuDelivery --help\t\t\tdisplay help" <<endl;
    cout << "ConcuDelivery --version\t\t\tdisplay version" <<endl;
    cout << endl;
}

void printVersion() {
    print_version();
}

int setupCLI(int argc, char **argv) {
    const struct option longopts[] =
        {
                {"version",         kCLINoAargument,        NULL, 'v'},
                {"help",            kCLINoAargument,        NULL, 'h'},
                {"debug",           kCLINoAargument,        NULL, 'd'},
                {"recepcionistas",  kCLIRequiredArgument,   NULL, 'r'},
                {"cocineras",       kCLIRequiredArgument,   NULL, 'c'},
                {"hornos",          kCLIRequiredArgument,   NULL, 'o'},
                {"cadetas",         kCLIRequiredArgument,   NULL, 'a'},
                {0,0,0,0},
        };

    int index;
    int iarg=0;

    //turn off getopt error message
    opterr=1;

    while(iarg != -1)
    {
        iarg = getopt_long(argc, argv, "vhdr:c:o:a:", longopts, &index);

        switch (iarg) {
            case 'h':
                printHelp();
                return -1;

            case 'v':
                printVersion();
                return -1;

            case 'd':
                debugMode = kDebugModeON;
                break;

            case 'r':
                if (optarg) {
                    recepcionistasCount = atol(optarg);
                }
                if (recepcionistasCount < 0) {
                    recepcionistasCount = kDefaultRecepcionistasCount;
                }
                break;

            case 'c':
                if (optarg) {
                    cocinerasCount = atol(optarg);
                }
                if (cocinerasCount < 0) {
                    cocinerasCount = kDefaultCocinerasCount;
                }
                break;

            case 'o':
                if (optarg) {
                    hornosCount = atol(optarg);
                }
                if (hornosCount < 0) {
                    hornosCount = kDefaultHornosCount;
                }
                break;

            case 'a':
                if (optarg) {
                    cadetasCount = atol(optarg);
                }
                if (cadetasCount < 0) {
                    cadetasCount = kDefaultCadetasCount;
                }
                break;
        }
    }

    return 0;
}

void inicializar() {
    Logger* log = Logger::getInstance();
    log->log(logINFO,"Inicio de ConcuDelivery.");
}

void loggearParametros() {
    Logger* log = Logger::getInstance();
    log->log(logINFO,"Recepcionistas: " + to_string(recepcionistasCount));
    log->log(logINFO,"Cocineras: " + to_string(cocinerasCount));
    log->log(logINFO,"Hornos: " + to_string(hornosCount));
    log->log(logINFO,"Cadetas: " + to_string(cadetasCount));
    log->log(logINFO,"Pedidos a simular: " + to_string(simulacionCount));
}

void crearRecepcionistas(vector<pid_t>& recepcionistas) {
    recepcionistas.reserve(cadetasCount);
    Logger* log = Logger::getInstance();
    Recepcionista r(cocinerasCount);
    for (long i = 0; i < recepcionistasCount; i++) {
        pid_t rpid = r.iniciar(i);
        log->log(logINFO,"Nueva recepcionista. PID: " + to_string(rpid));
        recepcionistas.push_back(rpid);
    }
}


void crearCocineras(vector<pid_t>& cocineras) {
    cocineras.reserve(cocinerasCount);
    Logger* log = Logger::getInstance();
    Cocinera c;
    for (long i = 0; i < cocinerasCount; i++) {
        pid_t rpid = c.iniciar(i);
        log->log(logINFO,"Nueva Cocinera. PID: " + to_string(rpid));
        cocineras.push_back(rpid);
    }
}

void crearHornos(vector<pid_t>& hornos) {
	hornos.reserve(hornosCount);
    Logger* log = Logger::getInstance();
    Horno h;
    for (long i = 0; i < hornosCount; i++) {
        pid_t rpid = h.iniciar(i);
        log->log(logINFO,"Nuevo Horno. PID: " + to_string(rpid));
        hornos.push_back(rpid);
    }
}

void crearCadetas(vector<pid_t>& cadetas) {
    cadetas.reserve(cadetasCount);
    Logger* log = Logger::getInstance();
    Cadeta c;
    for (long i = 0; i < cadetasCount; i++) {
        pid_t rpid = c.iniciar(i);
        log->log(logINFO,"Nueva Cadeta. PID: " + to_string(rpid));
        cadetas.push_back(rpid);
    }
}

void pararTodas(vector<pid_t>& pids) {
    Logger* log = Logger::getInstance();
    for (unsigned long i = 0; i < pids.size(); i++) {
        pid_t pid = pids.at(i);
        log->log(logINFO,"Parando pid "+ to_string(pid));
        Proceso::parar(pid);
        int status = 0;
        waitpid(pid,&status,0);
        log->log(logINFO,"Proceso " + to_string(pid) + " parado con status: " + to_string(status));
    }
}

bool realizarPedido(PedidosPorAtender *pedidosPorAtender, unsigned long count) {

    Pedido *p = new Pedido(count, 10.0, Pedido::ESPERANDO);

    return pedidosPorAtender->ingresarNuevoPedido(*p) == 0;
}

void comenzarTrabajo() {
    Logger* log = Logger::getInstance();

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGTERM, &sigint_handler);

    //1. crear recursos
    log->log(logINFO,"Creando recursos");

    /// Pedidos por atender (Cliente -> Recepcionista)
    unsigned long cantidadDePedidosRealizados = 0;
    PedidosPorAtender *pedidosPorAtender = PedidosPorAtender::getInstance();
    log->log(logINFO,"Buffer Pedidos para Atender creado.");

    // Pedidos para cocinar (Recepcionista -> Cocinera)
    PedidosParaCocinar::getInstance();
    log->log(logINFO,"Buffer Pedidos para Cocinar creado.");

    // Pedidos para hornear (Cocinera -> Horno)
    PedidosParaHornear *pedidosParaHornear = PedidosParaHornear::getInstance();
    /** Buffer de Pedidos para Hornear igual a la cantidad de Hornos **/
    pedidosParaHornear->setCantHornos(hornosCount);
    log->log(logINFO,"Buffer Pedidos para Hornear creado.");

    /// Pedidos para entregar y cobrar (Horno -> Cadeta)
    unsigned long cantidadDePedidosEntregados = 0;
    PedidosParaEntregar::getInstance();
    log->log(logINFO,"Buffer Pedidos para Entregar y Cobrar creado.");

    //2. Crear procesos
    log->log(logINFO,"Creando procesos");
    vector<pid_t> recepcionistas;
    vector<pid_t> cocineras;
    vector<pid_t> hornos;
    vector<pid_t> cadetas;
    Supervisora s;
    pid_t supervisora = s.iniciar(0);

    crearRecepcionistas(recepcionistas);
    crearCocineras(cocineras);
    crearHornos(hornos);
    crearCadetas(cadetas);

    pedidosPorAtender->inicializarParaEscribir();

    // sleep(3);

    //3. iniciar la simulacion
    while (sigint_handler.getGracefulQuit() == 0 && cantidadDePedidosRealizados < simulacionCount) {

        // hacer nuevo pedido
        bool pedidoRealizado = realizarPedido(pedidosPorAtender, cantidadDePedidosRealizados + 1);

        if (pedidoRealizado) {
            cantidadDePedidosRealizados++;
            log->log(logINFO,
                     " [ SIMULACION ] \tNuevo pedido en curso. Cantidad de pedidos realizados: " +
                     to_string(cantidadDePedidosRealizados));
        }

        // sleep max 1seg
        long time_in_usec = std::rand() % 1000;
        usleep(time_in_usec * 1000);
    }

    //4. Eserar a que cantidad de pedidos entregados / cobrados == cantidadDePedidosRealizados
    log->log(logINFO, " [ SIMULACION ] \tEsperando finalizacion de pedidos...");
    do {
        unsigned long c = PedidosParaEntregar::getInstance()->cantidadDePedidosEntregados();
        if (c != cantidadDePedidosEntregados) {
            cantidadDePedidosEntregados = c;
            log->log(logINFO,
                     " [ SIMULACION ] \tSimulación detecto nuevo pedido entregado. Pedidos simulados entregados: " +
                     to_string(cantidadDePedidosEntregados));
        }

        // sleep(1);
    } while (cantidadDePedidosEntregados < cantidadDePedidosRealizados);

    //5. detener procesos y eliminar recursos
    log->log(logINFO,">> Deteniendo procesos...");

    pedidosPorAtender->finalizarParaEscribir();

    pararTodas(recepcionistas);
    PedidosPorAtender::destroy();
    recepcionistas.clear();

    pararTodas(cocineras);
    PedidosParaCocinar::destroy();
    cocineras.clear();
    
    pararTodas(hornos);
    PedidosParaHornear::destroy();
    hornos.clear();

    pararTodas(cadetas);
    cadetas.clear();
    Proceso::parar(supervisora);
    PedidosParaEntregar::destroy();
    
    SignalHandler::destruir();

    log->log(logDEBUG,"Simulacion terminada");
}

void eliminarRecursos() {
    Logger::destroy();
}

int main(int argc, char **argv) {
    if (setupCLI(argc, argv)) {
        return ( 0 );
    }

    inicializar();
    loggearParametros();

    comenzarTrabajo();

    eliminarRecursos();
    return ( 0 );
}
