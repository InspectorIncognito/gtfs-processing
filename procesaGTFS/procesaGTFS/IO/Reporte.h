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
#include "../Utilidades/StringFunctions.h"
#include "../Utilidades/TimeStampHandler.h"
#include "../Utilidades/Histogram.h"
#include "../Utilidades/Types.h"

using namespace std;


/**
* Title: Reporte
* Description:	Clase abstracta para la generacion de reportes
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga <mzuniga@pragxis.cl>
* @version 0.0
*/
class Reporte
{
public:
	/**
	* Constructor de la clase
	* @param fdd referencia o puntero a objeto con todos los datos de entrada
	*/
	Reporte(string path_) {this->path = path_;}
	/**
	* Destructor de la clase
	*/
	virtual ~Reporte(void) {};

	///Variable que almacena el path donde se creara el reporte
	string path;

	///Variable que define el formato del reporte final (ASCII, LATEX..)
	int tipoReporte_;

	/**
	 * Metodo que imprime en un string el reporte del procesamiento de 
	 * de las expediciones
	 * @return string con el reporte
	 */
	virtual string Imprime(int iCapitulo, int TipoReporte) = 0;

	/**
	 * Metodo que imprime en un string el reporte de la lectura de datos en formato ASCII
	 * @return string con el reporte
	 */
	virtual string ImprimeASCII(int iCapitulo) = 0;

	/**
	 * Metodo que imprime en un string el reporte de la lectura de datos en formato ASCII
	 * @return string con el reporte
	 */
	virtual string ImprimeLATEX() = 0;

	/**
	* Metodo que inicializa las variables 
	*/
};