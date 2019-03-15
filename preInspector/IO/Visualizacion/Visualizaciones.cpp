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
#include "Visualizaciones.h"

Visualizaciones::Visualizaciones()
{
}

Visualizaciones::Visualizaciones(FuenteDatos *fdd)
{
	this->fdd_ = fdd;
}

Visualizaciones::~Visualizaciones()
{
}

void Visualizaciones::DibujaGoogleEarth()
{
	///Interfaz GOOGLE-EARTH
	cout << "Proceso  : Visualizaciones en Google-Earth... ";
	int nTimeStart1 = Cronometro::GetMilliCount();

	///Creacion de directorio para almacenar los kmls
	string directorio(fdd_->parametros->carpetaKmls);
	string creaDirectorio("mkdir ");
	creaDirectorio.append(directorio);
	system(creaDirectorio.c_str());

	string pathkml = string(string(fdd_->parametros->carpetaKmls + fdd_->parametros->slash  + "chequeo.kml").c_str());
	CreaKML *kml = new CreaKML(pathkml.c_str(), pathkml.c_str());

	DibujaRedParadas(kml);
	DibujaRedPuntosBips(kml);
	DibujaGrillaParadas(kml);

	///Creacion del archivo .kml
	kml->AbreFolder("Datos Por servicio ");
	map<string, map<int, Paradero> >::iterator isec;
	for (map<string, Ruta>::iterator iserv = fdd_->rutas.mapeo->begin(); iserv != fdd_->rutas.mapeo->end(); iserv++)
	{
		isec = fdd_->secParaderos.secuencias.find((*iserv).first);

		if (isec != fdd_->secParaderos.secuencias.end())
			DibujaServicio((*iserv).second, (*isec).second,(*iserv).first, kml);
	}
	kml->CierraFolder();

	kml->CierraArchivo();

	delete(kml);
	cout << Cronometro::GetMilliSpan(nTimeStart1) / 60000.0 << "(min)" << endl;
}

