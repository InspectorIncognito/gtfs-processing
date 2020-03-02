/**
* Project: "proceGTFS".
* Package:
* Class:
* Created by: Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* Terms: These coded instructions, statements, and computer programs are
* produced as part of a software project. They contain unpublished
* propietary information and are protected by copyright law. They may
* not be disclosed to third parties or copied or duplicated in any form,
* in whole or in part, without the prior written consent of Pragxis SpA.
* Copyright:  Pragxis (c) 2016
* Last modified : Mauricio Zuñiga 18-06-2016
*/
#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "Reporte.h"
#include "../Utilidades/TimeStampHandler.h"

using namespace std;

struct ErroresRuta{
	int nNodosRepetidos;
	int nNodosFueraDeRuta;
	int nNodosFueraDeSantiago;
};

struct ErroresParadas{
	string codigoParada;
	int fueraDeRuta;
	bool fueraDeSantiago;
	bool secuenciaInconsistente;
};

struct ErroresNoExistenciaDatos{
	bool noDicRuta;
	bool noDicSec;
	bool noRuta;
	bool noSec;
};

/**
* Title: Reporte de los datos de entrada
* Description:	Clase con metodos y variables para construir un reporte de los datos de entrada
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga <mzuniga@pragxis.cl>
* @version 0.0
*/
class ReporteFuenteDatos : public Reporte
{
public:
	/**
	* Constructor de la clase
	* @param fdd referencia o puntero a objeto con todos los datos de entrada
	*/
	ReporteFuenteDatos(string path_);
	/**
	* Destructor de la clase
	*/
	~ReporteFuenteDatos(void);

	///Variable que almacena el path donde se creara el reporte
	string path;

	///Variable que define el formato del reporte final (ASCII, LATEX..)
	int tipoReporte_;

	///Instancia de clase para manejo de tiempo
	TimeStampHandler tsh;

	///Numero de registros del diccionario de servicio-sentido
	int nRegistrosDiccionario;

	///Numero de servicios no encontrados en diccionario oficial
	int nSSnoEncontradosEnDiccionario;

	///Listado de servicios de reporte 2.2 no encontrados en diccionario
	map<string,int> ssNoEncontradosEnDiccionario;

	///Numero de registros del reporte 2.2
	int nRegistros22;

	///Numero de patentes en las expediciones 2.2
	int nSitios22;

	///Numero de expediciones en el reporte 2.2
	int nExp22;

	///Numero de expediciones con tiempo menor que 2 segundos
	int nExp22Menos2Segundos;

	///Histograma de tiempos de expediciones 2.2
	Histogram hist22;

	///Numero de registros del reporte 1.96
	int nRegistros196;

	///Numero de patentes en las expediciones 1.96
	int nSitios196;

	///Numero de expediciones en el reporte 1.96
	int nExp196;

	///Numero de patentes nuevas encontradas en 1.96
	int nSitiosEn196NoEn22;

	///Numero de registros no considerados (horainicio nulas)
	int nRegistrosNulos196;

	///Numero de buses en posicionamiento
	int nSitiosEnPos;

	///Numero de patentes nuevas encontradas en posicionamiento
	int nSitiosEnPosNoEnExps;

	///Numero de registros GPS
	int nRegistrosGPS;

	///Numero de registros GPS repetidos
	int nRegistrosGPSRepetidos;

	///Numero de registros con patente nula
	int nRegistrosGPSPatenteNula;

	///Histograma de tiempos de expediciones 2.2
	Histogram histTGps;

	///Histograma de numero de registros por hora
	Histogram histGpsPorHora;

	///Numero de rutas
	int nRutas;

	///Estructura para guardar la ruta mas larga
	pair<string,int> rutaMasLarga;

	///Estructura para guardar la ruta mas corta
	pair<string,int> rutaMasCorta;

	///Mapa que guarda las rutas que contienen algun tipo de error
	map< string, ErroresRuta> rutasErradas;

