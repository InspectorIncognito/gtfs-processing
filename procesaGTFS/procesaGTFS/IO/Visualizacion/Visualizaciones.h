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

#include <limits>
#include "CreaKML.h"
#include "../FuenteDatos.h"
#include "../Parametros.h"
#include "../../Utilidades/ConvertCoordinate.h"
#include "../../Utilidades/Cronometro.h"
#include "../../Transporte/Paraderos/RedParadas.h"

/**
* Title: Visualizaciones
* Description:	Clase contenedora de variables y metodos para generar distintas visualizaciones de los datos
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* @version 0.0
*/
class Visualizaciones{
public:
	/**
	 * Constructor por defecto
	 */
	Visualizaciones();
	/**
	 * Constructor
	 * @param expediciones_ referencia a expediciones que seran graficadas
	 * @param p_ referencia a los parametros externos
	 * @param rutas_ referencia a objeto con las rutas del sistema
	 * @param diccionario_ referencia a objeto con los diccionarios de servicio-sentido entre las tablas
	 * @param transacciones_ referencia a objeto con las transacciones del sistema
	 * @param paraderos_ referencia a objeto con la secuencia de paraderos del sistema
	 */
	Visualizaciones(FuenteDatos *fdd);
	/**
	 * Destructor por defecto
	 */
	~Visualizaciones();

	///Referencia a la fuente de datos
	FuenteDatos *fdd_;

	/**
	 * Metodo que dibuja en un archivo con formato kml, las distintas caracteristicas requeridas
	 */
	void DibujaGoogleEarth();
	
	/**
	* Metodo que dibuja una ruta del servicio indicado en el archivo kml
	* @param rutas referencia a objeto con las rutas del sistema
	* @param serviciosentido codigo del servicio sentido para a tabla de rutas
	* @param kml referencia a archivo kml donde se hara el dibujo
	*/
	void DibujaServicio(Ruta& ruta,map<int,Paradero>& secuencia, string serviciosentido,CreaKML *kml);

	void DibujaRedParadas(CreaKML *kml);

	void DibujaRedPuntosBips(CreaKML *kml);

	//void DibujaRedRutas(CreaKML *kml);

	void DibujaGrillaParadas(CreaKML *kml);

};

