#pragma once

#include <math.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "Types.h"

namespace ConvertCoordinate{

	#define UTMZONE "19H"
	/**
	*	Metodo que convierte coordenadas en Latitud/Longitud a UTM
	*/
	void LLtoUTM(int ReferenceEllipsoid, const double Lat, const double Long, 
				double &UTMNorthing, double &UTMEasting, char* UTMZone);

	/**
	*	Metodo que convierte coordenadas UTM a Latitud/Longitud
	*/
	void UTMtoLL(int ReferenceEllipsoid, const double UTMNorthing, const double UTMEasting, const char* UTMZone,
				double& Lat,  double& Long );
	
	char UTMLetterDesignator(double Lat);

	/**
	* Metodo que transforma la coordenada X al eje coordenadas UTM
	* @param x coordenada transformada a eje para minimo uso de memoria
	* @return x transformada a x UTM 
	*/
	int X(unsigned short int x);

	/**
	* Metodo que transforma la coordenada X UTM a un eje desplazado para mejor uso de memoria
	* @param x coordenada transformada a eje para minimo uso de memoria
	* @return x transformada a x UTM 
	*/
	unsigned short int X(int x);

	/**
	* Metodo que transforma la coordenada Y al eje coordenadas UTM
	* @param y coordenada transformada a eje para minimo uso de memoria
	* @return y transformada a y UTM 
	*/
	int Y(unsigned short int y);

	/**
	* Metodo que transforma la coordenada Y al eje coordenadas UTM
	* @param y coordenada transformada a eje para minimo uso de memoria
	* @return y transformada a y UTM 
	*/
	unsigned short int Y(int y);
}

