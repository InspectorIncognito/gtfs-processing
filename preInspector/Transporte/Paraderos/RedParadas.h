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

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include "Paradero.h"
#include "../../Utilidades/StringFunctions.h"
#include "../../Utilidades/Cronometro.h"
#include "../../Utilidades/ConvertCoordinate.h"

using namespace std;

/**
* Title: Red de Paradas
* Description:	Clase que contiene un conjunto de paraderos independientes (RED DE PARADAS)
* Copyright:  (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga
* @version 0.0
*/
class RedParadas
{
public:
  	/**
	* Contructor por defecto de la clase
	*/
	RedParadas(void);
	/**
	* Destructor por defecto de la clase
	*/
	~RedParadas(void);

	///Red de paradas, la llave es el codigo de paradero
	map<string,Paradero> red;


	/**
	 * Metodo que imprime en consola el listado de paraderos que contiene el objeto
	 */
	void Imprime(string nombreArchivo);
};
