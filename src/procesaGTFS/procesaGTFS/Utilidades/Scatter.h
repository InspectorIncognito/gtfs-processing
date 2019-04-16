/**
* Project: "pvm".
* Package: Utilidades
* Class: Scatter
* Created by: Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* Terms: These coded instructions, statements, and computer programs are
* produced as part of a software project. They contain unpublished
* propietary information and are protected by copyright law. They may
* not be disclosed to third parties or copied or duplicated in any form,
* in whole or in part, without the prior written consent of Pragxis SpA.
* Copyright:  Pragxis (c) 2013
* Last modified : Mauricio Zuñiga 15-04-2013
*/
#pragma once

#include <map>
#include <limits>
#include "StringFunctions.h"
#include "Types.h"

using namespace std;

/**
* Title: Scatter
* Description:	Clase que contiene variables y metodos para la construccion de un grafico de dipersion
* Copyright:  Pragxis (c) 2013
* Date: 15-04-2013
* @author Mauricio Zuñiga <mzuniga@pragxis.cl>
* @version 0.0
*/
class Scatter{
public:
	/**
	 * Constructor por defecto
	 **/
	Scatter();
	/**
	* Constructor
	*/
	Scatter(double maxsize,string pathin, int ix, int iy, string pathout);
	/**
	 * Destructor por defecto
	 **/
	~Scatter();

	///
	double maxsize;

	///Path con el archivo de datos 
	string pathIn;

	///Numero de la columna del eje x
	int ix;

	///Numero de la columna del eje y
	int iy;

	///Path donde quedara la figura de dibujarse (.png)
	string pathOut;

	/**
	* Metodo que inserta un nuevo valor
	*/
	//void Insert(double value);

	/**
	* Metodo que imprime la informacion del histograma
	*/
	string Imprime(string titulo,int tipoReporte);
	
	/**
	* Metodos que imprimen la informaicon en ASCII
	*/
	string ImprimeASCII(string titulo);

	/**
	* Metodos que imprimen la informaicon en LATEX
	*/
	string ImprimeLATEX(string titulo);

	/**
	* Metodo que crea un dibujo con el histograma
	*/
	void GraficaPython(string titulo);
};
