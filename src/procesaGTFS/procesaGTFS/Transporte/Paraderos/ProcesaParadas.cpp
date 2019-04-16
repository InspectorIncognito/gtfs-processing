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
#include "ProcesaParadas.h"


ProcesaParadas::ProcesaParadas(FuenteDatos *fdd)
{
	this->fdd_ = fdd;

	CorrigeParadasMismaPosicion();
}

ProcesaParadas::~ProcesaParadas(void)
{
}

void ProcesaParadas::CorrigeParadasMismaPosicion()
{
	int nTimeStart = Cronometro::GetMilliCount();

	cout << "Proceso X : Correccion de paradas misma posicion... " ;

	
	///Ciclo sobre secuencia de paradas
	for (map<string, Paradero>::iterator ipar1 = fdd_->redParaderos.red.begin(); ipar1 != fdd_->redParaderos.red.end();ipar1++)
	{
		for (map<string, Paradero>::iterator ipar2 = fdd_->redParaderos.red.begin(); ipar2 != fdd_->redParaderos.red.end(); ipar2++)
		{
			if ((*ipar1).first.compare((*ipar2).first) != 0)
			{
				float dx = (*ipar1).second.x - (*ipar2).second.x;
				float dy = (*ipar1).second.y - (*ipar2).second.y;
				float dist = sqrt(dx*dx + dy*dy);

				if (dist < 5)
				{
					cout << (*ipar1).first << "|" << (*ipar2).first << "|" << dist << endl;
				}

			}
		}

	}


//	cout <<reporte->tCrucePosExpediciones  << "(min)" << endl;
}