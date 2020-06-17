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
#include "FuenteDatos.h"

FuenteDatos::FuenteDatos(void)
{
	///Creacion de objeto para manejo de tiempo
	tsh = TimeStampHandler();
}

FuenteDatos::~FuenteDatos(void)
{
}

FuenteDatos::FuenteDatos(const char *nombreArchivoParametros)
{
	///Creacion de objeto para manejo de tiempo
	tsh = TimeStampHandler();

	///Lectura de Parametros
	this->parametros = new Parametros(nombreArchivoParametros);

	///
	outParameters.open(string(parametros->carpetaOutput + "/" + parametros->version + "/" + "PhoneParameters.csv").c_str());
	//outParameters.open("parameters" + parametros->version + ".csv");
	outParameters.precision(10);
	outParameters << "key;value" << endl;

	///Inicializacion de reporte
	this->reporte = new ReporteFuenteDatos(parametros->carpetaReportes);

	///Leer trips
	readTrips();

	///Lectura de diccionario de codigos servicio-sentido
	leeDiccionarioServicios();

	///Lectura de las rutas
	leeRutas();

	///Lectura de la red de paraderos
	leeRedDeParadas();
	//readScheduleMetro();

	///Instancia de lectura de secuencia de paraderos
	leeSecuenciaDeParadas();

	///Lectura de horarios de servicios
	//leeHorarios();
	
	///Lectura bustops, para obenter todas las secuencias posibles por servicio
	readStopTimes();

	///
	//leeSecuenciaDeParadasDTPM();

	///Lectura de puntos de carga
    //if(parametros->withBip)
    //    leePuntosDeCargaBip();

	for (map<string, int>::iterator it = UTMZonesShapes.begin(); it != UTMZonesShapes.end(); it++)
		cout << "UTM Zone Shapes: " << (*it).first << endl;

	for (map<string, int>::iterator it = UTMZonesStops.begin(); it != UTMZonesStops.end(); it++)
		cout << "UTM Zone Stops: " << (*it).first << endl;

	if (UTMZonesShapes.size() > 1 || UTMZonesStops.size() > 1)
	{
		cout << "ERROR : Mas de una zona UTM ( # Zonas Shapes : " << int(UTMZonesShapes.size()) << ")" << endl;
		cout << "ERROR : Mas de una zona UTM ( # Zonas Stops  : " << int(UTMZonesStops.size()) << ")" << endl;
		exit(1);
	}

	///Creacion de directorio para almacenar los kmls	
	system(string("mkdir " + parametros->carpetaKmls).c_str());
	system(string("mkdir " + parametros->carpetaReportes).c_str());
	system(string("mkdir " + parametros->carpetaOutput + "/" + parametros->version).c_str());
}

void FuenteDatos::readTrips()
{
	int nTimeStart = Cronometro::GetMilliCount();

	///Archivo de entrada Principal
	ifstream archivoTrips;
	archivoTrips.open(parametros->nombreCarpetaGTFS + parametros->slash + "trips.txt");

	///Chequeo de archivo 
	if (!archivoTrips.good())
		cout << "Error : No se encuentra el archivo " << parametros->nombreCarpetaGTFS + parametros->slash + "trips.txt" << "!" << endl;
	else
		cout << "Cargando datos de trips (" << parametros->nombreCarpetaGTFS + parametros->slash + "trips.txt" << ")... " << endl;

	vector<string> cur0;
	cur0 = StringFunctions::ExplodeF(',', &archivoTrips);

	///Lectura archivo primario
	while (archivoTrips.good())
	{
		cur0 = StringFunctions::ExplodeF(',', &archivoTrips);
		if (cur0.size() == 0 || cur0[0].compare("") == 0)
			continue;

		Trip trip;
		trip.route_id = cur0[0];
		trip.service_id = cur0[1];
		trip.trip_id = cur0[2];
		trip.trip_headsign = cur0[3];
		trip.direction_id = cur0[5];
		trip.shape_id = cur0[6];

		trips[cur0[2]] = trip;
	}

	///Variables para reporte
	cout << Cronometro::GetMilliSpan(nTimeStart) / 60000.0 << "(min)" << endl;
}

