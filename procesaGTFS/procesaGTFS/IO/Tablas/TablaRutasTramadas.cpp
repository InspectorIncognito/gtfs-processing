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
#include "TablaRutasTramadas.h"

TablaRutasTramadas::TablaRutasTramadas(FuenteDatos *fdd)
{
	this->fdd_ = fdd;

	Crear();
}

TablaRutasTramadas::~TablaRutasTramadas()
{

}

void TablaRutasTramadas::Crear()
{
	int nTimeStart = Cronometro::GetMilliCount();

	cout << "Proceso X : Creando e imprimiendo tabla de rutas tramadas... " ;

	///Impresion de la tabla
	ofstream fileout;
	fileout.open("servicelocation.csv");
	fileout << "servicio;";
	fileout << "distancia;";
	fileout << "latitud;";
	fileout << "longitud";
	fileout << endl;



	int id = 0;
	map<string, Ruta>::iterator iRuta;
	for (map<string, Ruta>::iterator iruta = fdd_->rutas.mapeo->begin(); iruta != fdd_->rutas.mapeo->end(); iruta++)
	{
		for (map<float, Vector3D>::iterator inodo = (*iruta).second.tramado.begin(); inodo != (*iruta).second.tramado.end(); inodo++)
		{
			double lat, lon;

			ConvertCoordinate::UTMtoLL(23, (*inodo).second.y, (*inodo).second.x, UTMZONE, lat, lon);

			string strlat = StringFunctions::Double2String(lat, 10);
			string strlon = StringFunctions::Double2String(lon, 10);

			fileout << (*iruta).first << ";";
			fileout << int((*inodo).first) << ";";
			fileout << strlat << ";";
			fileout << strlon << endl;
		}
	}

	fileout.close();


	cout << Cronometro::GetMilliSpan( nTimeStart )/60000.0 << "(min)" << endl;
}
