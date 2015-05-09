//
// Created by Kimi on 10/04/15.
//

#include "Cadeta.h"

Cadeta::Cadeta() {

}

Cadeta::~Cadeta() {

}

void Cadeta::realizarTarea() {
    //Logger::getInstance()->log(logDEBUG, "Esperando nuevo pedido...");
}

string Cadeta::nombre() {
    return "Cadeta " + to_string(getID());
}

void Cadeta::destruirRecursos() {

}