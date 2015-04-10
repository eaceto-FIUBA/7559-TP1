cmake_minimum_required(VERSION 3.1)
project(ConcuDelivery)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(ConcuDelivery ${SOURCE_FILES} Cocinera.cpp Cocinera.h Proceso.cpp Proceso.h Recepcionista.cpp Recepcionista.h Cadeta.cpp Cadeta.h Supervisora.cpp Supervisora.h SignalHandler.cpp Logger.cpp Logger.h)