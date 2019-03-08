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
#include "TablaGrilla.h"

TablaGrilla::TablaGrilla(FuenteDatos *fdd)
{
	this->fdd_ = fdd;

	Crear();
}

TablaGrilla::~TablaGrilla()
{

}

void TablaGrilla::Crear()
{
	int nTimeStart = Cronometro::GetMilliCount();

	cout << "Proceso X : Creando e imprimiendo tabla de grilla... " ;

	/////Impresion de la tabla
	//ofstream fileout;
	//fileout.open("grid.csv");
	//fileout << "iLat;";
	//fileout << "iLon;";
	//fileout << "stops;";
	//fileout << "services;";
	//fileout << "route_segments";
	//fileout << endl;

	//for (int iLat = 0; iLat < fdd_->grid.cells.size() ; iLat++)
	//{
	//	for (int iLon = 0; iLon < fdd_->grid.cells.at(iLat).size(); iLon++)
	//	{
	//		fileout << iLat << ";";
	//		fileout << iLon << ";";
	//		for (vector<string>::iterator istops = fdd_->grid.cells.at(iLat).at(iLon).stops.begin(); istops != fdd_->grid.cells.at(iLat).at(iLon).stops.end(); istops++)
	//		{
	//			if(istops== fdd_->grid.cells.at(iLat).at(iLon).stops.begin())
	//				fileout << (*istops);
	//			else
	//				fileout << "-" << (*istops);
	//		}
	//		fileout << ";";
	//		for (map< string, map<int, int> >::iterator iroute = fdd_->grid.cells.at(iLat).at(iLon).routesSegments.begin(); iroute != fdd_->grid.cells.at(iLat).at(iLon).routesSegments.end(); iroute++)
	//		{
	//			if (iroute == fdd_->grid.cells.at(iLat).at(iLon).routesSegments.begin())
	//				fileout << (*iroute).first;
	//			else
	//				fileout << "-" << (*iroute).first;
	//		}
	//		fileout << ";";
	//		for (map< string, map<int, int> >::iterator iroute = fdd_->grid.cells.at(iLat).at(iLon).routesSegments.begin(); iroute != fdd_->grid.cells.at(iLat).at(iLon).routesSegments.end(); iroute++)
	//		{
	//			if (iroute != fdd_->grid.cells.at(iLat).at(iLon).routesSegments.begin())
	//				fileout << "/";

	//			for (map<int, int>::iterator isegment = (*iroute).second.begin(); isegment != (*iroute).second.end(); isegment++)
	//			{
	//				if (isegment == (*iroute).second.begin())
	//				{
	//					fileout << (*isegment).first;
	//				}
	//				else
	//				{
	//					fileout << "-" << (*isegment).first;
	//				}
	//			}
	//			

	//		}
	//		fileout << endl;
	//		
	//	}
	//}
	//fileout.close();

	///Impresion de la tabla
	ofstream fileout1;
	fileout1.open("Android_grid" + fdd_->parametros->version + ".csv");
	fileout1 << "iLat;";
	fileout1 << "iLon;";
	fileout1 << "stops;";
	fileout1 << "services;";
	fileout1 << "route_segments;";
	fileout1 << "puntos";
	fileout1 << endl;

	for (int iLat = 0; iLat < fdd_->grid.cells.size(); iLat++)
	{
		for (int iLon = 0; iLon < fdd_->grid.cells.at(iLat).size(); iLon++)
		{
			if (!fdd_->grid.cells.at(iLat).at(iLon).stops.empty() || !fdd_->grid.cells.at(iLat).at(iLon).routesSegments.empty() || !fdd_->grid.cells.at(iLat).at(iLon).puntos.empty())
			{
				fileout1 << iLat << ";";
				fileout1 << iLon << ";";
				
				///Paradas
				for (vector<string>::iterator istops = fdd_->grid.cells.at(iLat).at(iLon).stops.begin(); istops != fdd_->grid.cells.at(iLat).at(iLon).stops.end(); istops++)
				{
					if (istops == fdd_->grid.cells.at(iLat).at(iLon).stops.begin())
						fileout1 << (*istops);
					else
						fileout1 << "/" << (*istops);
				}
				fileout1 << ";";

				///Rutas
				for (map< string, map<int, int> >::iterator iroute = fdd_->grid.cells.at(iLat).at(iLon).routesSegments.begin(); iroute != fdd_->grid.cells.at(iLat).at(iLon).routesSegments.end(); iroute++)
				{
					if (iroute == fdd_->grid.cells.at(iLat).at(iLon).routesSegments.begin())
						fileout1 << (*iroute).first;
					else
						fileout1 << "/" << (*iroute).first;
				}
				fileout1 << ";";
				for (map< string, map<int, int> >::iterator iroute = fdd_->grid.cells.at(iLat).at(iLon).routesSegments.begin(); iroute != fdd_->grid.cells.at(iLat).at(iLon).routesSegments.end(); iroute++)
				{
					if (iroute != fdd_->grid.cells.at(iLat).at(iLon).routesSegments.begin())
						fileout1 << "/";

					for (map<int, int>::iterator isegment = (*iroute).second.begin(); isegment != (*iroute).second.end(); isegment++)
					{
						if (isegment == (*iroute).second.begin())
						{
							fileout1 << (*isegment).first;
						}
						else
						{
							fileout1 << "-" << (*isegment).first;
						}
					}
				}
				fileout1 << ";";
				///Puntos
				for (vector<int>::iterator istops = fdd_->grid.cells.at(iLat).at(iLon).puntos.begin(); istops != fdd_->grid.cells.at(iLat).at(iLon).puntos.end(); istops++)
				{
					if (istops == fdd_->grid.cells.at(iLat).at(iLon).puntos.begin())
						fileout1 << (*istops);
					else
						fileout1 << "/" << (*istops);
				}
				fileout1 << endl;
			}

		}
	}
	fileout1.close();

	cout << Cronometro::GetMilliSpan( nTimeStart )/60000.0 << "(min)" << endl;
}
