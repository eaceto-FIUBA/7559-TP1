//
// Created by Kimi on 10/04/15.
//

#include "Cocinera.h"


Cocinera::Cocinera() {

}

Cocinera::~Cocinera() {

}

void Cocinera::realizarTarea() {
    //Logger::getInstance()->log(logDEBUG, "Esperando nuevo pedido...");
}

string Cocinera::nombre() {
    return "Cocinera " + to_string(getID());
}

void Cocinera::destruirRecursos() {

}