cmake_minimum_required(VERSION 2.6)
project(gtfstransapp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra ")

set( gtfstransapp_SRCS
  ${gtfstransapp_SOURCE_DIR}/main.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Parametros.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/FuenteDatos.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/ReporteFuenteDatos.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Tablas/TablaDistanciaEnRutaParadas.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Tablas/TablaGrilla.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Tablas/TablaParadas.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Tablas/TablaPuntoCarga.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Tablas/TablaRutasSimplificadas.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Tablas/TablaRutasTramadas.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Tablas/TablaServicios.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Tablas/TablaServiciosParaAndroid.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Tablas/TablaServiciosPorParada.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Tablas/TablaServiciosPorParadaParaAndriod.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Tablas/TablaServiciosPorParadasPorSecuencia.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Visualizacion/ColorScale.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Visualizacion/CreaKML.cpp
  ${gtfstransapp_SOURCE_DIR}/IO/Visualizacion/Visualizaciones.cpp
  ${gtfstransapp_SOURCE_DIR}/Transporte/CargaBip/PuntoBip.cpp
  ${gtfstransapp_SOURCE_DIR}/Transporte/Diccionarios/DiccionarioServicios.cpp
  ${gtfstransapp_SOURCE_DIR}/Transporte/Grid/Cell.cpp
  ${gtfstransapp_SOURCE_DIR}/Transporte/Grid/Grid.cpp
  ${gtfstransapp_SOURCE_DIR}/Transporte/Grid/GridProcess.cpp
  ${gtfstransapp_SOURCE_DIR}/Transporte/Paraderos/Paradero.cpp
  ${gtfstransapp_SOURCE_DIR}/Transporte/Paraderos/ProcesaParadas.cpp
  ${gtfstransapp_SOURCE_DIR}/Transporte/Paraderos/RedParadas.cpp
  ${gtfstransapp_SOURCE_DIR}/Transporte/Paraderos/SecuenciaParaderos.cpp
  ${gtfstransapp_SOURCE_DIR}/Transporte/Rutas/Ruta.cpp
  ${gtfstransapp_SOURCE_DIR}/Transporte/Rutas/Rutas.cpp
  ${gtfstransapp_SOURCE_DIR}/Transporte/Rutas/ProcesaRutas.cpp
  ${gtfstransapp_SOURCE_DIR}/Transporte/Servicios/Servicio.cpp
  ${gtfstransapp_SOURCE_DIR}/Utilidades/Geometry2.cpp
  ${gtfstransapp_SOURCE_DIR}/Utilidades/Cronometro.cpp
  ${gtfstransapp_SOURCE_DIR}/Utilidades/StringFunctions.cpp
  ${gtfstransapp_SOURCE_DIR}/Utilidades/ConvertCoordinate.cpp
  ${gtfstransapp_SOURCE_DIR}/Utilidades/TimeStampHandler.cpp
  ${gtfstransapp_SOURCE_DIR}/Utilidades/Geometry2.cpp
  ${gtfstransapp_SOURCE_DIR}/Utilidades/Histogram.cpp
)

add_executable(gtfstransapp main.cpp ${gtfstransapp_SRCS})

install(TARGETS gtfstransapp RUNTIME DESTINATION bin)

