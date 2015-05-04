//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_RECEPCIONISTA_H
#define CONCUDELIVERY_RECEPCIONISTA_H

#include "Proceso.h"
#include "MemoriaCompartidaConcurrente.h"
#include <string>

class Recepcionista: public Proceso {
private:

    MemoriaCompartidaConcurrente<unsigned long> pedidosEnCurso;

    bool hayLlamadaEntrante() {
        bool nuevoLlamado = false;
        if (pedidosEnCurso.tomarLockManualmente()) {
            unsigned long cantDePedidosActuales = pedidosEnCurso.leerInseguro();
            if (cantDePedidosActuales > 0) {
                cantDePedidosActuales--;
                nuevoLlamado = true;
                pedidosEnCurso.escribirInseguro(cantDePedidosActuales);
            }
            pedidosEnCurso.liberarLockManualmente();
        }
        return nuevoLlamado;
    }


    void realizarTarea() {

    }

    string nombre() {
        return "Recepcionista";
    };
public:

    Recepcionista() : pedidosEnCurso("pedidosEntrantes.shm", 'A') {

    }
};


#endif //CONCUDELIVERY_RECEPCIONISTA_H
