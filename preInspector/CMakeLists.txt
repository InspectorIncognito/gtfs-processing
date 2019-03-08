cmake_minimum_required(VERSION 2.6)
project(preinspector)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra ")

set( preinspector_SRCS
  ${preinspector_SOURCE_DIR}/main.cpp
  ${preinspector_SOURCE_DIR}/IO/Parametros.cpp
  ${preinspector_SOURCE_DIR}/IO/FuenteDatos.cpp
  ${preinspector_SOURCE_DIR}/Transporte/Diccionarios/DiccionarioServicios.cpp
  ${preinspector_SOURCE_DIR}/Transporte/Servicios/Servicio.cpp
  ${preinspector_SOURCE_DIR}/Utilidades/Cronometro.cpp
  ${preinspector_SOURCE_DIR}/Utilidades/StringFunctions.cpp
  ${preinspector_SOURCE_DIR}/Utilidades/ConvertCoordinate.cpp
  ${preinspector_SOURCE_DIR}/Utilidades/TimeStampHandler.cpp
#   ${preinspector_SOURCE_DIR}/IO/SourceData.cpp
)

add_executable(preinspector main.cpp ${preinspector_SRCS})

install(TARGETS preinspector RUNTIME DESTINATION bin)