void FuenteDatos::leeDiccionarioServicios()
{
	int nTimeStart = Cronometro::GetMilliCount();

	///Archivo de entrada Principal
	ifstream archivoDiccionario;
	archivoDiccionario.open(parametros->nombreCarpetaGTFS + parametros->slash + "routes.txt");

	///Chequeo de archivo 
	if (!archivoDiccionario.good())
		cout << "Error : No se encuentra el archivo " << parametros->nombreCarpetaGTFS + parametros->slash + "routes.txt" << "!" << endl;
	else
		cout << "Cargando datos de Diccionario (" << parametros->nombreCarpetaGTFS + parametros->slash + "routes.txt" << ")... ";

	///Vector contenedor de la linea actual del archivo
    vector<string> header;
	vector<string> cur;

	int nlineas = 0;

	///Lectura del header
	header = StringFunctions::ExplodeF(',', &archivoDiccionario);

    int iLongName=3;
    int iRouteColor=7;
    int iRouteId = 0;
    header.at(0).erase(0,3);
    
	///Lectura archivo primario
	while (archivoDiccionario.good())
	{
		nlineas++;

		///Lectura de linea del archivo
		cur = StringFunctions::ExplodeF(',', &archivoDiccionario);
		
		///Condicion de salida, a veces no es suficiente solo la condicion del ciclo
		if (cur.size() == 0 || cur[0].compare("") == 0)
			continue;

		///Informacion para el reporte
		reporte->nRegistrosDiccionario++;
		
		///nombre
		std::locale loc;
		std::string str = cur[iLongName];
		for (std::string::size_type i = 0; i < cur[iLongName].length(); ++i)
			str[i] = std::toupper(cur[iLongName][i], loc);
		
		///color
		string color = string("#"+cur.at(iRouteColor));
		
		Servicio ser = Servicio(cur[iRouteId], "", "", color);;

		vector<string> od = StringFunctions::Explode(str, '-');

		if (od.size() == 2)
			ser = Servicio(cur[iRouteId], od[0].substr(0, od[0].length() - 1), od[1].substr(1, od[1].length()), color);
		else if (od.size() == 1)
			ser = Servicio(cur[iRouteId], od[0].substr(0, od[0].length() - 1), "", color);
			//ser = Servicio(cur[iRouteId], str, "", color);
		else
			cout << "ERROR : Servicio no bien definido en datos de entrada(routes.txt)!" << endl;

		ser.tipo = cur.at(5);

		servicios[ser.nombre] = ser;

		///Insercion en diccionario de servicios-sentidos global
		dicSS.servicios[string(cur[iRouteId] + "_I")] = cur[iRouteId];
		dicSS.servicios[string(cur[iRouteId] + "_R")] = cur[iRouteId];
	}


	///DEBUG
	ofstream fout;
	fout.open("dicSS.txt");
	for (map<string, string>::iterator it = dicSS.servicios.begin(); it != dicSS.servicios.end(); it++)
	{
		fout << (*it).first << ";";
		fout << (*it).second << endl;
	}
	fout.close();

	ofstream fout1;
	fout1.open("servicios.txt");
	fout1 << "codigo;nombre;horarioI;horarioR;color;origen;destino;tipo" << endl;
	for (map<string, Servicio>::iterator it = servicios.begin(); it != servicios.end(); it++)
	{
		fout1 << (*it).first << ";";
		fout1 << (*it).second.nombre << ";";
		fout1 << (*it).second.horarioI << ";";
		fout1 << (*it).second.horarioR << ";";
		fout1 << (*it).second.color << ";";
		fout1 << (*it).second.origen << ";";
		fout1 << (*it).second.destino << ";";
		fout1 << (*it).second.tipo << endl;
	}
	fout1.close();



	cout << Cronometro::GetMilliSpan(nTimeStart) / 60000.0 << "(min)" << endl;

}

