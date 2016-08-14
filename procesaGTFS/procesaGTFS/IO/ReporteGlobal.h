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
#include <vector>
#include <string>
#include <time.h>
#include "../Utilidades/Cronometro.h"
#include "../Utilidades/Types.h"
#include "FuenteDatos.h"


using namespace std;

/**
* Title: Reporte global del procesamiento
* Description:	Clase que une los distintos reportes en uno solo
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga
* @version 0.0
*/
class ReporteGlobal {
  public:
	/**
	*	Contructor por defecto de la clase
	*/
	ReporteGlobal(FuenteDatos *fdd);
	/**
	*	Destructor por defecto
	*/
	~ReporteGlobal(void);
	
	///Puntero o referencia la fuente de datos
	FuenteDatos *fdd_;
	
	///Puntero al archivo del reporte ASCII
	ofstream *archivoASCII;

	///Puntero al archivo del reporte ASCII
	ofstream *archivoLATEX;

	///Fecha de ejecucion
	string fechaEjecucion;

	///Numeracion de los capitulos
	int capituloActual;
	
	/**
	 * Metodo que imprime la portada del reporte
	 * @param nombreArchivo nombre y locacion del archivo donde se imprimira el reporte
	 */
	void ImprimePortada();

	/**
	 * Metodo que imprime el reporte en formato ASCII
	 * @param nombreArchivo nombre y locacion del archivo donde se imprimira el reporte
	 */
	void ImprimePortadaASCII();

	/**
	 * Metodo que imprime el reporte en formato LATEX
	 * @param nombreArchivo nombre y locacion del archivo donde se imprimira el reporte
	 */
	void ImprimePortadaLATEX();

	/**
	 * Metodo que inserta un string en el archivo del reporte
	 * @param texto texto a insertar en el archivo
	 */
	void InsertaTexto(string textoASCII,string textoLATEX);
	
	/**
	 * Metodo que abre el archivo
	 */
	void AbreArchivo();
	
	/**
	 * Metodo que cierra el archivo de reporte
	 */
	void CierraArchivo();

	/**
	* Metodo que entrega el capitulo
	*/
	int Capitulo();
};