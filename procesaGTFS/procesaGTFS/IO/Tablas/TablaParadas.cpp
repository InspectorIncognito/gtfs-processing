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
#include "TablaParadas.h"

TablaParadas::TablaParadas(FuenteDatos *fdd)
{
	this->fdd_ = fdd;

	Crear();
}

TablaParadas::~TablaParadas()
{

}

void TablaParadas::Crear()
{
	int nTimeStart = Cronometro::GetMilliCount();

	cout << "Proceso X : Creando e imprimiendo tabla de paradas... " ;

	///Impresion de la tabla
	ofstream fileout;
	fileout.open("busstop" + fdd_->parametros->version + ".csv");
	fileout.precision(10);
	

	///Escribo el header
	fileout << "stop_code" << ";";
	fileout << "stop_name" << ";";
	fileout << "stop_lat" << ";";
	fileout << "stop_lon" << endl;

	///Ciclo sobre todas las secuencia de paraderos
	for (map< string, Paradero >::iterator ired = fdd_->redParaderos.red.begin(); ired != fdd_->redParaderos.red.end(); ired++)
	{
		double lat, lon;

		ConvertCoordinate::UTMtoLL(23, (*ired).second.y, (*ired).second.x, UTMZONE, lat, lon);

		string strlat = StringFunctions::Double2String(lat, 10);
		string strlon = StringFunctions::Double2String(lon, 10);

		fileout << (*ired).second.codigo << ";";
		fileout << (*ired).second.nombre << ";";
		fileout << strlat << ";";
		fileout << strlon << endl;
		


	}
	fileout.close();


	cout << Cronometro::GetMilliSpan( nTimeStart )/60000.0 << "(min)" << endl;
}
