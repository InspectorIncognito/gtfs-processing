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
#include <string>
#include <map>
#include "../../Utilidades/Types.h"

using namespace std;

/**
* Title: Paradero
* Description:	Clase que modela un parada de bus
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* @version 0.0
*/
class Paradero
{
public:
	///Codificacion de los tipos de transporte posibles de una etapa o transaccion
	enum TIPOTRANSPORTE { BUS=1, METRO=2, ZP=3 };

	/**
	* Constructor por defecto
	*/
	Paradero(void);
	/**
	* Constructor
	*/
	Paradero(double lat, double lon,int x,int y,string codigo,string nombre);
	/**
	* Destructor
	*/
	~Paradero(void);

	///Coordenada X UTM
	int x;

	///Coordenada Y UTM
	int y;

	///Coordenada 
	double lat;

	///Coordenada
	double lon;

	///Codigo del paradero
	string codigo;

	///Nombre del paradero
	string nombre;
};