void FuenteDatos::leeRutas()
{
	int nTimeStart = Cronometro::GetMilliCount();

	///Archivo de entrada Principal
	ifstream archivoRutas;
	archivoRutas.open(parametros->nombreCarpetaGTFS + parametros->slash + "shapes.txt");

	///Chequeo de archivo 
	if (!archivoRutas.good())
		cout << "Error : No se encuentra el archivo " << parametros->nombreCarpetaGTFS + parametros->slash + "shapes.txt" << "!" << endl;
	else
		cout << "Cargando datos de Rutas (" << parametros->nombreCarpetaGTFS + parametros->slash + "shapes.txt" << ")... ";

	///Vector contenedor de la linea actual del archivo
	vector<string> cur;

	int nlineas = 0;

	///Lectura del header
	cur = StringFunctions::ExplodeF(',', &archivoRutas);

	string antServicio = string("-");
	bool unavez = true;
	map< string, Ruta >::iterator iserv_ant;
	Vector3D nodo_ant;
	int corr_ant;

	double x, y;

	///Lectura archivo primario
	while (archivoRutas.good())
	{
		nlineas++;

		///Lectura de linea del archivo
		cur = StringFunctions::ExplodeF(',', &archivoRutas);

		///Condicion de salida, a veces no es suficiente solo la condicion del ciclo
		if (cur.size() == 0 || cur[0].compare("") == 0)
			continue;

		//cout << cur[0] << " | " << cur[1] << " | " << cur[2] << " | " << cur[3] << endl;
		///Generacion del codigo servicio-sentido concatenando las 3 columnas servicio-sentido-variante
		//vector<string> cod = StringFunctions::Explode(cur[0],'_');

		//if (cod.size() != 2)
		//	continue;

		//string servicio = cod.at(0);
		string servicio = cur[0];
		

		//if(servicio.at(0)=='L' || servicio.at(0) == 'M')
		//	servicio.erase(std::remove(servicio.begin(), servicio.end(), '-'), servicio.end());

		dicSS.servicios_rutas[servicio] = servicio;

		///Transformacion de coordenadas
		ConvertCoordinate::LLtoUTM(23, atof(cur[1].c_str()), atof(cur[2].c_str()), y, x, UTMZone);
		UTMZonesShapes[UTMZone] = 1;
		
		map< string, Ruta >::iterator iserv = rutas.mapeo->find(servicio);

		if (iserv == rutas.mapeo->end())
		{
			Ruta ruta = Ruta(servicio);

			bool ok = ruta.AgregarNodo(Vector3D(x,y,0.0),atoi(cur[3].c_str()));

			rutas.mapeo->insert(pair<string, Ruta >(servicio, ruta));

			///Variable para reporte
			reporte->nRutas++;

			///Chequeo del nodo respecto a una zona cuadrada que encierra la zona metropolitana de santiago
			//if (!estaEnSantiago(x, y) )
			//	reporte->InsertaRutaError(servicio, 0, 0, 1);
		}
		else
		{
			bool ok = (*iserv).second.AgregarNodo(Vector3D(x,y,0.0),atoi(cur[3].c_str()));

			///Chequeo del nodo respecto a una zona cuadrada que encierra la zona metropolitana de santiago
			//if (!estaEnSantiago(x, y))
			//	reporte->InsertaRutaError(servicio, 0, 0, 1);

			///Chequeo nodos repetidos
			if (!ok)
				reporte->InsertaRutaError(servicio, 1, 0, 0);
		}

		///Parche para usar el ultimo nodo (Nodo Fin)
		if (unavez)
		{
			antServicio = servicio;
			iserv_ant = rutas.mapeo->find(servicio);
			unavez = false;
		}

		if (antServicio.compare(servicio) != 0)
		{
			antServicio = servicio;
			bool ok = (*iserv_ant).second.AgregarNodo(nodo_ant, corr_ant + 1);
			iserv_ant = rutas.mapeo->find(servicio);

			///Chequeo del nodo respecto a una zona cuadrada que encierra la zona metropolitana de santiago
			//if (!estaEnSantiago(int(nodo_ant.x), int(nodo_ant.y)))
			//	reporte->InsertaRutaError(servicio, 0, 0, 1);

			///Chequeo nodos repetidos
			if (!ok)
				reporte->InsertaRutaError(servicio, 1, 0, 0);
		}

		nodo_ant = Vector3D(x, y, 0.0);
		corr_ant = atoi(cur[3].c_str());

	}


	///Nodos Fuera de ruta
	//map< string, int> check = output->ChequeoGeometrico();
	//for (map<string, int>::iterator it = check.begin(); it != check.end(); it++)
	//{
	//	reporte->InsertaRutaError((*it).first, 0, (*it).second, 0);
	//}

	//output->SimplificaRutas();
	rutas.CalculaLargosRutas();
	rutas.CalculaTramificado(parametros->distanciaTramado);
	rutas.SimplificaRutas();

	///Variables para reporte
	//reporte->rutaMasLarga = output->RutaMasLarga();
	//reporte->rutaMasCorta = output->RutaMasCorta();

	ofstream fout1;
	fout1.open("rutas.txt");
	fout1 << "servicio;orden;x;y;z" << endl;
	fout1.precision(10);
	for (map<string, Ruta>::iterator it = rutas.mapeo->begin(); it != rutas.mapeo->end(); it++)
	{
		for(map<int, Vector3D>::iterator inodo = (*it).second.nodos->begin(); inodo != (*it).second.nodos->end();inodo++)
		{
			fout1 << (*it).second.nombre << ";";
			fout1 << (*inodo).first << ";";
			fout1 << (*inodo).second.x << ";";
			fout1 << (*inodo).second.y << ";";
			fout1 << (*inodo).second.z << endl;
		}
	}
	fout1.close();

	cout << Cronometro::GetMilliSpan(nTimeStart) / 60000.0 << "(min)" << endl;

}

