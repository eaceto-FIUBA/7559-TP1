//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_RECEPCIONISTA_H
#define CONCUDELIVERY_RECEPCIONISTA_H

#include "Proceso.h"
#include "MemoriaCompartidaConcurrente.h"


class Recepcionista: public Proceso {

public:
    Recepcionista();
    ~Recepcionista();

private:
    MemoriaCompartidaConcurrente<unsigned long>* pedidosPorAtender;

    void procesarNuevoPedido();

    bool puedoProcesarNuevoPedido();

    void realizarTarea();

    string nombre() {
        return "Recepcionista";
    };


};


#endif //CONCUDELIVERY_RECEPCIONISTA_H
