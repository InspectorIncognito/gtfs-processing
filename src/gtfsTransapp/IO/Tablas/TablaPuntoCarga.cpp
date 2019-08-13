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
#include "TablaPuntoCarga.h"

TablaPuntoCarga::TablaPuntoCarga(FuenteDatos *fdd)
{
	this->fdd_ = fdd;

	Crear();
}

TablaPuntoCarga::~TablaPuntoCarga()
{

}

void TablaPuntoCarga::Crear()
{
	int nTimeStart = Cronometro::GetMilliCount();

	cout << "Proceso X : Creando e imprimiendo tabla de puntos de carga... " ;

	///Impresion de la tabla
	ofstream fileout;
	fileout.open(string(fdd_->parametros->carpetaOutput + "/" + fdd_->parametros->version + "/" + "PhoneFareLoadPoint.csv").c_str());
	//fileout.open("Android_puntoscarga" + fdd_->parametros->version + ".csv");
	fileout.precision(10);
	

	///Escribo el header
	fileout << "punto_id" << ";";
	fileout << "punto_name" << ";";
	fileout << "punto_lat" << ";";
	fileout << "punto_lon" << endl;

	///Ciclo sobre todas las secuencia de paraderos
	for (map< int, PuntoBip >::iterator ipunto = fdd_->puntosDeCargaBip.begin(); ipunto != fdd_->puntosDeCargaBip.end(); ipunto++)
	{
		fileout << (*ipunto).first << ";";
		fileout << (*ipunto).second.nombre << ";";
		fileout << (*ipunto).second.lat << ";";
		fileout << (*ipunto).second.lon << endl;
	}
	fileout.close();


	cout << Cronometro::GetMilliSpan( nTimeStart )/60000.0 << "(min)" << endl;
}
