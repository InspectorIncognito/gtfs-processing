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
#include "TablaServicios.h"

TablaServicios::TablaServicios(FuenteDatos *fdd)
{
	this->fdd_ = fdd;

	Crear();
}

TablaServicios::~TablaServicios()
{

}

void TablaServicios::Crear()
{
	int nTimeStart = Cronometro::GetMilliCount();

	cout << "Proceso X : Creando e imprimiendo tabla servicios... " ;

	///Impresion de la tabla
	ofstream fileout;
	fileout.open(string(fdd_->parametros->carpetaOutput + "/" + fdd_->parametros->version + "/" + "ServerServices.csv").c_str());
	//fileout.open("services" + fdd_->parametros->version + ".csv");
	fileout << "service;";
	fileout << "long_name;";
	fileout << "color";
//	fileout << "color_id";
	fileout << endl;

	for (map< string, Servicio >::iterator iserv = fdd_->servicios.begin(); iserv != fdd_->servicios.end(); iserv++)
	{
		fileout << (*iserv).first << ";";
		fileout << (*iserv).second.route_long_name << ";";
		fileout << (*iserv).second.route_color << endl;

//		map<string, string>::iterator it = fdd_->dicSS.colores.find((*iserv).first);

//		if (it != fdd_->dicSS.colores.end())
//			fileout << (*it).second << endl;
//		else
//		{
//			cout << "ERROR : No se encontro el servicio " << (*iserv).first << " en la tabla de colores." << endl;
//			fileout << "0" << endl;
//		}

		//fileout << 1 << endl;
	}
	fileout.close();


	cout << Cronometro::GetMilliSpan( nTimeStart )/60000.0 << "(min)" << endl;
}
