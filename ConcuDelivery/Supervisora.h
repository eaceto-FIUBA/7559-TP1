//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_SUPERVISORA_H
#define CONCUDELIVERY_SUPERVISORA_H

#include "Proceso.h"

class Supervisora : public Proceso {

public:
    Supervisora();

    ~Supervisora();

    void destruirRecursos();

private:
    void realizarTarea();

    void inicializarProceso(unsigned long id);

    string nombre();
};


#endif //CONCUDELIVERY_SUPERVISORA_H
