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
#include "TablaDistanciaEnRutaParadas.h"

TablaDistanciaEnRutaParadas::TablaDistanciaEnRutaParadas(FuenteDatos *fdd)
{
	this->fdd_ = fdd;

	Crear();
}

TablaDistanciaEnRutaParadas::~TablaDistanciaEnRutaParadas()
{

}

void TablaDistanciaEnRutaParadas::Crear()
{
	int nTimeStart = Cronometro::GetMilliCount();

	cout << "Proceso X : Creando e imprimiendo tabla de distancias en ruta de paradas... " ;

	///Impresion de la tabla
	ofstream fileout;
	fileout.open("servicestopdistance.csv");
	fileout << "paradero;";
	fileout << "servicio;";
	fileout << "distancia";
	fileout << endl;

	map<string, string>::iterator iDicServ;
	map<string, Ruta>::iterator iRuta;
	int id = 0;
	for (map< string, map < int, Paradero > >::iterator iserv = fdd_->secParaderos.secuencias.begin(); iserv != fdd_->secParaderos.secuencias.end(); iserv++)
	{
		for (map < int, Paradero >::iterator ipar = (*iserv).second.begin(); ipar != (*iserv).second.end(); ipar++)
		{
			//fileout << ++id << ";";
			fileout << (*ipar).second.codigo << ";";
			fileout << (*iserv).first << ";";

			iRuta = fdd_->rutas.mapeo->find((*iserv).first);

			Vector3D p = Vector3D((*ipar).second.x, (*ipar).second.y, 0.0);
			float distance = -1;
			if (iRuta != fdd_->rutas.mapeo->end())
				distance = (*iRuta).second.GetDistanceOnRoute(&p);
			
			fileout << int(distance) << endl;
			
			

		}
	}
	fileout.close();


	cout << Cronometro::GetMilliSpan( nTimeStart )/60000.0 << "(min)" << endl;
}
