//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_COCINERA_H
#define CONCUDELIVERY_COCINERA_H

#include "Proceso.h"

class Cocinera: public Proceso {
public:
    Cocinera();

    ~Cocinera();

    void destruirRecursos();

private:
    void realizarTarea();

    void inicializarProceso(unsigned long id);

    string nombre();
};

#endif //CONCUDELIVERY_COCINERA_H
