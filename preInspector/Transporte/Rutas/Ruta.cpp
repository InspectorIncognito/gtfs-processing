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
#include "Ruta.h"

Ruta::Ruta()
{
	this->nodos = new map<int,Vector3D>;
}

Ruta::Ruta(string nombre_)
{
	this->nombre = nombre_;
	this->nodos = new map<int,Vector3D>;
	this->nodosSimplificados = new map<int, Vector3D>;
}

Ruta::~Ruta()
{
}

bool Ruta::AgregarNodo(Vector3D p, int correlativo)
{
	map<int,Vector3D>::iterator inodo;
	inodo = this->nodos->find(correlativo);

	if(inodo==this->nodos->end())
	{
		this->nodos->insert(pair<int,Vector3D>(correlativo,p));

		return true;
	}

	return false;
}

pair<int,int> Ruta::ObtenerDistanciasRutas(Vector3D *p)
{
	map<int,Vector3D>::iterator inodo1;
	map<int,Vector3D>::iterator inodo2;
	inodo1=this->nodos->begin();
	inodo2=inodo1;
	inodo2++;

	Vector3D projection = Geometry::PointProjectToSegment(p, &((*inodo1).second), &((*inodo2).second));
	double distToRoute_ = Geometry::DistPoint2Point(&projection,p); 
	double distAcu=0.0;
	double distEnRuta_=0.0;

	int i=0;
	for (inodo1 = this->nodos->begin(); inodo1 != this->nodos->end(); inodo1++)
	{
		i++;
		inodo2=inodo1;
		inodo2++;

		if (inodo2 == this->nodos->end())
			break;

		Vector3D projectAux = Geometry::PointProjectToSegment(p, &((*inodo1).second), &((*inodo2).second));
		
		double dist = Geometry::DistPoint2Point(&projectAux,p); 
		if(distToRoute_ >= dist)
		{
			distToRoute_ = dist;
			projection = projectAux;
			distEnRuta_ = Geometry::DistPoint2Point(&((*inodo1).second),&projection) + distAcu;
		}
		distAcu+=Geometry::DistPoint2Point(&((*inodo1).second), &((*inodo2).second));
	}

	pair<int,int> output;
	output.first = int(distEnRuta_);
	output.second = int(distToRoute_);

	return output;

}

double Ruta::GetDistanceOnRoute(Vector3D *p)
{
	map<int,Vector3D>::iterator inodo1;
	map<int,Vector3D>::iterator inodo2;
	inodo1 = this->nodos->begin();
	inodo2=inodo1;
	inodo2++;

	Vector3D projection = Geometry::PointProjectToSegment(p, &((*inodo1).second), &((*inodo2).second));
	double distToRoute = Geometry::DistPoint2Point(&projection,p); 
	double distAcu=0.0;
	double distRuta=0.0;

	int i=0;
	for (inodo1 = this->nodos->begin(); inodo1 != this->nodos->end(); inodo1++)
	{
		i++;
		inodo2=inodo1;
		inodo2++;

		if (inodo2 == this->nodos->end())
			break;

		Vector3D projectAux = Geometry::PointProjectToSegment(p, &((*inodo1).second), &((*inodo2).second));
		
		double dist = Geometry::DistPoint2Point(&projectAux,p); 
		if(distToRoute >= dist)
		{
			distToRoute = dist;
			projection = projectAux;
			distRuta= Geometry::DistPoint2Point(&((*inodo1).second),&projection) + distAcu;
		}
		distAcu+=Geometry::DistPoint2Point(&((*inodo1).second), &((*inodo2).second));
	}

	return distRuta;

}

double Ruta::GetDistanceToRoute(Vector3D *p)
{
	map<int,Vector3D>::iterator inodo1;
	map<int,Vector3D>::iterator inodo2;
	inodo1 = this->nodos->begin();
	inodo2=inodo1;
	inodo2++;

	Vector3D projection = Geometry::PointProjectToSegment(p, &((*inodo1).second), &((*inodo2).second));
	double distToRoute = Geometry::DistPoint2Point(&projection,p); 
	double distAcu=0.0;
	double distRuta=0.0;

	int i=0;
	for (inodo1 = this->nodos->begin(); inodo1 != this->nodos->end(); inodo1++)
	{
		i++;
		inodo2=inodo1;
		inodo2++;

		if (inodo2 == this->nodos->end())
			break;

		Vector3D projectAux = Geometry::PointProjectToSegment(p, &((*inodo1).second), &((*inodo2).second));
		
		double dist = Geometry::DistPoint2Point(&projectAux,p); 
		if(distToRoute >= dist)
		{
			distToRoute = dist;
			projection = projectAux;
			distRuta= Geometry::DistPoint2Point(&((*inodo1).second),&projection) + distAcu;
		}
		distAcu+=Geometry::DistPoint2Point(&((*inodo1).second), &((*inodo2).second));
	}

	return distToRoute;

}

void Ruta::GetSimplifyRoute(double tolerance)
{
	vector<Vector3D> antes;
	vector<Vector3D> despues;

	for (map<int, Vector3D>::iterator it = nodos->begin(); it != nodos->end(); it++)
		antes.push_back((*it).second);

	Geometry::poly_simplify(tolerance, &antes, &despues);

	int i = 0;
	for (vector<Vector3D>::iterator it = despues.begin(); it != despues.end(); it++, i++)
		nodosSimplificados->insert(pair<int, Vector3D>(i, (*it)));
}

double Ruta::GetTramado(double distance)
{
	float i = 0;

	map<int, Vector3D>::iterator inodo = nodos->begin();
	tramado[i] = (*inodo).second;

	float accum = distance;
	for (accum = distance; accum < largoTotal; accum += distance)
	{
		tramado[accum] = pointAtDistance(accum);
	}

	map<int, Vector3D>::reverse_iterator irnodo = this->nodos->rbegin();
	tramado[accum] = (*irnodo).second;


	return 0;
}

Vector3D Ruta::pointAtDistance(double distance)
{
	map<int, Vector3D>::iterator inodoIni = nodos->begin();
	map<int, Vector3D>::reverse_iterator inodoFin = this->nodos->rbegin();

	if (distance <= 0.0)
		return (*inodoIni).second;
	if (distance >= largoTotal)
		return (*inodoFin).second;

	double accum = 0.0;
	map<int, Vector3D>::iterator inodo_sgt;
	for (map<int, Vector3D>::iterator inodo = nodos->begin(); inodo != nodos->end(); inodo++)
	{
		inodo_sgt = inodo;
		inodo_sgt++;

		if (inodo_sgt != nodos->end())
		{
			double length = Geometry::DistPoint2Point(&((*inodo).second), &((*inodo_sgt).second));	
			double prev = accum;
			accum += length;

			if (distance <= accum && distance >= prev)
			{
				double t = (distance - prev) / length;
				Vector3D v1 = Vector3D((*inodo).second.x * (1.0 - t), (*inodo).second.y * (1.0 - t), (*inodo).second.z * (1.0 - t));
				Vector3D v2 = Vector3D((*inodo_sgt).second.x * t, (*inodo_sgt).second.y * t, (*inodo_sgt).second.z * t);

				return  Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
			}
		}
	}

	return Vector3D();
}