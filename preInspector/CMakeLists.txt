cmake_minimum_required(VERSION 2.6)
project(preinspector)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra ")

set( preinspector_SRCS
  ${preinspector_SOURCE_DIR}/main.cpp
  ${preinspector_SOURCE_DIR}/IO/Parametros.cpp
  ${preinspector_SOURCE_DIR}/IO/FuenteDatos.cpp
  ${preinspector_SOURCE_DIR}/Transporte/Diccionarios/DiccionarioServicios.cpp
  ${preinspector_SOURCE_DIR}/Transporte/Servicios/Servicio.cpp
  ${preinspector_SOURCE_DIR}/Transporte/Rutas/Rutas.cpp
  ${preinspector_SOURCE_DIR}/Transporte/Rutas/Ruta.cpp
  ${preinspector_SOURCE_DIR}/Transporte/Paraderos/Paradero.cpp
  ${preinspector_SOURCE_DIR}/Transporte/Paraderos/RedParadas.cpp
  ${preinspector_SOURCE_DIR}/Transporte/Paraderos/SecuenciaParaderos.cpp
  ${preinspector_SOURCE_DIR}/Transporte/Paraderos/ProcesaParadas.cpp
  ${preinspector_SOURCE_DIR}/Transporte/Grid/Cell.cpp
  ${preinspector_SOURCE_DIR}/Transporte/Grid/Grid.cpp
  ${preinspector_SOURCE_DIR}/Transporte/Grid/GridProcess.cpp
  ${preinspector_SOURCE_DIR}/Utilidades/Cronometro.cpp
  ${preinspector_SOURCE_DIR}/Utilidades/StringFunctions.cpp
  ${preinspector_SOURCE_DIR}/Utilidades/ConvertCoordinate.cpp
  ${preinspector_SOURCE_DIR}/Utilidades/TimeStampHandler.cpp
  ${preinspector_SOURCE_DIR}/Utilidades/Geometry2.cpp
  ${preinspector_SOURCE_DIR}/IO/Tablas/TablaServiciosPorParadaParaAndriod.cpp
  ${preinspector_SOURCE_DIR}/IO/Tablas/TablaGrilla.cpp

)

add_executable(preinspector main.cpp ${preinspector_SRCS})

install(TARGETS preinspector RUNTIME DESTINATION bin)