void Visualizaciones::DibujaServicio(Ruta& ruta, map<int, Paradero>& secuencia, string serviciosentido, CreaKML *kml)
{
	kml->AbreFolder(serviciosentido.c_str());
	//Dibujo ruta
	//kml->AbreFolder("ruta");

	map<int, Vector3D>::iterator fin = ruta.nodos->end();
	map<int, Vector3D>::iterator fin2 = ruta.nodos->end();
	//fin--;
	//fin2 = fin;
	//fin2--;
	//for (map<int, Vector3D>::iterator inodo = ruta.nodos->begin(); inodo != fin; inodo++)
	//{

	//	double lat2, lon2;
	//	double lat3, lon3;

	//	map<int, Vector3D>::iterator inodo2 = inodo;
	//	inodo2++;

	//	ConvertCoordinate::UTMtoLL(23, (*inodo).second.y, (*inodo).second.x, UTMZONE, lat2, lon2);
	//	ConvertCoordinate::UTMtoLL(23, (*inodo2).second.y, (*inodo2).second.x, UTMZONE, lat3, lon3);

	//	string strlat2 = StringFunctions::Double2String(lat2, 20);
	//	string strlon2 = StringFunctions::Double2String(lon2, 20);

	//	string strlat3 = StringFunctions::Double2String(lat3, 20);
	//	string strlon3 = StringFunctions::Double2String(lon3, 20);


	//	string style("SheetColorAzul");
	//	string style2("SheetColorNaranja");

	//	if (inodo == ruta.nodos->begin() || inodo == fin2)
	//	{
	//		kml->InsertaSegmento("",
	//			strlat2.c_str(),
	//			strlon2.c_str(),
	//			strlat3.c_str(),
	//			strlon3.c_str(),
	//			style2.c_str(),
	//			0);
	//	}
	//	else
	//	{
	//		kml->InsertaSegmento("",
	//			strlat2.c_str(),
	//			strlon2.c_str(),
	//			strlat3.c_str(),
	//			strlon3.c_str(),
	//			style.c_str(),
	//			0);
	//	}
	//}
	//kml->CierraFolder();

	//Dibujo ruta simplificada
	kml->AbreFolder("ruta_simplificada");

	fin = ruta.nodosSimplificados->end();
	fin2 = ruta.nodosSimplificados->end();
	fin--;
	fin2 = fin;
	fin2--;
	for (map<int, Vector3D>::iterator inodo = ruta.nodosSimplificados->begin(); inodo != fin; inodo++)
	{

		double lat2, lon2;
		double lat3, lon3;

		map<int, Vector3D>::iterator inodo2 = inodo;
		inodo2++;

		ConvertCoordinate::UTMtoLL(23, (*inodo).second.y, (*inodo).second.x, UTMZONE, lat2, lon2);
		ConvertCoordinate::UTMtoLL(23, (*inodo2).second.y, (*inodo2).second.x, UTMZONE, lat3, lon3);

		string strlat2 = StringFunctions::Double2String(lat2, 20);
		string strlon2 = StringFunctions::Double2String(lon2, 20);

		string strlat3 = StringFunctions::Double2String(lat3, 20);
		string strlon3 = StringFunctions::Double2String(lon3, 20);


		string style("SheetColorAzul");
		string style2("SheetColorNaranja");

		kml->InsertaPunto(StringFunctions::Int2String((*inodo).first).c_str(),
			"",
			strlat2.c_str(),
			strlon2.c_str(),
			style.c_str(),
			0);

		if (inodo == ruta.nodosSimplificados->begin() || inodo == fin2)
		{
			kml->InsertaSegmento("",
				strlat2.c_str(),
				strlon2.c_str(),
				strlat3.c_str(),
				strlon3.c_str(),
				style2.c_str(),
				0);
		}
		else
		{
			kml->InsertaSegmento("",
				strlat2.c_str(),
				strlon2.c_str(),
				strlat3.c_str(),
				strlon3.c_str(),
				style.c_str(),
				0);
		}
	}
	kml->CierraFolder();

	//Dibujo secuencia
	kml->AbreFolder("paradas");
	for (map<int, Paradero>::iterator inodo = secuencia.begin(); inodo != secuencia.end(); inodo++)
	{
		double lat, lon;

		ConvertCoordinate::UTMtoLL(23, (*inodo).second.y, (*inodo).second.x, UTMZONE, lat, lon);

		string strlat = StringFunctions::Double2String(lat, 20);
		string strlon = StringFunctions::Double2String(lon, 20);

		string style("SheetColorAmarillo");

		kml->InsertaPunto((*inodo).second.codigo.c_str(),
			"",
			strlat.c_str(),
			strlon.c_str(),
			style.c_str(),
			0);

	}
	kml->CierraFolder();

	//Dibujo ruta
	//kml->AbreFolder("Tramado");
	//for (map<float, Vector3D>::iterator inodo = ruta.tramado.begin(); inodo != ruta.tramado.end(); inodo++)
	//{
	//	double lat, lon;

	//	ConvertCoordinate::UTMtoLL(23, (*inodo).second.y, (*inodo).second.x, UTMZONE, lat, lon);

	//	string strlat = StringFunctions::Double2String(lat, 20);
	//	string strlon = StringFunctions::Double2String(lon, 20);

	//	string style("SheetColorNaranja");

	//	kml->InsertaPunto("",
	//		"",
	//		strlat.c_str(),
	//		strlon.c_str(),
	//		style.c_str(),
	//		0);

	//}
	//kml->CierraFolder();

	kml->CierraFolder();
}

void Visualizaciones::DibujaRedParadas(CreaKML *kml)
{
	//Dibujo trayectoria bus
	kml->AbreFolder("Red de Paradas ");

	for (map<string, Paradero>::iterator ipar = fdd_->redParaderos.red.begin(); ipar != fdd_->redParaderos.red.end(); ipar++)
	{
		double lat, lon;

		ConvertCoordinate::UTMtoLL(23, (*ipar).second.y, (*ipar).second.x, UTMZONE, lat, lon);

		string strlat = StringFunctions::Double2String(lat, 20);
		string strlon = StringFunctions::Double2String(lon, 20);

		string style("SheetColorAzul");
		string style2("SheetColorNaranja");

		kml->InsertaPunto((*ipar).second.codigo.c_str(),
			"",
			strlat.c_str(),
			strlon.c_str(),
			style.c_str(),
			0);
	}

	kml->CierraFolder();

}

void Visualizaciones::DibujaRedPuntosBips(CreaKML *kml)
{
	//Dibujo trayectoria bus
	kml->AbreFolder("Red de puntos ");

	for (map<int, PuntoBip>::iterator ipar = fdd_->puntosDeCargaBip.begin(); ipar != fdd_->puntosDeCargaBip.end(); ipar++)
	{
		double lat, lon;

		ConvertCoordinate::UTMtoLL(23, (*ipar).second.y, (*ipar).second.x, UTMZONE, lat, lon);

		string strlat = StringFunctions::Double2String(lat, 20);
		string strlon = StringFunctions::Double2String(lon, 20);

		string style("SheetColorRosado");


		kml->InsertaPunto((*ipar).second.nombre.c_str(),
			"",
			strlat.c_str(),
			strlon.c_str(),
			style.c_str(),
			0);
	}

	kml->CierraFolder();

}

