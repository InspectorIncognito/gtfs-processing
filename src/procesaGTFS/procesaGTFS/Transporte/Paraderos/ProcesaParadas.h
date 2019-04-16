/**
* Project: "pvm".
* Package: Transporte
* Class: ProcesaPosicionamiento
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

#include <string>
#include <iostream>
#include "../../Utilidades/TimeStampHandler.h"
#include "../../Utilidades/Cronometro.h"
#include "../../IO/FuenteDatos.h"
#include "Paradero.h"

using namespace std;

/**
* Title: Procesa Rutas
* Description:	Clase con metodos para cruzar el posicionamiento con la asignacion de servicio
* Copyright:  Pragxis (c) 2013
* Date: 15-04-2013
* @author Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* @version 0.0
*/
class ProcesaParadas
{
public:
	/**
	* Constructor
	*/
	ProcesaParadas(FuenteDatos *fdd);
	/**
	* Destructor
	*/
	~ProcesaParadas(void);

	///Referencia a la fuente de datos externos
	FuenteDatos *fdd_;

	/**
	*	Extiende extremos de ruta, la idea es que las rutas van dsdes el primer paradero al ultimo
	*   sin dejar paradas fuera de ruta en lso extremos
	*/
	void CorrigeParadasMismaPosicion();
};
