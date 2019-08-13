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

	///Deteccion de paraderos con ambos sentidos
	///output == parada, servicioSentido - Sentido
	map<string, map<string, string> > serviciosPorParada;
	map<string, string> servicioParadaRepetida;
	map<string, map<string, string> >::iterator it;
	map<string, string>::iterator is;
	map<string, string>::iterator iDicServ;
	for (map< string, map < int, Paradero > >::iterator iserv = fdd_->secParaderos.secuencias.begin(); iserv != fdd_->secParaderos.secuencias.end(); iserv++)
	{
		for (map < int, Paradero >::iterator ipar = (*iserv).second.begin(); ipar != (*iserv).second.end(); ipar++)
		{
			iDicServ = fdd_->dicSS.servicios.find((*iserv).first);

			string servicio = (*iserv).first.substr(0, (*iserv).first.length() - 1);
			string sentido = (*iserv).first.substr((*iserv).first.length() - 1, 1);

			it = serviciosPorParada.find((*ipar).second.codigo);

			///Caso de ser el primero del servicio no ingresar
			if (it == serviciosPorParada.end())
			{
				map<string, string> tmp;
				tmp[servicio] = sentido;

				serviciosPorParada[(*ipar).second.codigo] = tmp;
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
					servicioParadaRepetida[servicio] = (*ipar).second.codigo;
				}
			}
		}
	}

	/////DEBUG
	//ofstream dbg;
	//dbg.open("paradasRepetidas.dbg");
	//for (map<string, string>::iterator iit = servicioParadaRepetida.begin(); iit != servicioParadaRepetida.end(); iit++)
	//{
	//	dbg << (*iit).first << "|" << (*iit).second << endl;
	//}
	//dbg.close();


	///Impresion de la tabla
	ofstream fileout;
	fileout.open(string(fdd_->parametros->carpetaOutput + "/" + fdd_->parametros->version + "/" + "ServerServiceStopDistance.csv").c_str());
	fileout << "paradero;";
	fileout << "servicio;";
	fileout << "distancia";
	fileout << endl;

	//map<string, string>::iterator iDicServ;
	map<string, Ruta>::iterator iRuta;
	int id = 0;
	for (map< string, map < int, Paradero > >::iterator iserv = fdd_->secParaderos.secuencias.begin(); iserv != fdd_->secParaderos.secuencias.end(); iserv++)
	{
		for (map < int, Paradero >::iterator ipar = (*iserv).second.begin(); ipar != (*iserv).second.end(); ipar++)
		{
			///Determino si es el primero y ultimo de un servicio, y saco el caso de ser el primero
			string servicio = (*iserv).first.substr(0, (*iserv).first.length() - 1);
			string sentido = (*iserv).first.substr((*iserv).first.length() - 1, 1);

			map<string, string>::iterator iit = servicioParadaRepetida.find(servicio);
			if (iit != servicioParadaRepetida.end() && (*iit).second.compare((*ipar).second.codigo)==0 && ipar == (*iserv).second.begin())
				continue;
				

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
