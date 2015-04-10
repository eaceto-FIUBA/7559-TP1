#include <iostream>

#include "Recepcionista.h"
#include "Cocinera.h"
#include "Cadeta.h"
#include "Supervisora.h"

using namespace std;

void print_welcome() {
    cout << "    ConcuDelivery v.01    " << endl;
    cout << "==========================" << endl;
}

int main() {
    print_welcome();

    Cadeta* cadetas = new Cadeta[1];

    for (int i = 0; i < 1; i++) {

        pid_t pidCadeta = cadetas[i].iniciar();
        cout << "Cadeta " + cadetas[i].descripcion() + " corriendo con PID: " + to_string(pidCadeta) << endl;
    }

    sleep(1000);

    cout << "SLEEP" << endl;

    while(1) {
        sleep(100);
    }
    /*
    sleep(5000);

    for (int i = 0; i < 5; i++) {
        cadetas[i].parar();
    }

    delete[] cadetas;

    return 0;
     */
}