//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_CADETA_H
#define CONCUDELIVERY_CADETA_H

#include "Proceso.h"

class Cadeta: public Proceso {
public:
    Cadeta();

    ~Cadeta();

    void destruirRecursos();

private:
    void realizarTarea();

    void inicializarProceso(unsigned long id);

    string nombre();
};


#endif //CONCUDELIVERY_CADETA_H
