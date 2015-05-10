
#ifndef HORNO_H_
#define HORNO_H_

#include "Proceso.h"

class Horno: public Proceso {

public:

	Horno();

	~Horno();

    void destruirRecursos();

private:

    void realizarTarea();

    string nombre();
};

#endif /* HORNO_H_ */

