//
// Created by Kimi on 10/04/15.
//

#include "Supervisora.h"

Supervisora::Supervisora() {

}

Supervisora::~Supervisora() {

}

void Supervisora::realizarTarea() {
    //Logger::getInstance()->log(logDEBUG, "Esperando nuevo pedido...");
}

string Supervisora::nombre() {
    return "Supervisora " + to_string(getID());
}

void Supervisora::destruirRecursos() {

}