void FuenteDatos::leeRedDeParadas()
{
	int nTimeStart = Cronometro::GetMilliCount();

	///Archivo de entrada Principal
	ifstream archivoParaderos;
	archivoParaderos.open(parametros->nombreCarpetaGTFS + parametros->slash + "stops.txt");

	///Chequeo de archivo 
	if (!archivoParaderos.good())
		cout << "Error : No se encuentra el archivo " << parametros->nombreCarpetaGTFS + parametros->slash + "stops.txt" << "!" << endl;
	else
		cout << " Cargando red de Paraderos... " << endl;

	///Vector contenedor de la linea actual del archivo
	vector<string> cur;

	///Iterador para buscar los paraderos
	map< string, Paradero >::iterator itPar;

	int nlineas = 0;

	///Lectura del header
	cur = StringFunctions::ExplodeF(',', &archivoParaderos);

	double x, y;

	///Lectura archivo primario
	while (archivoParaderos.good())
	{
		nlineas++;

		///Lectura de linea del archivo
		cur = StringFunctions::ExplodeF(',', &archivoParaderos);

		///Condicion de salida, a veces no es suficiente solo la condicion del ciclo
		if (cur.size() == 0 || cur[0].compare("") == 0)
		{
			//mde_->Advertencia("Advertencia : se ha encontrado linea vacia en " + parametros->nombreArchivoRedParadas + "!\n");
			continue;
		}

		reporte->nRedParadas++;

		ConvertCoordinate::LLtoUTM(23, atof(cur[3].c_str()), atof(cur[4].c_str()), y, x, UTMZone);
		UTMZonesStops[UTMZone] = 1; 

		//cout << atof(cur[3].c_str()) << "|" << atof(cur[4].c_str()) << "|" << x << "|" << y << endl;

		string nombre = cur[2];
		string mode = string("-");


		Paradero par = Paradero(atof(cur[3].c_str()), atof(cur[4].c_str()),(int)x, (int)y, cur[0], nombre);
		par.mode = mode;

		redParaderos.red[par.codigo] = par;
	}

	CorrigeParadasMismaPosicion();

	///DEBUG
	ofstream fout;
	fout.open("redParadas.txt");
	for(map< string , Paradero >::iterator it=redParaderos.red.begin();it!=redParaderos.red.end();it++)
	{
		fout << (*it).second.codigo << "|";
		fout << (*it).second.nombre << "|";
		fout << (*it).second.x << "|";
		fout << (*it).second.y << "|";
		fout << (*it).second.lat << "|";
		fout << (*it).second.lon << "|";
		fout << endl;
	}
	fout.close();

	cout << Cronometro::GetMilliSpan(nTimeStart) / 60000.0 << "(min)" << endl;
}

