/**
* Project: "pvm".
* Package: Transporte
* Class: ProcesaPosicionamiento
* Created by: Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* Terms: These coded instructions, statements, and computer programs are
* produced as part of a software project. They contain unpublished
* propietary information and are protected by copyright law. They may
* not be disclosed to third parties or copied or duplicated in any form,
* in whole or in part, without the prior written consent of Pragxis SpA.
* Copyright:  Pragxis (c) 2013
* Last modified : Mauricio Zuñiga 15-04-2013
*/
#include "GridProcess.h"


GridProcess::GridProcess(FuenteDatos *fdd)
{
	this->fdd_ = fdd;

	///Construccion de Grilla
	ConstruyeGrilla();
	//ConstruyeGrillaFija();
	IngresaParaderosAGrilla();
	IngresaRutasAGrilla();
// 	IngresaPuntosBipsAGrilla();
}

GridProcess::~GridProcess(void)
{
}

void GridProcess::ConstruyeGrilla()
{
	int nTimeStart = Cronometro::GetMilliCount();
	cout << "Construyendo grilla regular....";

	///Deteccion de minimos y maximos que define dimensiones de la grilla
	fdd_->grid.minLat = 9999999;
	fdd_->grid.maxLat = -9999999;
	fdd_->grid.minLon = 9999999;
	fdd_->grid.maxLon = -9999999;

	for (map< string, Paradero >::iterator it = fdd_->redParaderos.red.begin(); it != fdd_->redParaderos.red.end(); it++)
	{
		fdd_->grid.minLat = std::fmin(fdd_->grid.minLat, (*it).second.lat);
		fdd_->grid.maxLat = std::fmax(fdd_->grid.maxLat, (*it).second.lat);
		fdd_->grid.minLon = std::fmin(fdd_->grid.minLon, (*it).second.lon);
		fdd_->grid.maxLon = std::fmax(fdd_->grid.maxLon, (*it).second.lon);
	}
	
	fdd_->grid.minLat += 0.001;
	fdd_->grid.maxLat += 0.001;
	fdd_->grid.minLon += 0.001;
	fdd_->grid.maxLon += 0.001;

	fdd_->grid.nLat = 100;
	fdd_->grid.nLon = 100;

	fdd_->grid.delta_Latitud = fdd_->grid.maxLat - fdd_->grid.minLat;
	fdd_->grid.delta_Longitud = fdd_->grid.maxLon - fdd_->grid.minLon;

	fdd_->grid.ddLat = fdd_->grid.delta_Latitud / fdd_->grid.nLat;
	fdd_->grid.ddLon = fdd_->grid.delta_Longitud / fdd_->grid.nLon;

	fdd_->outParameters << "GridMinLatitude;" << fdd_->grid.minLat << endl;
	fdd_->outParameters << "GridMinLongitude;" << fdd_->grid.minLon << endl;
	fdd_->outParameters << "GridDistLatitude;" << fdd_->grid.ddLat << endl;
	fdd_->outParameters << "GridDistLongitude;" << fdd_->grid.ddLon << endl;

	cout << "Min : " << fdd_->grid.minLat << " | " << fdd_->grid.minLon << endl;
	cout << "Max : " << fdd_->grid.maxLat << " | " << fdd_->grid.maxLon << endl;
	cout << "ddLat : " << fdd_->grid.ddLat << endl;
	cout << "ddLon : " << fdd_->grid.ddLon << endl;

	///Llenado de estructura
	for (double dLat = 0; dLat <= fdd_->grid.delta_Latitud; dLat += fdd_->grid.ddLat)
	{
		//cout << "FLAG 0"  << endl;
		vector< Cell > row;
		for (double dLon = 0; dLon <= fdd_->grid.delta_Longitud; dLon += fdd_->grid.ddLon)
		{
			//cout << "FLAG 1" << endl;
			Cell cell;
			row.push_back(cell);
		}
		fdd_->grid.cells.push_back(row);
	}

	cout << fdd_->grid.cells.size() << endl;

	cout << Cronometro::GetMilliSpan(nTimeStart) / 60000.0 << "(min)" << endl;
}