	///Numero de rutas con nodos repetidos
	int nRutasNodosRepetidos;

	///Numero de rutas con nodos fuera de Ruta
	int nRutasNodosFueraDeRuta;

	///Numero de rutas con nodos fuera de santiago
	int nRutasNodosFueraDeSantiago;

	///Numero de secuencias de parada
	int nSecParadas;

	///Numero de paraderos de secuencia con mas paraderos
	pair<string,int> nMaxParadas;

	///Numero de paraderos de secuencia con menos paraderos
	pair<string,int> nMinParadas;

	///Secuencias sin ruta encontrada
	map<string,int> secuenciasSinRutas;

	///Codigos de parada sin formato conocido
	map<string,int> paradasSinFormato;

	///Listado de servicios y paradas con algu problema en particular
	map<string, vector < ErroresParadas > > paradasErradas;

	///Listados de servicio-parada con inconsistencias en su secuencia
	map<string,string> inconsistenciasSecuencia;

	///Numero de paradas en la red de paradas
	int nRedParadas;

	///Numero de paradas inactivas
	int nRedParadasInactivas;

	///Numero de paradas repetidas
	int nRedParadasRepetidas;

	///Numero de zonas pagas
	int nZonasPagas;

	///Numero de validadores
	int nValidadoresZonasPagas;

	///Paraderos de zonas pagas no encontrados en los paraderos de la red
	map<string,string> paraderosNoEncontradosDeZonasPagas;

	///Paraderos de las distribuciones no encontrados en las zonas pagas
	map<string,int> paraderosNoEncontradosDeDistribuciones;

	///Distribuciones que no cierran en 1+-0.001
	map< string , map<int, map<string,double> > > distribucionesMalas;

	///Numero de registros o validadores en tabla
	int nValidadoresZonasPagasTabla;

	///Numero de registros o validadores en tabla
	int nValidadoresRepetidosZonasPagasTabla;

	///Numero de validadores sin zona paga encontrada
	int nValidadoresSinZonaPaga;

	///Validadores con servicio repetido
	map<string,int> ValidadoresConServiciosRepetidos;

	///Paraderos de los validadores no encontrados de las secuencias de paradas
	map<string,int> ValidadoresConParaderoSinSecuencia;

	///Numero de patentes encontradas en el archivo de capacidades no encontradas
	int nPatentesEnCapacidadesNoEnResto;

	///Numero de patentes encontradas en el archivo de capacidades no encontradas
	int nPatentesEnRestoNoEnCapacidades;

	///Patentes que no estan en el archivo de capacidades
	map<string,int> patentesSinCapacidadEncontrada;

	///Reconocimiento previo de no existencia de rutas o secuencia de paradas para servicios del reporte 2.2
	map< string , ErroresNoExistenciaDatos > noExistenciaDatos;

	///Numero de estaciones de metro
	int nEstacionesMetro;

	///Numero de esaciones de metro repetidas
	int nEstacionesMetroRepetidas;

	///Numero de registros 
	int nEvasionRegistros;

	///Numero de registros repetidos de evasion
	int nEvasionRegistrosRepetidos;

	///Registros repetidos de evasion (paradero-servicio-nveces)
	map< string , int > evasionRepetida;

	///Registros repetidos en evasion por zona
	map< string , int > evasionRepetidaZonas;

	///Numero de zonificaciones
	map<string,int> zonificaciones;

	///Numero de zonas con 2 o menos nodos por zonificacion
	map<string,int> zonasMenos2Nodos;

	///Tiempo lectura diccionario
	double tDiccionario;

	///Tiempo lectura R22
	double tR22;

	///Tiempo lectura 196
	double tR196;

	///Tiempo lectura posicionamiento
	double tPosicionamiento;

	///Tiempo lectura rutas
	double tRutas;

	///Tiempo lectura paradas
	double tParadas;

	///Tiempo lectura de zonas pagas
	double tZonasPagas;

	///Tiempo de lectura estaciones metro
	double tEstacionesMetro;

