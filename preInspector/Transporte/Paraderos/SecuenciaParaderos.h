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
#include <limits>
#include "../../Utilidades/StringFunctions.h"
#include "Paradero.h"

using namespace std;

/**
* Title: SecuenciaParaderos
* Description:	Clase que contiene un conjunto de paraderos, agrupados por servicio-sentido-variante
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* @version 0.0
*/
class SecuenciaParaderos
{
public:
	SecuenciaParaderos(void);
	~SecuenciaParaderos(void);

	///Servicio-sentido vector de paraderos
	map< string , map < int, Paradero > > secuencias;

	/**
	* Metodo que inserta un nuevo paradero al diccionario
	*/
	void InsertaParadero(Paradero paradero);

};
