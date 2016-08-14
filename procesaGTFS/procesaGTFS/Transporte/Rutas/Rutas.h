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
#include <limits>
#include <fstream>
#include "Ruta.h"

using namespace std;

/**
* Title: Rutas
* Description:	Clase contenedora de un conjunto de rutas, agrupados por servicio
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* @version 0.0
*/
class Rutas
{
public:
	Rutas(void);
	~Rutas(void);

	///Mapeo de las rutas a traves del codigo de servicio-sentido
	map<string,Ruta> *mapeo;

	/**
	 * Metodo que entrega la distancia de la ruta mas larga y su servicio
	 * @return par con el codigo del servicio con la ruta mas larga y su distancia
	 */
	pair<string,int> RutaMasLarga();
	
	/**
	 * Metodo que entrega la distancia de la ruta mas corta y su servicio
	 * @return par con el codigo del servicio con la ruta mas corta y su distancia
	 */
	pair<string,int> RutaMasCorta();

	/**
	* Metodo que chequea nodos fuera de ruta
	*/
	map< string, int> ChequeoGeometrico();

	/**
	* Metodo que calcula los largos de las rutas
	*/
	void CalculaLargosRutas();

	void CalculaTramificado(double distancia);

	/**
	* Metodo que simplifica las rutas
	*/
	void SimplificaRutas();
};
