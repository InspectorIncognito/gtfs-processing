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
#include "Servicio.h"

Servicio::Servicio(void)
{

}

Servicio::~Servicio(void)
{
}

Servicio::Servicio(string nombre_, string origen_, string destino_, string color_)
{
	nombre = nombre_;
	origen = origen_;
	destino = destino_;
	color = color_;
	horarioI = ("");
	horarioR = ("");
}
