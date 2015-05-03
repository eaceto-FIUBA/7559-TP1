#include <iostream>
#include <getopt.h>
#include <vector>

// Include de objectos del modelo
#include "Recepcionista.h"
#include "Cocinera.h"
#include "Cadeta.h"
#include "Supervisora.h"
#include "Logger.h"

using namespace std;

#define kAppVersion "0.1"

#define kDefaultRecepcionistasCount 5
#define kDefaultCadetasCount        2
#define kDefaultCocinerasCount      3
#define kDefaultHornosCount         6
#define kDefaultSimulacionCount     1024

#define kCLINoAargument         0
#define kCLIRequiredArgument    1
#define kCLIOptionalArgument    2

#define kDebugModeON            true
#define kDebugModeOFF           false
#define kDefaultDebugMode       kDebugModeOFF

bool            debugMode               = kDefaultDebugMode;
unsigned long   recepcionistasCount     = kDefaultRecepcionistasCount;
unsigned long   cadetasCount            = kDefaultCadetasCount;
unsigned long   hornosCount             = kDefaultHornosCount;
unsigned long   cocinerasCount          = kDefaultCocinerasCount;

unsigned long   simulacionCount         = kDefaultSimulacionCount;

void print_version() {
    cout << "ConcuDelivery v" << kAppVersion << endl << endl;
    cout << "[     ]\tOpromolla, Giovanni\t\tgiopromolla@gmail.com " << endl;
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
    log->log(logDEBUG,"Inicio de ConcuDelivery.");
}

void loggearParametros() {
    Logger* log = Logger::getInstance();
    log->log(logDEBUG,"Recepcionistas: " + to_string(recepcionistasCount));
    log->log(logDEBUG,"Cocineras: " + to_string(cocinerasCount));
    log->log(logDEBUG,"Hornos: " + to_string(hornosCount));
    log->log(logDEBUG,"Cadetas: " + to_string(cadetasCount));
    log->log(logDEBUG,"Pedidos a simular: " + to_string(simulacionCount));
}

void crearRecepcionistas(vector<Recepcionista>& recepcionistas) {
    recepcionistas.reserve(cadetasCount);
    Logger* log = Logger::getInstance();
    for (unsigned long i = 0; i < recepcionistasCount; i++) {
        Recepcionista r;
        log->log(logDEBUG,"Creando recepcionista "+ to_string(i) + ": " + r.descripcion());
        recepcionistas.push_back(r);
    }
}


void crearCocineras(vector<Cocinera>& cocineras) {
    cocineras.reserve(cocinerasCount);
    Logger* log = Logger::getInstance();
    for (unsigned long i = 0; i < cocinerasCount; i++) {
        Cocinera c;
        log->log(logDEBUG,"Creando cocinera "+ to_string(i) + ": " + c.descripcion());
        cocineras.push_back(c);
    }
}


void crearCadetas(vector<Cadeta>& cadetas) {
    cadetas.reserve(cadetasCount);
    Logger* log = Logger::getInstance();
    for (unsigned long i = 0; i < cadetasCount; i++) {
        Cadeta c;
        log->log(logDEBUG,"Creando cadeta "+ to_string(i) + ": " + c.descripcion());
        cadetas.push_back(c);
    }
}

void comenzarTrabajo() {
    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGTERM, &sigint_handler);

    // crear procesos
    vector<Recepcionista> recepcionistas;
    vector<Cocinera> cocineras;
    vector<Cadeta> cadetas;
    Supervisora* supervisora = new Supervisora;

    crearRecepcionistas(recepcionistas);
    crearCocineras(cocineras);
    crearCadetas(cadetas);

    bool working = false;
    while (sigint_handler.getGracefulQuit() == 0 && working) {

    }

    recepcionistas.clear();
    cocineras.clear();
    cadetas.clear();
    supervisora->parar();

    delete supervisora;

    SignalHandler::destruir();
}

void eliminarRecursos() {
    Logger* log = Logger::getInstance();
    delete log;
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
