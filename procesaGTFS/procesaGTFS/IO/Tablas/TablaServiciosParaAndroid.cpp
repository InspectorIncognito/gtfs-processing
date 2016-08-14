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
#include "TablaServiciosParaAndroid.h"

TablaServiciosParaAndroid::TablaServiciosParaAndroid(FuenteDatos *fdd)
{
	this->fdd_ = fdd;

	Crear();
}

TablaServiciosParaAndroid::~TablaServiciosParaAndroid()
{

}

void TablaServiciosParaAndroid::Crear()
{
	int nTimeStart = Cronometro::GetMilliCount();

	cout << "Proceso X : Creando e imprimiendo tabla servicios para Android... ";

	///Impresion de la tabla
	ofstream fileout;
	fileout.open("Android_services.csv");
	fileout << "service;";
	fileout << "origin;";
	fileout << "destiny;";
	fileout << "color_id;";
	fileout << "stopsI;";
	fileout << "stopsR";
	fileout << endl;

	for (map< string, Servicio >::iterator iserv = fdd_->servicios.begin(); iserv != fdd_->servicios.end(); iserv++)
	{
		fileout << (*iserv).first << ";";

		vector<string> palabrasOrigen = StringFunctions::Explode((*iserv).second.origen, ' ');

		for (int i = 0; i < palabrasOrigen.size(); i++)
		{
			if (i == 0)
				fileout << NormalizaPalabra(palabrasOrigen.at(i));
			else
				fileout << " " << NormalizaPalabra(palabrasOrigen.at(i));
		}
		fileout << ";";

		vector<string> palabrasDestino = StringFunctions::Explode((*iserv).second.destino, ' ');

		for (int i = 0; i < palabrasDestino.size(); i++)
		{
			if (i == 0)
				fileout << NormalizaPalabra(palabrasDestino.at(i));
			else
				fileout << " " << NormalizaPalabra(palabrasDestino.at(i));
		}
		fileout << ";";

		map<string, string>::iterator it = fdd_->dicSS.colores.find((*iserv).first);

		if (it != fdd_->dicSS.colores.end())
			fileout << (*it).second << ";";
		else
		{
			cout << "ERROR : No se encontro el servicio " << (*iserv).first << " en la tabla de colores." << endl;
			fileout << "0" << ";";
		}


		map< string, map < int, Paradero > >::iterator isI = fdd_->secParaderos.secuencias.find((*iserv).first + "I");
		if (isI != fdd_->secParaderos.secuencias.end())
		{
			for (map < int, Paradero >::iterator ipar = (*isI).second.begin(); ipar != (*isI).second.end(); ipar++)
			{
				if (ipar == (*isI).second.begin())
					fileout << (*ipar).second.codigo;
				else
					fileout << "-" << (*ipar).second.codigo;
			}
		}
		fileout << ";";

		map< string, map < int, Paradero > >::iterator isR = fdd_->secParaderos.secuencias.find((*iserv).first + "R");
		if (isR != fdd_->secParaderos.secuencias.end())
		{
			for (map < int, Paradero >::iterator ipar = (*isR).second.begin(); ipar != (*isR).second.end(); ipar++)
			{
				if (ipar == (*isR).second.begin())
					fileout << (*ipar).second.codigo;
				else
					fileout << "-" << (*ipar).second.codigo;
			}
		}
		fileout << endl;

	}
	fileout.close();


	cout << Cronometro::GetMilliSpan(nTimeStart) / 60000.0 << "(min)" << endl;
}


string TablaServiciosParaAndroid::NormalizaPalabra(string in)
{
	std::locale loc;
	std::string out = in;
	for (std::string::size_type i = 0; i < in.length(); ++i)
	{
		//cout << i << " | " << in << endl;

		if (i == 0)
			out[i] = in[i];
		else
			out[i] = std::tolower(in[i], loc);
	}

	///Dteccion de simbolo metro
	std::size_t found = out.find("(m)");
	if (found != std::string::npos)
	{
		out.replace(found, 3, "(M)");
	}
		

	return out;
}