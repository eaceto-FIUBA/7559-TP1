#include <iostream>
#include <getopt.h>

// Include de objectos del modelo
#include "Recepcionista.h"
#include "Cocinera.h"
#include "Cadeta.h"
#include "Supervisora.h"

using namespace std;

#define kDefaultRecepcionistasCount 5
#define kDefaultCadetasCount        2
#define kDefaultCocinerasCount      3
#define kDefaultHornosCount         6

#define kCLINoAargument         0
#define kCLIRequiredArgument    1
#define kCLIOptionalArgument    2

#define kDebugModeON            true
#define kDebugModeOFF           false
#define kDefaultDebugMode       kDebugModeOFF

int     debugMode               = kDefaultDebugMode;
int     recepcionistasCount     = kDefaultRecepcionistasCount;
int     cadetasCount            = kDefaultCadetasCount;
int     hornosCount             = kDefaultHornosCount;
int     cocinerasCount          = kDefaultCocinerasCount;

void print_welcome() {
    cout << "                 ConcuDelivery v.01                 " << endl;
    cout << "====================================================" << endl;
    cout << "[     ] Opromolla, Giovanni - giopromolla@gmail.com " << endl;
    cout << "[84316] Aceto, Ezequiel - ezequiel.aceto@gmail.com  " << endl;
    cout << "====================================================" << endl;
}

int exit() {
    // matar al resto de los procesos
    return 0;
}

void printHelp() {
    print_welcome();
    cout << "Ayuda" << endl << endl;
}

void printVersion() {
    print_welcome();
    cout << "Version v.01 (1C 2015)" << endl << endl;
}

void setupCLI(int argc, char **argv) {
    const struct option longopts[] =
        {
                {"version",         kCLINoAargument,        NULL, 'v'},
                {"help",            kCLINoAargument,        NULL, 'h'},
                {"debug",           kCLINoAargument,        NULL, 'd'},
                {"recepcionistas",  kCLIOptionalArgument,   NULL, 'r'},
                {"cocineras",       kCLIOptionalArgument,   NULL, 'c'},
                {"hornos",          kCLIOptionalArgument,   NULL, 'o'},
                {"cadetas",         kCLIOptionalArgument,   NULL, 'a'},
                {0,0,0,0},
        };

    int index;
    int iarg=0;

    //turn off getopt error message
    opterr=1;

    while(iarg != -1)
    {
        iarg = getopt_long(argc, argv, "vhdr:c:o:a:", longopts, &index);

        switch (iarg)
        {
            case 'h':
                printHelp();
                exit();
                break;

            case 'v':
                printVersion();
                exit();
                break;

            case 'd':
                debugMode = kDebugModeON;
                break;

            case 'r':
                recepcionistasCount = atoi(::optarg);
                if (recepcionistasCount < 0) {
                    recepcionistasCount = kDefaultRecepcionistasCount;
                }
                break;

            case 'c':
                cocinerasCount = atoi(::optarg);
                if (cocinerasCount < 0) {
                    cocinerasCount = kDefaultCocinerasCount;
                }
                break;

            case 'o':
                hornosCount = atoi(::optarg);
                if (hornosCount < 0) {
                    hornosCount = kDefaultHornosCount;
                }
                break;

            case 'a':
                cadetasCount = atoi(::optarg);
                if (cadetasCount < 0) {
                    cadetasCount = kDefaultCadetasCount;
                }
                break;
        }
    }
}

void printRunParameters() {
    cout << "====================================" << endl;
    cout << "Parametros" << endl;
    cout << "------------------------------------" << endl;
    cout << "\tRecepcionistas:\t\t" << recepcionistasCount << endl;
    cout << "\tCocineras:\t\t" << cocinerasCount << endl;
    cout << "\tHornos:\t\t\t" << hornosCount << endl;
    cout << "\tCadetas:\t\t" << cadetasCount << endl;
    cout << "====================================" << endl << endl;

}

int main(int argc, char **argv) {
    setupCLI(argc, argv);

    printRunParameters();

    return exit();
}