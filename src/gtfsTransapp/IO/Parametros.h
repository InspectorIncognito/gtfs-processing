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

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include "../Utilidades/Types.h"
#include "../Utilidades/StringFunctions.h"

using namespace std;

/**
* Title: Parametros externos
* Description:	Clase contenedora de los parametros externos adicionales, como por ejemplo locacion 
* 		de los archivos de datos y la ubicacion de los campos necesarios dentro del archivo
* 		ademas segun el caso tambien si incluyen otros parametros necesarios para el calculo
* 		de los distintos algoritmos de la aplicacion.
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga G.
* @version 0.0
*/
class Parametros{
public:
	/**
	 * Constructor por defecto
	 **/
	Parametros();
	/**
	 * Constructor 
	 * @param file nombre del archivo donde se encuentran los parametros a leer
	 **/
	Parametros(const char *file);
	/**
	 * Destructor por defecto
	 **/
	~Parametros();

	///Nombre del archivo con los datos de transacciones
	string nombreCarpetaGTFS;

	///Nombre de archivo con horarios del funcionamiento de servicios
	//string nombreArchivoHorarios;

	///Nombre de archivo con horarios del funcionamiento de servicios
	//string nombreArchivoHorariosMetro;

	///Nombre de archivo con datos de puntos de carga bip
	string nombreArchivoPuntosCargaBip;

	///Nombre de archivo con los datos de secuencias de paradas de dtpm
	//string nombreArchivoConsolidadoDeParadascddcsd;

	///Nombre carpeta para guardar los kmls
	string carpetaKmls;

	///Nombre carpeta para guardar los reportes
	string carpetaReportes;

	///Nombre carpeta para guardar los reportes
	string carpetaOutput;

	///Distancia para tramar la ruta
	float distanciaTramado;

	///Mapeo colores hexadecimales a decimales
	//int nColores;
	//map<string, string> mapeoColores;
	
	///Caracter separador para path
	string slash;

	///sufijo para versionar los archivos
	string version;
    
    ///switches
    bool withBip=false;
    bool withMetro=false;
    

	/**
	 * Metodo para la impresion en consola de los parametros utilizados
	 */
	virtual void Imprime();


};