void Visualizaciones::DibujaGrillaParadas(CreaKML *kml)
{
	kml->AbreFolder("grilla");

	kml->AbreFolder("grilla");
	double delta_Latitud = fdd_->grid.maxLat - fdd_->grid.minLat;
	double delta_Longitud = fdd_->grid.maxLon - fdd_->grid.minLon;

	double ddLat = delta_Latitud / fdd_->grid.nLat;
	double ddLon = delta_Longitud / fdd_->grid.nLon;

	//Horizontales (Latitud)
	for (double dLat = 0; dLat <= delta_Latitud; dLat += ddLat)
	{

		string strlat2 = StringFunctions::Double2String(fdd_->grid.minLat + dLat, 20);
		string strlon2 = StringFunctions::Double2String(fdd_->grid.minLon, 20);
		string strlat3 = StringFunctions::Double2String(fdd_->grid.minLat + dLat, 20);
		string strlon3 = StringFunctions::Double2String(fdd_->grid.maxLon, 20);

		string style("SheetColorNaranja");

		kml->InsertaSegmento("",
			strlat2.c_str(),
			strlon2.c_str(),
			strlat3.c_str(),
			strlon3.c_str(),
			style.c_str(),
			0);
	}

	//Verticales (Longitud)
	for (double dLon = 0; dLon <= delta_Longitud; dLon += ddLon)
	{
		string strlat2 = StringFunctions::Double2String(fdd_->grid.minLat, 20);
		string strlon2 = StringFunctions::Double2String(fdd_->grid.minLon + dLon, 20);
		string strlat3 = StringFunctions::Double2String(fdd_->grid.maxLat, 20);
		string strlon3 = StringFunctions::Double2String(fdd_->grid.minLon + dLon, 20);

		string style("SheetColorNaranja");

		kml->InsertaSegmento("",
			strlat2.c_str(),
			strlon2.c_str(),
			strlat3.c_str(),
			strlon3.c_str(),
			style.c_str(),
			0);
	}
	kml->CierraFolder();

	kml->AbreFolder("datos por celda");
	for (int iLat = 0; iLat < fdd_->grid.cells.size(); iLat++)
	{
		for (int iLon = 0; iLon < fdd_->grid.cells.at(iLat).size(); iLon++)
		{
			double clat = fdd_->grid.minLat + iLat * fdd_->grid.ddLat + fdd_->grid.ddLat * 0.5;
			double clon = fdd_->grid.minLon + iLon * fdd_->grid.ddLon + fdd_->grid.ddLon * 0.5;

			string description = string("celda : ");
			description.append("(" + StringFunctions::Int2String(iLat) + "," + StringFunctions::Int2String(iLon) + ")\n");
			description.append("paradas : ");
			for (vector<string>::iterator istops = fdd_->grid.cells.at(iLat).at(iLon).stops.begin(); istops != fdd_->grid.cells.at(iLat).at(iLon).stops.end(); istops++)
			{
				if (istops == fdd_->grid.cells.at(iLat).at(iLon).stops.begin())
					description.append((*istops));
				else
					description.append("-" + (*istops));
			}
			description.append("\n");

			description.append("rutas : ");
			for (map< string, map<int, int> >::iterator iruta = fdd_->grid.cells.at(iLat).at(iLon).routesSegments.begin(); iruta != fdd_->grid.cells.at(iLat).at(iLon).routesSegments.end(); iruta++)
			{
				description.append((*iruta).first + " ( ");
				for(map<int, int>::iterator isegment = (*iruta).second.begin(); isegment != (*iruta).second.end();isegment++)
				{
					if (isegment == (*iruta).second.begin())
						description.append(StringFunctions::Int2String((*isegment).first));
					else
						description.append("-" + StringFunctions::Int2String((*isegment).first));
				}
				description.append(" ) \n");
			}


			string strlat = StringFunctions::Double2String(clat, 20);
			string strlon = StringFunctions::Double2String(clon, 20);

			string style("SheetColorAmarillo");

			kml->InsertaPunto("",
				description.c_str(),
				strlat.c_str(),
				strlon.c_str(),
				style.c_str(),
				0);

		}
	}
	kml->CierraFolder();

	kml->CierraFolder();
}
