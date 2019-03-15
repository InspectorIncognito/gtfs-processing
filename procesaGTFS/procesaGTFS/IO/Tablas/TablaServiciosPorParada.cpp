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
#include "TablaServiciosPorParada.h"

TablaServiciosPorParada::TablaServiciosPorParada(FuenteDatos *fdd)
{
	this->fdd_ = fdd;

	Crear();
}

TablaServiciosPorParada::~TablaServiciosPorParada()
{

}

void TablaServiciosPorParada::Crear()
{
	int nTimeStart = Cronometro::GetMilliCount();

	cout << "Proceso X : Creando e imprimiendo tabla servicios... " ;

	///Construccion del dato
	///output == parada, servicioSentido - Sentido
	map<string, map<string,string> > output;
	map<string, map<string, string> >::iterator it;
	map<string, string>::iterator is;
	map<string, string>::iterator iDicServ;
	for (map< string, map < int, Paradero > >::iterator iserv = fdd_->secParaderosTODOS.secuencias.begin(); iserv != fdd_->secParaderosTODOS.secuencias.end(); iserv++)
	{
		for (map < int, Paradero >::iterator ipar = (*iserv).second.begin(); ipar != (*iserv).second.end(); ipar++)
		{
			iDicServ = fdd_->dicSS.servicios.find((*iserv).first);

			it = output.find((*ipar).second.codigo);

			string servicio = (*iserv).first.substr(0,(*iserv).first.length() - 1);
			string sentido = (*iserv).first.substr((*iserv).first.length() - 1, 1);

			///Caso de ser el primero del servicio no ingresar
			if (it == output.end())
			{
				map<string, string> tmp;
				tmp[servicio] = sentido;

				output[(*ipar).second.codigo] = tmp;
			}
			else
			{
				is = (*it).second.find(servicio);

				///Caso que el servicio no esta repetido
				if (is == (*it).second.end())
				{
					(*it).second[servicio] = sentido;
				}
				///Caso servicio repetido
				else
				{
					///identifico que no sea el primer paradero de la secuencia
					if ((*ipar).first != 1)
					{
						(*it).second[servicio] = sentido;
					}
				}
			}
		}
	}

	
	///Impresion de la tabla
	ofstream fileout;
	fileout.open("servicesbybusstop" + fdd_->parametros->version + ".csv");
	fileout << "paradas;";
	fileout << "servicios";
	fileout << endl;

	for (it = output.begin(); it != output.end(); it++)
	{
		if ((*it).first.at(0) != 'P')
			continue;

		fileout << (*it).first << ";";
		for (is = (*it).second.begin(); is != (*it).second.end(); is++)
		{
			if (is == (*it).second.begin())
				fileout << (*is).first << (*is).second;
			else
				fileout << "-" << (*is).first << (*is).second;

		}
		fileout << endl;
	}
	fileout.close();
	

	/*
	///Impresion de la tabla
	ofstream fileout;
	fileout.open("servicesbybusstop.csv");
	fileout << "id;";
	fileout << "code;";
	fileout << "service_id;";
	fileout << "busStop_id";
	fileout << endl;

	map<string, string>::iterator iDicServ;
	int id = 0;
	for (map< string, map < int, Paradero > >::iterator iserv = fdd_->secParaderos.secuencias.begin(); iserv != fdd_->secParaderos.secuencias.end(); iserv++)
	{
		for (map < int, Paradero >::iterator ipar = (*iserv).second.begin(); ipar != (*iserv).second.end(); ipar++)
		{
			iDicServ = fdd_->dicSS.servicios.find((*iserv).first);

			fileout << ++id << ";";
			fileout << (*iserv).first << ";";
			if (iDicServ != fdd_->dicSS.servicios.end())
				fileout << (*iDicServ).second << ";";
			else
			{
				cout << "ERROR : Servicio-sentido sin servicio usuario encontrado : " << (*iserv).first << endl;
				fileout << "-" << ";";
			}

			fileout << (*ipar).second.codigo << endl;
			

		}
	}
	fileout.close();
	*/


	cout << Cronometro::GetMilliSpan( nTimeStart )/60000.0 << "(min)" << endl;
}
