#include "Pedido.h"

Pedido::Pedido(){
    numero = 0;
    costo = 0;
    estado = ESPERANDO;
}

Pedido::Pedido(unsigned long n, double c, Estado e){
    numero = n;
    costo = c;
    estado = e;
}

Pedido::~Pedido() {
    numero = 0;
    costo = 0;
    estado = ESPERANDO;
}
