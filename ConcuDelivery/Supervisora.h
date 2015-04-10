//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_SUPERVISORA_H
#define CONCUDELIVERY_SUPERVISORA_H

#include "Proceso.h"

class Supervisora : public Proceso {
private:
    void realizarTarea() {
        //cout << "TAREA Supervisora" << endl;
    }

    string nombre() {
        return "Supervisora";
    };
};


#endif //CONCUDELIVERY_SUPERVISORA_H
