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

#include <iostream>
#include <string>
#include <vector>
#include <map>


using namespace std;

/**
* Title: Cell
* Description:	Clase que modela un parada de bus
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* @version 0.0
*/
class Cell
{
public:
	/**
	* Constructor
	*/
	Cell(void);
	/**
	* Destructor
	*/
	~Cell(void);

	///Coordinate Latitude
	float minLat;

	///Coordinate Longitude 
	float minLon;

	///bus stops of the cell
	vector<string> stops;

	///bus stops of the cell
	vector<int> puntos;

	///cell's route (servicio, lista de segmentos)
	map< string, map<int,int> > routesSegments;
};
