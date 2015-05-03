//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_CADETA_H
#define CONCUDELIVERY_CADETA_H

#include "Proceso.h"

class Cadeta: public Proceso {
    void realizarTarea() {
        // Logger* log = Logger::getInstance();
        // log->log(logDEBUG,"Cadeta. Trabajando.");
        sleep(1);
    }

    string nombre() {
        return "Cadeta";
    };
};


#endif //CONCUDELIVERY_CADETA_H
