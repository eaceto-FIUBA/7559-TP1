//
// Created by Kimi on 10/04/15.
//

#include "Supervisora.h"
#include "PedidosParaEntregar.h"
#include "Constantes.h"

Supervisora::Supervisora() : Proceso() {

}

Supervisora::~Supervisora() {

}

void Supervisora::realizarTarea() {

	unsigned long c = PedidosParaEntregar::getInstance()->cantidadDePedidosEntregados();
	this->log(logINFO, "\tPedidos Entregados: " + to_string(c) + "\tRecaudación: $"+to_string(c*PEDIDO_COSTO) +".");
	sleep(1); //Lee la recaudación cada 1 segundo.
}

void Supervisora::inicializarProceso(unsigned long id) {

}

string Supervisora::nombre() {
    return "Supervisora " + to_string(getID());
}

void Supervisora::destruirRecursos() {

}