void GridProcess::IngresaParaderosAGrilla()
{
	int nTimeStart = Cronometro::GetMilliCount();
	cout << "Ingresando paradas a grilla regular....";

	//cout << "FLAG 0" << endl;
	for (map< string, Paradero >::iterator it = fdd_->redParaderos.red.begin(); it != fdd_->redParaderos.red.end(); it++)
	{
		//cout << "FLAG 1" << endl;
		///Calculo la celda
		int iLat = ((*it).second.lat - fdd_->grid.minLat) / fdd_->grid.ddLat;
		int iLon = ((*it).second.lon - fdd_->grid.minLon) / fdd_->grid.ddLon;

		//cout << (*it).second.lat << "|" << (*it).second.lon << "|" << iLat << "|" << iLon << endl;

		//cout << "FLAG 2" << endl;
		fdd_->grid.cells.at(iLat).at(iLon).stops.push_back((*it).second.codigo);
		//cout << "FLAG 3" << endl;
	}
	//cout << "FLAG 4" << endl;
	cout << Cronometro::GetMilliSpan(nTimeStart) / 60000.0 << "(min)" << endl;
}

void GridProcess::IngresaRutasAGrilla()
{
	int nTimeStart = Cronometro::GetMilliCount();
	cout << "Ingresando rutas a grilla regular....";

	map<string, string>::iterator iDicServ;
	map< string, map<int, int> >::iterator iCelRuta;
	for (map<string, Ruta>::iterator iruta = fdd_->rutas.mapeo->begin(); iruta != fdd_->rutas.mapeo->end(); iruta++)
	{
		iDicServ = fdd_->dicSS.servicios.find((*iruta).first);

		if (iDicServ == fdd_->dicSS.servicios.end())
			continue;

		///1.- Deteccion de nodos dentro de celda
		for (map<int, Vector3D>::iterator inodo = (*iruta).second.nodosSimplificados->begin(); inodo != (*iruta).second.nodosSimplificados->end(); inodo++)
		{
			double lat, lon;
			ConvertCoordinate::UTMtoLL(23, (*inodo).second.y, (*inodo).second.x, UTMZONE, lat, lon);

			///Calculo la celda
			int iLat = (lat - fdd_->grid.minLat) / fdd_->grid.ddLat;
			int iLon = (lon - fdd_->grid.minLon) / fdd_->grid.ddLon;

			if (iLat >= 0 && iLat < fdd_->grid.cells.size())
			{
				if (iLon >= 0 && iLon < fdd_->grid.cells.at(iLat).size())
				{
					iCelRuta = fdd_->grid.cells.at(iLat).at(iLon).routesSegments.find((*iruta).first);

					if (iCelRuta == fdd_->grid.cells.at(iLat).at(iLon).routesSegments.end())
					{
						///ingreso nodo actual
						map<int, int> tmp;
						tmp[(*inodo).first] = (*inodo).first;

						///Chequeo si existe un nodo anterior, puesto que tambien deberia 
						map<int, Vector3D>::iterator inodo_ant = inodo;
						inodo_ant--;
						if (inodo_ant != (*iruta).second.nodosSimplificados->end())
							tmp[(*inodo_ant).first] = (*inodo_ant).first;

						fdd_->grid.cells.at(iLat).at(iLon).routesSegments[(*iruta).first] = tmp;

					}
					else
					{
						///ingreso nodo actual
						(*iCelRuta).second[(*inodo).first] = (*inodo).first;

						///Chequeo si existe un nodo anterior, puesto que tambien deberia 
						map<int, Vector3D>::iterator inodo_ant = inodo;
						inodo_ant--;
						if (inodo_ant != (*iruta).second.nodosSimplificados->end())
							(*iCelRuta).second[(*inodo_ant).first] = (*inodo_ant).first;
					}
				}
			}
		}

		///1.- Deteccion de nodos dentro de celda
		//              s2
		// (x3,y3)**************(x2,y2)
		//        *            *
		//        *            *
		//    s3  *            *  s1
		//        *            *
		// (x0,y0)**************(x1,y1)
		//              s0
		char UTMZone[5];
		map<int, Vector3D>::iterator inodo_sgt;
		for (map<int, Vector3D>::iterator inodo = (*iruta).second.nodosSimplificados->begin(); inodo != (*iruta).second.nodosSimplificados->end(); inodo++)
		{
			inodo_sgt = inodo;
			inodo_sgt++;

			if (inodo_sgt != (*iruta).second.nodosSimplificados->end())
			{

				double lat0, lon0;
				double lat1, lon1;
				ConvertCoordinate::UTMtoLL(23, (double)(*inodo).second.y, (double)(*inodo).second.x, UTMZONE, lat0, lon0);
				ConvertCoordinate::UTMtoLL(23, (double)(*inodo_sgt).second.y, (double)(*inodo_sgt).second.x, UTMZONE, lat1, lon1);

				///Calculo la celda
				int iLat0 = (lat0 - fdd_->grid.minLat) / fdd_->grid.ddLat;
				int iLon0 = (lon0 - fdd_->grid.minLon) / fdd_->grid.ddLon;

				int iLat1 = (lat1 - fdd_->grid.minLat) / fdd_->grid.ddLat;
				int iLon1 = (lon1 - fdd_->grid.minLon) / fdd_->grid.ddLon;

				//if ((*iruta).first.compare("227R")==0 && (*inodo).first == 7)
				//{
				//	cout << "FLAG 01 : " << (*iruta).first << endl;
				//	cout << "FLAG 02 : " << (*inodo).first << endl;
				//	cout << "FLAG 03 : " << (*inodo_sgt).first << endl;
				//	cout << "FLAG 04 : " << lat0 << " - " << lon0 << endl;
				//	cout << "FLAG 05 : " << lat1 << " - " << lon1 << endl;
				//	cout << "FLAG 06 : " << iLat0 << "," << iLon0 << endl;
				//	cout << "FLAG 07 : " << iLat1 << "," << iLon1 << endl;
				//}

				int minILat = fminf(iLat0, iLat1);
				int minILon = fminf(iLon0, iLon1);
				int maxILat = fmaxf(iLat0, iLat1);
				int maxILon = fmaxf(iLon0, iLon1);

				for (int ilat = minILat; ilat <= maxILat; ilat++)
				{
					for (int ilon = minILon; ilon <= maxILon; ilon++)
					{

						///2.- Deteccion de intersecciones dentro de celda
						double x0, y0;
						double x1, y1;
						double x2, y2;
						double x3, y3;

						///Esquinas
						ConvertCoordinate::LLtoUTM(23, fdd_->grid.minLat + ilat * fdd_->grid.ddLat, fdd_->grid.minLon + ilon * fdd_->grid.ddLon, y0, x0, UTMZone);
						ConvertCoordinate::LLtoUTM(23, fdd_->grid.minLat + ilat * fdd_->grid.ddLat + fdd_->grid.ddLat, fdd_->grid.minLon + ilon * fdd_->grid.ddLon, y1, x1, UTMZone);
						ConvertCoordinate::LLtoUTM(23, fdd_->grid.minLat + ilat * fdd_->grid.ddLat + fdd_->grid.ddLat, fdd_->grid.minLon + ilon * fdd_->grid.ddLon + fdd_->grid.ddLon, y2, x2, UTMZone);
						ConvertCoordinate::LLtoUTM(23, fdd_->grid.minLat + ilat * fdd_->grid.ddLat, fdd_->grid.minLon + ilon * fdd_->grid.ddLon + fdd_->grid.ddLon, y3, x3, UTMZone);

						///Segmentos
						Segmento s0 = Segmento(Vector3D((*inodo).second.x, (*inodo).second.y, 0), Vector3D((*inodo_sgt).second.x, (*inodo_sgt).second.y, 0));
						Segmento s1 = Segmento(Vector3D(x0, y0, 0), Vector3D(x1, y1, 0));
						Segmento s2 = Segmento(Vector3D(x1, y1, 0), Vector3D(x2, y2, 0));
						Segmento s3 = Segmento(Vector3D(x2, y2, 0), Vector3D(x3, y3, 0));
						Segmento s4 = Segmento(Vector3D(x3, y3, 0), Vector3D(x0, y0, 0));

						Vector3D I0, I1;

						bool interseccion = false;
						if (Geometry::intersect2D_Segments(s0, s1, &I0, &I1) > 0) interseccion = true;
						if (Geometry::intersect2D_Segments(s0, s2, &I0, &I1) > 0) interseccion = true;
						if (Geometry::intersect2D_Segments(s0, s3, &I0, &I1) > 0) interseccion = true;
						if (Geometry::intersect2D_Segments(s0, s4, &I0, &I1) > 0) interseccion = true;

						//if ((*iruta).first.compare("227R") == 0 && (*inodo).first == 7)
						//{
						//	cout << "FLAG 1 : " << ilat << "," << ilon << "|" << interseccion << endl;
						//	//cout << "FLAG 1 : " << ilat << "," << ilon << "|" << endl;
						//}

						if (interseccion)
						{
							iCelRuta = fdd_->grid.cells.at(ilat).at(ilon).routesSegments.find((*iruta).first);

							if (iCelRuta == fdd_->grid.cells.at(ilat).at(ilon).routesSegments.end())
							{
								///ingreso nodo actual
								map<int, int> tmp;
								tmp[(*inodo).first] = (*inodo).first;

								fdd_->grid.cells.at(ilat).at(ilon).routesSegments[(*iruta).first] = tmp;

							}
							else
							{
								///ingreso nodo actual
								(*iCelRuta).second[(*inodo).first] = (*inodo).first;
							}
						}

					}
				}

			}
		}




	}

	cout << Cronometro::GetMilliSpan(nTimeStart) / 60000.0 << "(min)" << endl;
}

