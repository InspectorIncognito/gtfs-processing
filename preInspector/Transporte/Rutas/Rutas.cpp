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
#include "Rutas.h"

Rutas::Rutas(void)
{
	this->mapeo = new map<string,Ruta>;
}

Rutas::~Rutas(void)
{
}

void Rutas::CalculaTramificado(double distancia)
{
	for (map<string, Ruta>::iterator iruta = mapeo->begin(); iruta != mapeo->end(); iruta++)
		(*iruta).second.GetTramado(distancia);
}

void Rutas::CalculaLargosRutas()
{
	for(map<string,Ruta>::iterator iruta = mapeo->begin(); iruta!= mapeo->end() ; iruta++)
	{
		map<int,Vector3D>::iterator iFin = (*iruta).second.nodos->end();
		iFin--;
		(*iruta).second.largoTotal = (*iruta).second.GetDistanceOnRoute(&((*iFin).second));
	}
}


pair<string,int> Rutas::RutaMasLarga()
{
	string nombre = string("-");
	int largo = numeric_limits<int>::min();
	
	for(map<string,Ruta>::iterator iruta = mapeo->begin(); iruta!= mapeo->end() ; iruta++)
	{
		if((*iruta).second.largoTotal >= largo)
		{
			nombre = (*iruta).first;
			largo = (*iruta).second.largoTotal;
		}
	}
	
	return pair<string,int>(nombre,largo);
}

pair<string,int> Rutas::RutaMasCorta()
{
	string nombre = string("-");
	int largo = numeric_limits<int>::max();
	
	for(map<string,Ruta>::iterator iruta = mapeo->begin(); iruta!= mapeo->end() ; iruta++)
	{
		if((*iruta).second.largoTotal <= largo)
		{
			nombre = (*iruta).first;
			largo = (*iruta).second.largoTotal;
		}
	}
	
	return pair<string,int>(nombre,largo);
}

map< string, int> Rutas::ChequeoGeometrico()
{
	///Mapa que guarda las rutas que contienen algun tipo de error
	map< string, int> rutasNodosFueraDeRuta;

	///Ciclo sobre todas las rutas
	for(map<string,Ruta>::iterator iruta = mapeo->begin();iruta != mapeo->end(); iruta++)
	{
		///Ciclo sobre los nodos de cada ruta
		map<int,Vector3D>::iterator inodoAnt,inodoSig;
		for(map<int,Vector3D>::iterator inodo=(*iruta).second.nodos->begin();inodo!=(*iruta).second.nodos->end();inodo++)
		{
			inodoAnt=inodo;
			inodoSig=inodo;
			inodoAnt--;
			inodoSig++;
			
			///Chequeo de distancia del nodo respecto al segmento formado con el anterior y siguiente nodo
			if(inodoAnt!=(*iruta).second.nodos->end()
			&& inodoSig!=(*iruta).second.nodos->end())
			{
				double dist = Geometry::DistPointSegment(&((*inodo).second), &((*inodoAnt).second), &((*inodoSig).second));
				
				if(dist > 200.0)
				{
					map< string, int>::iterator it=rutasNodosFueraDeRuta.find((*iruta).first);

					if(it==rutasNodosFueraDeRuta.end())
					{
						rutasNodosFueraDeRuta.insert(pair< string, int>((*iruta).first,1));
					}
					else
					{
						(*it).second++;
					}
				}
			}
		}
	}

	return rutasNodosFueraDeRuta;
}

void Rutas::SimplificaRutas()
{
	for (map<string, Ruta>::iterator iruta = mapeo->begin(); iruta != mapeo->end(); iruta++)
		(*iruta).second.GetSimplifyRoute(4.0);
}