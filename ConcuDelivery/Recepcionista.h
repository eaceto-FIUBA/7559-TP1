//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_RECEPCIONISTA_H
#define CONCUDELIVERY_RECEPCIONISTA_H

#include "Proceso.h"

class Recepcionista: public Proceso {
private:
    void realizarTarea() {
        cout << "TAREA Recepcionista" << endl;
        sleep(1);
    }

    string nombre() {
        return "Recepcionista";
    };
};


#endif //CONCUDELIVERY_RECEPCIONISTA_H
