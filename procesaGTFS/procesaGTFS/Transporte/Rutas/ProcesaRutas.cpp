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
#include "ProcesaRutas.h"


ProcesaRutas::ProcesaRutas(FuenteDatos *fdd)
{
	this->fdd_ = fdd;

	MejoraExtremos();
}

ProcesaRutas::~ProcesaRutas(void)
{
}

void ProcesaRutas::MejoraExtremos()
{
	int nTimeStart = Cronometro::GetMilliCount();

	cout << "Proceso X : Mejorando extremos de las rutas... " ;

	
	///Ciclo sobre secuencia de paradas
	for (map< string, map < int, Paradero > >::iterator isec = fdd_->secParaderos.secuencias.begin(); isec != fdd_->secParaderos.secuencias.end();isec++)
	{
		///1.- Deteccion de rutas con problemas
		map<string, Ruta>::iterator iruta = fdd_->rutas.mapeo->find((*isec).first);
		
		if (iruta != fdd_->rutas.mapeo->end())
		{
			//map<int, Vector3D> *nodosSimplificados;
			//pair<int, int> ObtenerDistanciasRutas(Vector3D *p);
		}
	}


//	cout <<reporte->tCrucePosExpediciones  << "(min)" << endl;
}