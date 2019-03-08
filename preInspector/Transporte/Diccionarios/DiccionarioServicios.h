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
#include <string>
#include <iostream>
#include <stdlib.h>
#include "../../Utilidades/StringFunctions.h"

using namespace std;

/**
* Title: DiccionarioServicios
* Description:	Clase contenedora de los diccionarios necesarios para el proceso
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* @version 0.0
*/
class DiccionarioServicios
{
public:
	DiccionarioServicios(void);
	~DiccionarioServicios(void);

	///Tabla diccionario para codigo 2.2 y paraderos
	map<string,string> servicios;

	///Tabla diccionario para codigo 2.2 y paraderos
	map<string, string> servicios_rutas;

	///Colores por servicio (servicio - color)
	map<string, string> colores;

};
