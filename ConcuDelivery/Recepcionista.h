//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_RECEPCIONISTA_H
#define CONCUDELIVERY_RECEPCIONISTA_H

#include "Proceso.h"
#include "MemoriaCompartidaConcurrente.h"
#include <string>

class Recepcionista: public Proceso {

public:
    Recepcionista();
    ~Recepcionista();

private:

    void realizarTarea();

    string nombre() {
        return "Recepcionista";
    };
};


#endif //CONCUDELIVERY_RECEPCIONISTA_H
