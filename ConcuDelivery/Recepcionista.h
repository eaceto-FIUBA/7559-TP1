//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_RECEPCIONISTA_H
#define CONCUDELIVERY_RECEPCIONISTA_H

#include "Proceso.h"
#include <string>

class Recepcionista: public Proceso {

public:
    Recepcionista(unsigned long cocinerasDisponibles);

    ~Recepcionista();

    void destruirRecursos();

private:

    unsigned long cantDeCocinerasDisponibles;

    void realizarTarea();

    string nombre();
};


#endif //CONCUDELIVERY_RECEPCIONISTA_H