// void GridProcess::IngresaPuntosBipsAGrilla()
// {
// 	int nTimeStart = Cronometro::GetMilliCount();
// 	cout << "Ingresando puntos de carga a grilla regular....";
// 
// 	for (map< int, PuntoBip >::iterator it = fdd_->puntosDeCargaBip.begin(); it != fdd_->puntosDeCargaBip.end(); it++)
// 	{
// 		///Calculo la celda
// 		int iLat = ((*it).second.lat - fdd_->grid.minLat) / fdd_->grid.ddLat;
// 		int iLon = ((*it).second.lon - fdd_->grid.minLon) / fdd_->grid.ddLon;
// 
// 		//cout << iLat << "|" << (*it).second.lat << "|" << (*it).second.lat - fdd_->grid.minLat << endl;
// 		//cout << iLon << "|" << (*it).second.lon << "|" << (*it).second.lon - fdd_->grid.minLon << endl;
// 
// 		if(iLat >= 0 && iLat < fdd_->grid.nLat && iLon >= 0 && iLon < fdd_->grid.nLon)
// 			fdd_->grid.cells.at(iLat).at(iLon).puntos.push_back((*it).second.id);
// 	}
// 	cout << Cronometro::GetMilliSpan(nTimeStart) / 60000.0 << "(min)" << endl;
// }

