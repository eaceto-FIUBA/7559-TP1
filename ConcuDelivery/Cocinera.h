//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_COCINERA_H
#define CONCUDELIVERY_COCINERA_H

#include "Proceso.h"

class Cocinera: public Proceso {
private:
    virtual string nombre();

    virtual void realizarTarea();
};


#endif //CONCUDELIVERY_COCINERA_H