void FuenteDatos::CorrigeParadasMismaPosicion()
{
	int nTimeStart = Cronometro::GetMilliCount();

	cout << "Proceso X : Correccion de paradas misma posicion... ";

	char *UTMZone = "19H";
	///Ciclo sobre secuencia de paradas
	for (map<string, Paradero>::iterator ipar1 = redParaderos.red.begin(); ipar1 != redParaderos.red.end(); ipar1++)
	{
		for (map<string, Paradero>::iterator ipar2 = redParaderos.red.begin(); ipar2 != redParaderos.red.end(); ipar2++)
		{
			if ((*ipar1).first.compare((*ipar2).first) != 0)
			{
				float dx = (*ipar1).second.x - (*ipar2).second.x;
				float dy = (*ipar1).second.y - (*ipar2).second.y;
				float dist = sqrt(dx*dx + dy*dy);

				if (dist < 5)
				{
					if(abs(dx) <= abs(dy))
					{
						if ((*ipar1).second.x >= (*ipar2).second.x)
							(*ipar1).second.x += 5;
						else
							(*ipar1).second.x -= 5;

						double lat, lon;
						ConvertCoordinate::UTMtoLL(23, (*ipar1).second.y, (*ipar1).second.x, UTMZone, lat, lon);

						(*ipar1).second.lat = lat;
						(*ipar1).second.lon = lon;
					}
					else
					{
						if ((*ipar1).second.y >= (*ipar2).second.y)
							(*ipar1).second.y += 5;
						else
							(*ipar1).second.y -= 5;

						double lat, lon;
						ConvertCoordinate::UTMtoLL(23, (*ipar1).second.y, (*ipar1).second.x, UTMZone, lat, lon);

						(*ipar1).second.lat = lat;
						(*ipar1).second.lon = lon;
					}
					//cout << (*ipar1).first << "|" << (*ipar2).first << "|" << dist << endl;
				}
			}
		}
	}

	cout << Cronometro::GetMilliSpan(nTimeStart) / 60000.0 << "(min)" << endl;
}

