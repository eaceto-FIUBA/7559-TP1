cmake_minimum_required(VERSION 3.1)
project(ConcuDelivery)

set(EXECUTABLE_OUTPUT_PATH "bin/")
set(CMAKE_CXX_FLAGS "-g -Wall")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
	Constantes.h 
	Utils.cpp 
	Utils.h
	Cadeta.cpp 
	Cadeta.h 
	Cocinera.cpp 
	Cocinera.h
	Horno.cpp
	Horno.h 
	EventHandler.h
	LockFile.cpp 
	LockFile.h
	MemoriaCompartida2.h
    MemoriaCompartidaConcurrente.h
    Fifo.h
    Fifo.cpp
    FifoEscritura.h
    FifoEscritura.cpp
    FifoLectura.h
    FifoLectura.cpp
	Logger.cpp
	Logger.h		 
	main.cpp
	Semaforo.cpp
	Semaforo.h
	Proceso.cpp 
	Proceso.h 
	Recepcionista.cpp 
	Recepcionista.h 
	SIGINT_Handler.h
	SignalHandler.cpp
	SignalHandler.h  
	Supervisora.cpp 
	Supervisora.h
	PedidosPorAtender.cpp
	PedidosPorAtender.h
	PedidosParaCocinar.cpp
	PedidosParaCocinar.h
	PedidosParaEntregar.cpp
	PedidosParaEntregar.h
	PedidosParaHornear.cpp
	PedidosParaEntregar.h)
	
add_executable(ConcuDelivery ${SOURCE_FILES} Pedido.cpp Pedido.h)
