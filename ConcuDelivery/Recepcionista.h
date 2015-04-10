//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_RECEPCIONISTA_H
#define CONCUDELIVERY_RECEPCIONISTA_H

#include "Proceso.h"

class Recepcionista: public Proceso {
private:
    virtual string nombre();

    virtual void realizarTarea();
};


#endif //CONCUDELIVERY_RECEPCIONISTA_H