void FuenteDatos::leeSecuenciaDeParadas()
{
	int nTimeStart = Cronometro::GetMilliCount();

	///Archivo de entrada Principal
	ifstream archivoParaderos;
	archivoParaderos.open(parametros->nombreCarpetaGTFS + parametros->slash + "stop_times.txt");

	///Chequeo de archivo 
	if (!archivoParaderos.good())
		cout << "Error : No se encuentra el archivo " << parametros->nombreCarpetaGTFS + parametros->slash + "stop_times.txt" << "!" << endl;
	else
		cout << "Cargando datos de Secuencia de Paraderos (" << parametros->nombreCarpetaGTFS + parametros->slash + "stop_times.txt" << ")... ";

	///Vector contenedor de la linea actual del archivo
	vector<string> cur;

	int nlineas = 0;

	///Lectura del header
	cur = StringFunctions::ExplodeF(',', &archivoParaderos);

	map< string, map<int,Paradero> >::iterator iserv;
	map < string, Paradero >::iterator ired;

	bool activo = false;
	string servicio_ant = string("-");

	map<string, Ruta>::iterator iRuta;

	///Lectura archivo primario
	while (archivoParaderos.good())
	{
		nlineas++;

		///Lectura de linea del archivo
		cur = StringFunctions::ExplodeF(',', &archivoParaderos);

		///Condicion de salida, a veces no es suficiente solo la condicion del ciclo
		if (cur.size() == 0 || cur[0].compare("") == 0)
			continue;

		///Generacion del codigo servicio-sentido concatenando las 3 columnas servicio-sentido-variante

		map< string, Trip >::iterator itrip = trips.find(cur.at(0));

		string servicio;
		if (itrip != trips.end())
		{
			servicio = (*itrip).second.shape_id;
		}
		else
		{
			cout << "ERROR : no se encontro el trip de id : " << cur.at(0) << endl;
			continue;
		}

		if (servicio.compare(servicio_ant) != 0)
			activo = true;

		if (servicio.compare(servicio_ant) == 0 && cur[4].compare("1") == 0)
			activo = false;

		///Busco paradero en red de paradas
		Paradero par;
		ired = redParaderos.red.find(cur[3]);
		if (ired != redParaderos.red.end())
		{
			par = (*ired).second;
		}
		else
		{
			cout << "ERROR : paradero " << cur[3] << " de stops_times.txt no se encuentra en stops.txt!" << endl;
			continue;
		}

		iRuta = rutas.mapeo->find(servicio);
		Vector3D p = Vector3D((*ired).second.x, (*ired).second.y, 0.0);
		float distance = -1;
		if (iRuta != rutas.mapeo->end())
		{
			distance = (*iRuta).second.GetDistanceOnRoute(&p);
		}
		else
		{
			cout << "ERROR no se encuentra la ruta : " << servicio << endl;
		}

		if (activo)
		{
			///Construccion de secuencia
			iserv = secParaderos.secuencias.find(servicio);
			if (iserv == secParaderos.secuencias.end())
			{
				map<int, Paradero> tmp;
				tmp[int(distance)] = par;
				secParaderos.secuencias[servicio] = tmp;
			}
			else
			{
				(*iserv).second[int(distance)] = par;
			}
		}

		///Construccion de secuencia
		iserv = secParaderosTODOS.secuencias.find(servicio);
		//cout << servicio << endl;
		if (iserv == secParaderosTODOS.secuencias.end())
		{
			map<int, Paradero> tmp;
			tmp[int(distance)] = par;
			secParaderosTODOS.secuencias[servicio] = tmp;
		}
		else
		{
			(*iserv).second[int(distance)] = par;
		}
			
		servicio_ant = servicio;
	}

	///DEBUG
	ofstream fout;
	fout.open("secParadas.txt");
	for (iserv = secParaderos.secuencias.begin(); iserv != secParaderos.secuencias.end(); iserv++)
	{
		for (map<int, Paradero>::iterator ipar = (*iserv).second.begin(); ipar != (*iserv).second.end(); ipar++)
		{
			fout << (*iserv).first << ";";
			fout << (*ipar).first << ";";
			fout << (*ipar).second.codigo << ";";
			fout << (*ipar).second.nombre << endl;
		}
	}
	fout.close();

	///Variables para reporte
	cout << Cronometro::GetMilliSpan(nTimeStart) / 60000.0 << "(min)" << endl;
}