	///Tiempo de lectura del grafo del metro
	double tGrafoMetro;

	///Tiempo lectura datos de evasion
	double tEvasion;

	///Tiempo de lectura de tabla de validadores
	double tValidadores;

	/**
	* Metodo que inserta una ruta problematica o un nuevo error de una ruta ya problematica
	*/
	void InsertaRutaError(string codigoRuta,int nNodosRepetidos,int nNodosFueraDeRuta,int nNodosFueraDeSantiago);

	/**
	* Metodo que inserta una ruta problematica o un nuevo error de una ruta ya problematica
	*/
	void InsertaParadaError(string codigoSec,string codigoParada,int fueraDeRuta,bool fueraDeSantiago);

	/**
	* Metodo que guarda una nueva distribucion mala
	*/
	void InsertaDistribucionMala(string validador,int mediahora,map<string,double> distribucion);

	/**
	 * Metodo que imprime en un string el reporte del procesamiento de 
	 * de las expediciones
	 * @return string con el reporte
	 */
	string Imprime(int iCapitulo, int TipoReporte);

	/**
	 * Metodo que imprime en un string el reporte de la lectura de datos en formato ASCII
	 * @return string con el reporte
	 */
	string ImprimeASCII(int iCapitulo);

	/**
	 * Metodo que imprime en un string el reporte de la lectura de datos en formato ASCII
	 * @return string con el reporte
	 */
	string ImprimeLATEX();

	/**
	* Metodo que imprime la seccion de reporte para los diccionarios
	*/
	string ImprimeDiccionarioASCII(int iCapitulo);
	string ImprimeDiccionarioLATEX();

	/**
	* Metodo que imprime la seccion de reporte para los diccionarios
	*/
	string ImprimeReporte22ASCII(int iCapitulo);
	string ImprimeReporte22LATEX();

	/**
	* Metodo que imprime la seccion de reporte para los diccionarios
	*/
	string ImprimeReporte196ASCII(int iCapitulo);
	string ImprimeReporte196LATEX();

	/**
	* Metodo que imprime la seccion de reporte para los diccionarios
	*/
	string ImprimePosicionamientoASCII(int iCapitulo);
	string ImprimePosicionamientoLATEX();

	/**
	* Metodo que imprime la seccion de reporte para los diccionarios
	*/
	string ImprimeRutasASCII(int iCapitulo);
	string ImprimeRutasLATEX();

	/**
	* Metodo que imprime la seccion de reporte para los diccionarios
	*/
	string ImprimeParadasASCII(int iCapitulo);
	string ImprimeParadasLATEX();

	/**
	* Metodo que imprime la seccion de reporte para los diccionarios
	*/
	string ImprimeRedParadasASCII(int iCapitulo);
	string ImprimeRedParadasLATEX();

	/**
	* Metodo que imprime la seccion de reporte para los diccionarios
	*/
	string ImprimeZPsASCII(int iCapitulo);
	string ImprimeZPsLATEX();

	/**
	* Metodo que imprime la seccion de reporte para la evasion
	*/
	string ImprimeEvasionASCII(int iCapitulo);
	string ImprimeEvasionLATEX();

	/**
	* Metodo que imprime la seccion de reporte para el metro
	*/
	string ImprimeMetroASCII(int iCapitulo);
	string ImprimeMetroLATEX();

	/**
	* Metodo que imprime la seccion de reporte para el metro
	*/
	string ImprimeValidadoresASCII(int iCapitulo);
	string ImprimeValidadoresLATEX();

	/**
	* Metodo que imprime la seccion resumen de datos no existentes ASCII
	*/
	string ImprimeNoExistenciaASCII(int iCapitulo);
	string ImprimeNoExistenciaLATEX();
	
	/**
	* Metodo que imprime la seccion resumen de datos no existentes ASCII
	*/
	string ImprimeZonificacionesASCII(int iCapitulo);
	string ImprimeZonificacionesLATEX();
};