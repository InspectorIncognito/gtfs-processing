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
#pragma once

#include <map>
#include <cstdlib>
#include <string>
#include <iostream>
#include "../../Utilidades/Geometry2.h"

using namespace std;

/**
* Title: Ruta
* Description:	Clase que modela una ruta de un servicio-sentido-variante
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* @version 0.0
*/
class Ruta{
	public:
		/**
		* Constructor
		*/
		Ruta();
		/**
		* Constructor
		*/
		Ruta(string nombre);
		/**
		* Destructor
		*/
		~Ruta();

		///Codigo del servicio de tabla de rutas
		string nombre;

		///Largo total de la ruta
		float largoTotal;

		///Polilinea que define la ruta
		map<int,Vector3D> *nodos;

		///Polilinea que define la ruta
		map<int, Vector3D> *nodosSimplificados;

		///Nodos que conforman el tramado
		std::map<float, Vector3D> tramado;

		/**
		* Metodo que calcula la distancia en la ruta del vector p
		* @param p vector el cual se proyectara a la uta y se medira distancia en ruta a partir del origen
		* @return distancia en la ruta
		*/
		double GetDistanceOnRoute(Vector3D *p);

		/**
		* Metodo que calcula la distancia a la ruta del vector p
		* @param p vector el cual se proyectara a la ruta y se medira distancia a la ruta
		* @return distancia a la ruta
		*/
		double GetDistanceToRoute(Vector3D *p);

		/**
		* Metodo para obtener las distancias en y hacia la ruta
		* @param p referencia vector que define las coordenadas del punto a calculas las distancias
		* @param x referencia 
		* @return first = Distancia en ruta ; second = Distancia a ruta
		*/
		pair<int,int> ObtenerDistanciasRutas(Vector3D *p); 

		/**
		* Metodo que inserta un nuevo nodo en la ruta (polilinea)
		* @param p nuevo nodo a insertar
		* @param correlativo indicador de la posicion correlativa del ultimo nodo insertado
		* @return verdadero si el nodo fue agregado
		*/
		bool AgregarNodo(Vector3D p,int correlativo);

		double GetTramado(double distance);
		Vector3D pointAtDistance(double distance);

		/**
		* Metodo que elimina nodos de la ruta realmacenandola en nodos
		*/
		void GetSimplifyRoute(double tolerance);
};
