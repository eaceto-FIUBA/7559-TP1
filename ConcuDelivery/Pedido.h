
#ifndef CONCUDELIVERY_PEDIDO_H
#define CONCUDELIVERY_PEDIDO_H


class Pedido {

public:

    enum Estado {
        ESPERANDO = 0,
        ATENDIDO = 1,
        LISTO_PARA_COCINAR = 2,
        COCINADO = 3,
        ENTREGADO_Y_COBRADO = 4
    };

    Pedido();
    Pedido(unsigned long n, double c, Estado e);
    ~Pedido();

    unsigned long numero;
    double costo;
    Estado estado;
};


#endif //CONCUDELIVERY_PEDIDO_H