void FuenteDatos::readStopTimes()
{
	int nTimeStart = Cronometro::GetMilliCount();

	map< string, Trip >::iterator itrip;

	///Archivo de entrada Principal
	ifstream archivoParaderos;
	archivoParaderos.open(parametros->nombreCarpetaGTFS + parametros->slash + "stop_times.txt");

	///Chequeo de archivo 
	if (!archivoParaderos.good())
		cout << "Error : No se encuentra el archivo " << parametros->nombreCarpetaGTFS + parametros->slash + "stop_times.txt" << "!" << endl;
	else
		cout << "Cargando datos de Secuencia de Paraderos (" << parametros->nombreCarpetaGTFS + parametros->slash + "stop_times.txt" << ")... ";

	///Vector contenedor de la linea actual del archivo
	vector<string> cur;

	///Lectura del header
	cur = StringFunctions::ExplodeF(',', &archivoParaderos);

	///Lectura archivo primario
	int nlineas = 0;
	while (archivoParaderos.good())
	{
		nlineas++;

		///Lectura de linea del archivo
		cur = StringFunctions::ExplodeF(',', &archivoParaderos);

		///Condicion de salida, a veces no es suficiente solo la condicion del ciclo
		if (cur.size() == 0 || cur[0].compare("") == 0)
			continue;

		///Generacion del codigo servicio-sentido concatenando las 3 columnas servicio-sentido-variante

		map< string, Trip >::iterator itrip = trips.find(cur.at(0));
		string servicio;
		if (itrip != trips.end())
		{
			servicio = (*itrip).second.shape_id;
		}
		else
		{
			cout << "ERROR : no se encontro el trip de id : " << cur.at(0) << endl;
			continue;
		}

		string trip_id = cur[0];

		isec = secuencias.find(trip_id);
		if (isec == secuencias.end())
		{
			map<int, string> tmp;
			tmp[atoi(cur[4].c_str())] = cur[3];

			Secuencia secu;
			secu.codigo = servicio;
			secu.version = "-";
			secu.paradas = tmp;
			secu.destino = (*itrip).second.trip_headsign;

			
//			secu.hora_ini = cur[1];
//			secu.hora_fin = cur[2];
			
			secu.hora_ini = tsh.RedondeaMediaHora(cur[1]);
			secu.hora_fin = tsh.RedondeaMediaHora(cur[2]);


			/*
			ifrec = frecuencias.find(trip_id);
			if (ifrec != frecuencias.end())
			{
				secu.hora_ini = (*ifrec).second.hora_ini;
				secu.hora_fin = (*ifrec).second.hora_fin;
			}
			else
			{
				cout << "no encontre en frequencies : " << trip_id << endl;
			}
			*/

			//itrip = trips.find(trip_id);
			if (itrip != trips.end())
			{
				secu.shape_id = (*itrip).second.shape_id;
				secu.tipodia = (*itrip).second.service_id;
			//	secu.headway = (*ifrec).second.headways;
			}
			else
			{
				cout << "no encontre en trips : " << trip_id << endl;
			}
			secuencias[trip_id] = secu;
		}
		else
		{
			(*isec).second.paradas[atoi(cur[4].c_str())] = cur[3];
			(*isec).second.hora_fin = tsh.RedondeaMediaHora(cur[2]);
		}
	}

	ofstream dbg;
	dbg.open("secuencias.txt");
	dbg << "key|codigo|shape_id|tipodia|hora_ini|hora_fin" << endl;
	for (map<string, Secuencia >::iterator it = secuencias.begin(); it != secuencias.end(); it++)
	{
		dbg << (*it).first << "|";
		dbg << (*it).second.codigo << "|";
		dbg << (*it).second.shape_id << "|";
		dbg << (*it).second.tipodia << "|";
		dbg << (*it).second.hora_ini << "|";
		dbg << (*it).second.hora_fin << endl;
	}
	dbg.close();

	///Variables para reporte
	cout << Cronometro::GetMilliSpan(nTimeStart) / 60000.0 << "(min)" << endl;
}
/*
bool FuenteDatos::estaEnSantiago(COORD x_, COORD y_)
{
	///Chequeo del nodo respecto a una zona cuadrada que encierra la zona metropolitana de santiago
	if(x_ < 320000 
	|| y_ > 6319991
	|| x_ > 365208 
	|| y_ < 6265683)
	{
		return false;
	}

	return true;
}




void FuenteDatos::leeHorarios()
{
	int nTimeStart = Cronometro::GetMilliCount();

	///Archivo de entrada Principal
	ifstream archivoHorarios;
	archivoHorarios.open(parametros->nombreArchivoHorarios.c_str());

	///Chequeo de archivo 
	if (!archivoHorarios.good())
	{
		cout << "No se ha encontrado archivo de horarios " << parametros->nombreArchivoHorarios.c_str() << "!" << endl;
		cout << "Los servicios quedaran con un horario nulo. " << endl;

		for (map<string, Servicio>::iterator iser = servicios.begin(); iser != servicios.end(); iser++)
		{
			(*iser).second.horarioI = string("-");
			(*iser).second.horarioR = string("-");
		}

		return;
	}
	else
		cout << "Cargando datos de Diccionario (" << parametros->nombreCarpetaGTFS + parametros->slash + "routes.txt" << ")... ";

	///Vector contenedor de la linea actual del archivo
	vector<string> cur;

	int nlineas = 0;

	///Lectura del header
	cur = StringFunctions::ExplodeF(';', &archivoHorarios);

	map<string, Servicio>::iterator iser;
	///Lectura archivo primario
	while (archivoHorarios.good())
	{
		nlineas++;

		///Lectura de linea del archivo
		cur = StringFunctions::ExplodeF(';', &archivoHorarios);

		///Condicion de salida, a veces no es suficiente solo la condicion del ciclo
		if (cur.size() == 0 || cur[0].compare("") == 0)
			continue;

		iser = servicios.find(cur.at(0));

		if (iser != servicios.end())
		{
			std::locale loc;
			std::string str = cur[1];
			for (std::string::size_type i = 0; i < cur[1].length(); ++i)
				str[i] = std::toupper(cur[1][i], loc);

			//cout << cur.at(0) << "|" << (*iser).second.destino << "|" << str << endl;
			if ((*iser).second.destino.compare(str) == 0)
				(*iser).second.horarioI.append(cur.at(2) + "-" + cur.at(3) + "-" + cur.at(4) + "/");
			else
				(*iser).second.horarioR.append(cur.at(2) + "-" + cur.at(3) + "-" + cur.at(4) + "/");
		}
	}


	
	///DEBUG
	ofstream fout;
	fout.open("servicios_horario.txt");
	for (map<string, Servicio>::iterator iser = servicios.begin(); iser != servicios.end(); iser++)
	{
		fout << (*iser).first << ";";

		vector<string> horariosI = StringFunctions::Explode((*iser).second.horarioI, '-');
		for (vector<string>::iterator ihor = horariosI.begin(); ihor != horariosI.end(); ihor++)
		{
			if ((*ihor).compare("") == 0)
				continue;

			if (ihor == horariosI.begin())
				fout << (*ihor);
			else
				fout << "-" << (*ihor);
		}
		fout << ";";

		vector<string> horariosR = StringFunctions::Explode((*iser).second.horarioR, '-');
		for (vector<string>::iterator ihor = horariosR.begin(); ihor != horariosR.end(); ihor++)
		{
			if ((*ihor).compare("") == 0)
				continue;

			if (ihor == horariosR.begin())
				fout << (*ihor);
			else
				fout << "-" << (*ihor);
		}

		fout << endl;
	}
	fout.close();
	

	reporte->tDiccionario = Cronometro::GetMilliSpan(nTimeStart) / 60000.0;

	cout << reporte->tDiccionario << "(min)" << endl;

}
*/

