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
#include "TablaServiciosPorParadaParaAndriod.h"


TablaServiciosPorParadaParaAndriod::TablaServiciosPorParadaParaAndriod(FuenteDatos *fdd)
{
	this->fdd_ = fdd;

	Crear();
}

TablaServiciosPorParadaParaAndriod::~TablaServiciosPorParadaParaAndriod()
{

}

void TablaServiciosPorParadaParaAndriod::Crear()
{
	int nTimeStart = Cronometro::GetMilliCount();

	cout << "Proceso X : Creando e imprimiendo tabla servicios... " ;

	///Construccion del dato
	map<string, map<string,int> > output;
	map<string, map<string, int> >::iterator it;
	map<string, string>::iterator iDicServ;
	for (map< string, map < int, Paradero > >::iterator iserv = fdd_->secParaderosTODOS.secuencias.begin(); iserv != fdd_->secParaderosTODOS.secuencias.end(); iserv++)
	{

		for (map < int, Paradero >::iterator ipar = (*iserv).second.begin(); ipar != (*iserv).second.end(); ipar++)
		{
			iDicServ = fdd_->dicSS.servicios.find((*iserv).first);

			if (iDicServ == fdd_->dicSS.servicios.end())
				continue;

			it = output.find((*ipar).second.codigo);

			if (it == output.end())
			{
				map<string, int> tmp;
				tmp[(*iDicServ).second] = 1;
				output[(*ipar).second.codigo] = tmp;
			}
			else
			{
				(*it).second[(*iDicServ).second] = 1;
			}

		}
	}

	///Impresion de la tabla
	ofstream fileout;
	fileout.open("Android_busstops" + fdd_->parametros->version + ".csv");
	fileout << "code;";
	fileout << "name;";
	fileout << "latitude;";
	fileout << "longitude;";
	fileout << "services";
	fileout << endl;
	fileout.precision(10);

	for (it = output.begin(); it != output.end(); it++)
	{
		map< string, Paradero >::iterator ired = fdd_->redParaderos.red.find((*it).first);

		if (ired != fdd_->redParaderos.red.end())
		{
			double lat, lon;

			ConvertCoordinate::UTMtoLL(23, (*ired).second.y, (*ired).second.x, UTMZONE, lat, lon);

			string strlat = StringFunctions::Double2String(lat, 10);
			string strlon = StringFunctions::Double2String(lon, 10);

			fileout << (*ired).second.codigo<< ";";
            
            //fileout << StringFunctions::toCamelCase((*ired).second.nombre) << ";";
            fileout << StringFunctions::toCamelCase(StringFunctions::EliminaCadenasBlancos((*ired).second.nombre)) << ";";

// 			vector<string> nombre_ = StringFunctions::Explode((*ired).second.nombre, '-');

// 			string nombre = string("");
// 			for (int i = 1; i < nombre_.size(); i++)
// 				nombre.append(nombre_.at(i));

// 			fileout << EliminaCadenasBlancos(nombre) << ";";
			fileout << lat << ";";
			fileout << lon << ";";
		}
		else
		{
			cout << "ADVERTENCIA :  no se encuentra el paradero " << (*it).first << " en la red." << endl;
		}

		for (map<string, int>::iterator it1 = (*it).second.begin(); it1 != (*it).second.end(); it1++)
		{
			if(it1== (*it).second.begin())
				fileout << (*it1).first ;
			else
				fileout << "-" << (*it1).first;
		}
		fileout << endl;
		
	}
	fileout.close();

	cout << Cronometro::GetMilliSpan( nTimeStart )/60000.0 << "(min)" << endl;
}
