//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_SUPERVISORA_H
#define CONCUDELIVERY_SUPERVISORA_H

#include "Proceso.h"

class Supervisora : public Proceso {
private:
    virtual string nombre();

    virtual void realizarTarea();
};


#endif //CONCUDELIVERY_SUPERVISORA_H