void GridProcess::ConstruyeGrillaFija()
{
	int nTimeStart = Cronometro::GetMilliCount();
	cout << "Construyendo grilla regular....";

	///Deteccion de minimos y maximos que define dimensiones de la grilla
	fdd_->grid.minLat = -33.589809;
	fdd_->grid.maxLat = -33.326579;
	fdd_->grid.minLon = -70.916865;
	fdd_->grid.maxLon = -70.371407;

	//for (map< string, Paradero >::iterator it = fdd_->redParaderos.red.begin(); it != fdd_->redParaderos.red.end(); it++)
	//{
	//	fdd_->grid.minLat = std::fmin(fdd_->grid.minLat, (*it).second.lat);
	//	fdd_->grid.maxLat = std::fmax(fdd_->grid.maxLat, (*it).second.lat);
	//	fdd_->grid.minLon = std::fmin(fdd_->grid.minLon, (*it).second.lon);
	//	fdd_->grid.maxLon = std::fmax(fdd_->grid.maxLon, (*it).second.lon);
	//}

	fdd_->grid.nLat = 500;
	fdd_->grid.nLon = 500;

	fdd_->grid.delta_Latitud = fdd_->grid.maxLat - fdd_->grid.minLat;
	fdd_->grid.delta_Longitud = fdd_->grid.maxLon - fdd_->grid.minLon;

	fdd_->grid.ddLat = fdd_->grid.delta_Latitud / fdd_->grid.nLat;
	fdd_->grid.ddLon = fdd_->grid.delta_Longitud / fdd_->grid.nLon;

	cout << "Min : " << fdd_->grid.minLat << " | " << fdd_->grid.minLon << endl;
	cout << "Max : " << fdd_->grid.maxLat << " | " << fdd_->grid.maxLon << endl;
	cout << "ddLat : " << fdd_->grid.ddLat << endl;
	cout << "ddLon : " << fdd_->grid.ddLon << endl;

	///Llenado de estructura
	cout.precision(10);
	for (double dLat = 0; dLat <= fdd_->grid.delta_Latitud; dLat += fdd_->grid.ddLat)
	{
		//cout << "FLAG 0"  << endl;
		vector< Cell > row;
		for (double dLon = 0; dLon <= fdd_->grid.delta_Longitud; dLon += fdd_->grid.ddLon)
		{
			cout << "-,-," << fdd_->grid.minLat + dLat << "," << fdd_->grid.minLon + dLon<< endl;
			Cell cell;
			row.push_back(cell);
		}
		fdd_->grid.cells.push_back(row);
	}

	cout << fdd_->grid.cells.size() << endl;

	cout << Cronometro::GetMilliSpan(nTimeStart) / 60000.0 << "(min)" << endl;
}
