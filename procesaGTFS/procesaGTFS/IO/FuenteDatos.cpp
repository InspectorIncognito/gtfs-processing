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
* Last modified : Jorge Roa 16-02-2017
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

	///Inicializacion de reporte
	this->reporte = new ReporteFuenteDatos(parametros->carpetaReportes);

	///Prerocesamiento de stops
	PreprocesaGTFS();

	///Lectura de archivo que contiene ida y regreso de los servicios (opcional)
	leeSentidoServicios();

	///Lectura de diccionario de codigos servicio-sentido
	leeDiccionarioServicios();

	///Lectura de trips para insertar en rutas (GTFS Concepcion solamente)
	leeTrips();

	///Lectura de las rutas
	leeRutas();

	///Lectura de la red de paraderos
	leeRedDeParadas();

	///Instancia de lectura de secuencia de paraderos
	leeSecuenciaDeParadas();

	///Lectura de horarios de servicios
	leeHorarios();

	///Lectura de puntos de carga
	leePuntosDeCargaBip();

	///Creacion de directorio para almacenar los kmls	
	system(string("mkdir " + parametros->carpetaKmls).c_str());
	system(string("mkdir " + parametros->carpetaReportes).c_str());
}

void FuenteDatos::leeSentidoServicios() {
	int nTimeStart = Cronometro::GetMilliCount();

	///Verificando si existe el archivo
	if (parametros->nombreArchivoIR != "" && parametros->nombreArchivoIR != "-") {
		///Archivo de entrada Principal
		ifstream archivoSentidoServicios;
		archivoSentidoServicios.open(parametros->nombreCarpetaGTFS + parametros->slash + parametros->nombreArchivoIR);

		///Chequeo de archivo 
		if (!archivoSentidoServicios.good())
			cout << "Error : No se encuentra el archivo " << parametros->nombreCarpetaGTFS + parametros->slash + parametros->nombreArchivoIR << "!" << endl;
		else
			cout << "Cargando datos de serviciosIR (" << parametros->nombreCarpetaGTFS + parametros->slash + parametros->nombreArchivoIR << ")... ";

		///Vector contenedor de la linea actual del archivo
		vector<string> cur;

		///Lectura archivo primario
		while (archivoSentidoServicios.good())
		{
			///Lectura de linea del archivo
			cur = StringFunctions::ExplodeF(',', &archivoSentidoServicios);

			///Condicion de salida, a veces no es suficiente solo la condicion del ciclo
			if (cur.size() == 0 || cur[0].compare("") == 0)
				continue;

			serviciosIR[cur[0]] = cur[1];
			serviciosIR[cur[1]] = cur[0];

		}

		cout << "Finalizado " << Cronometro::GetMilliSpan(nTimeStart) / 60000.0 << "(min)" << endl;
	}
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
	vector<string> cur;

	int nlineas = 0;
	int route_id, route_short_name, route_long_name,route_color;
	///Lectura del header
	cur = StringFunctions::ExplodeF(',', &archivoDiccionario);
	for (int i = 0; i < cur.size(); i++) {
		if (cur[i] == "route_id")
			route_id = i;
		if (cur[i] == "route_short_name")
			route_short_name = i;
		if (cur[i] == "route_long_name")
			route_long_name = i;
		if (cur[i] == "route_color")
			route_color = i;
	}

	///Lectura archivo primario
	while (archivoDiccionario.good())
	{
		nlineas++;

		///Lectura de linea del archivo
		cur = StringFunctions::ExplodeF(',', &archivoDiccionario);
		
		///Condicion de salida, a veces no es suficiente solo la condicion del ciclo
		if (cur.size() == 0 || cur[0].compare("") == 0)
			continue;

		routes[cur[route_id]] = cur[route_long_name];
		map_servicios[cur[route_id]] = cur[route_short_name]; // rout_id is key, short_name value

		///Informacion para el reporte
		reporte->nRegistrosDiccionario++;

		std::locale loc;
		std::string str = cur[route_long_name];
		for (std::string::size_type i = 0; i < cur[route_long_name].length(); ++i)
			str[i] = std::toupper(cur[route_long_name][i], loc);

		vector<string> od = StringFunctions::Explode(str, '-');

		Servicio ser;
		if (od.size() == 2) {
			ser = Servicio(cur[route_short_name], od[0], od[1], cur[route_color]);
		}
		else if (od.size() == 1) {
			ser = Servicio(cur[route_short_name], od[0], "", cur[route_color]);
		}
		else
			cout << "ERROR : Servicio no bien definido en datos de entrada(routes.txt)!" << endl;

		servicios[ser.nombre] = ser;

		///Insercion en diccionario de servicios-sentidos global
		dicSS.servicios[cur[route_short_name]] = cur[route_short_name];

		///identificacion de colores
		map<string, string>::iterator icolor = parametros->mapeoColores.find(cur[route_color]);

		if (icolor != parametros->mapeoColores.end())
		{
			dicSS.colores[cur[route_short_name]] = (*icolor).second;

		}

	}

}


void FuenteDatos::leeTrips()
{
	int nTimeStart = Cronometro::GetMilliCount();

	///Archivo de entrada Principal
	ifstream archivoTrips;
	archivoTrips.open(parametros->nombreCarpetaGTFS + parametros->slash + "trips.txt");

	///Chequeo de archivo 
	if (!archivoTrips.good())
		cout << "Error : No se encuentra el archivo " << parametros->nombreCarpetaGTFS + parametros->slash + "trips.txt" << "!" << endl;
	else
		cout << "Cargando datos de Trips (" << parametros->nombreCarpetaGTFS + parametros->slash + "trips.txt" << ")... ";

	///Vector contenedor de la linea actual del archivo
	vector<string> cur;

	int nlineas = 0;

	///Lectura del header
	cur = StringFunctions::ExplodeF(',', &archivoTrips);

	int route_id, service_id, shape_id, direction_id,direction, trip_id, trip_headsign;
	string route_ant = "";

	for (int i = 0; i < cur.size(); i++) {
		if (cur[i] == "route_id")
			route_id = i;
		if (cur[i] == "service_id")
			service_id = i;
		if (cur[i] == "shape_id")
			shape_id = i;
		if (cur[i] == "direction_id")
			direction_id = i;
		if (cur[i] == "trip_id")
			trip_id = i;
		if (cur[i] == "trip_headsign")
			trip_headsign = i;
	}
	/// En algunos casos contiene el mismo direction_id para la Ida y Regreso, pero diferente trip_headsign
	map<string, vector<string>> route_direction;
	bool directionR = false;

	///Lectura archivo primario
	while (archivoTrips.good())
	{
		nlineas++;

		///Lectura de linea del archivo
		cur = StringFunctions::ExplodeF(',', &archivoTrips);

		///Condicion de salida, a veces no es suficiente solo la condicion del ciclo
		if (cur.size() == 0 || cur[0].compare("") == 0)
			continue;
		
		directionR = false;
		routeBytrip[cur[service_id]] = cur[route_id];
		map<string,string>::iterator itrips = trips.find(cur[shape_id]);
		map<string, string>::iterator ishort_name = map_servicios.find(cur[route_id]);
		map<string, int>::iterator idirection = directionByShape.find(cur[shape_id]);
		map<string, string>::iterator iservice = serviceByTrip.find(cur[trip_id]);
		map<string, vector<string>>::iterator iroute_direction = route_direction.find(cur[trip_headsign]);

		if (iroute_direction == route_direction.end()) {
			vector<string> aux;
			aux.push_back(cur[trip_headsign]);
			route_direction[cur[route_id]] = aux;
		}
		else {
			vector<string> aux = iroute_direction->second;
			if (aux[0] != cur[trip_headsign]) {
				directionR = true;
			}
		}
		
		if (iservice == serviceByTrip.end()) {
			if (atoi(cur[direction_id].c_str()) == 0 && !directionR)
				serviceByTrip[cur[trip_id]] = string(ishort_name->second + "I");
			else
				serviceByTrip[cur[trip_id]] = string(ishort_name->second + "R");
		}
		if (idirection == directionByShape.end()) {
			if (atoi(cur[direction_id].c_str()) == 0 && !directionR)
				directionByShape[cur[shape_id]] = atoi(cur[direction_id].c_str());
			else
				directionByShape[cur[shape_id]] = 1;
			map<string, string>::iterator iservice = dicSS.servicios.find(ishort_name->second);
			if (iservice != dicSS.servicios.end()) {
				dicSS.servicios.erase(iservice);
			}
			if (atoi(cur[direction_id].c_str()) == 0 && !directionR)
				dicSS.servicios[string(ishort_name->second + "I")] = ishort_name->second;
			else
				dicSS.servicios[string(ishort_name->second + "R")] = ishort_name->second;
		}
		if (itrips == trips.end()) {
			trips[cur[shape_id].c_str()] = ishort_name->second; //guarda short_name, key shape_id
		}
		else if(itrips != trips.end() && itrips->second != ishort_name->second){
			cout << "Mismo shape_id y diferente servicio!!" << endl;
		}
	}

	ofstream fout;
	fout.open("dicSS.txt");
	for (map<string, string>::iterator it = dicSS.servicios.begin(); it != dicSS.servicios.end(); it++)
		fout << (*it).first << ";" << (*it).second << endl;
	fout.close();

	reporte->tDiccionario = Cronometro::GetMilliSpan(nTimeStart) / 60000.0;

	cout << reporte->tDiccionario << "(min)" << endl;
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

	int shape_id, shape_pt_lat, shape_pt_lon, shape_pt_sequence;

	for (int i = 0; i < cur.size(); i++) {
		if (cur[i] == "shape_id")
			shape_id = i;
		if (cur[i] == "shape_pt_lat")
			shape_pt_lat = i;
		if (cur[i] == "shape_pt_lon")
			shape_pt_lon = i;
		if (cur[i] == "shape_pt_sequence")
			shape_pt_sequence = i;
	}

	string antServicio = string("-");
	bool unavez = true;
	map< string, Ruta >::iterator iserv_ant;
	Vector3D nodo_ant;
	int corr_ant;
	string sentido;

	double x, y;
	char UTMZone[5];

	///Lectura archivo primario
	while (archivoRutas.good())
	{
		nlineas++;

		///Lectura de linea del archivo
		cur = StringFunctions::ExplodeF(',', &archivoRutas);

		///Condicion de salida, a veces no es suficiente solo la condicion del ciclo
		if (cur.size() == 0 || cur[0].compare("") == 0)
			continue;

		///Generación de servicio-sentido
		map< string, string>::iterator itrips = trips.find(cur[shape_id]);
		map<string, int>::iterator idirection = directionByShape.find(cur[shape_id]);
		if (idirection != directionByShape.end()) {
			if (idirection->second == 0)
				sentido = "I";
			else
				sentido = "R";
		}else 
			sentido = "I";

		dicSS.servicios_rutas[string(itrips->second+sentido)] = string(itrips->second+sentido);

		///Transformacion de coordenadas
		ConvertCoordinate::LLtoUTM(23, atof(cur[shape_pt_lat].c_str()), atof(cur[shape_pt_lon].c_str()), y, x, UTMZone);


		map< string, Ruta >::iterator iserv = rutas.mapeo->find(string(itrips->second+sentido));

		if (iserv == rutas.mapeo->end())
		{
			Ruta ruta = Ruta(string(itrips->second+sentido));

			bool ok = ruta.AgregarNodo(Vector3D(x, y, 0.0), atoi(cur[shape_pt_sequence].c_str()));

			map<string, int>::iterator idirection = directionByShape.find(cur[shape_id]);
			
			rutas.mapeo->insert(pair<string, Ruta >(string(itrips->second+sentido), ruta));
			
			///Variable para reporte
			reporte->nRutas++;

		}
		else
		{
			bool ok = (*iserv).second.AgregarNodo(Vector3D(x, y, 0.0), atoi(cur[shape_pt_sequence].c_str()));


			///Chequeo nodos repetidos
			if (!ok)
				reporte->InsertaRutaError(string(itrips->second + sentido), 1, 0, 0);
		}

		///Parche para usar el ultimo nodo (Nodo Fin)
		if (unavez)
		{
			antServicio = string(itrips->second+sentido);
			iserv_ant = rutas.mapeo->find(string(itrips->second + sentido));
			unavez = false;
		}

		if (antServicio.compare(string(itrips->second + sentido)) != 0)
		{
			antServicio = string(itrips->second + sentido);
			bool ok = (*iserv_ant).second.AgregarNodo(nodo_ant, corr_ant + 1);
			iserv_ant = rutas.mapeo->find(string(itrips->second + sentido));

			///Chequeo nodos repetidos
			if (!ok)
				reporte->InsertaRutaError(string(itrips->second + sentido), 1, 0, 0);
		}

		nodo_ant = Vector3D(x, y, 0.0);
		corr_ant = atoi(cur[shape_pt_sequence].c_str());
	}


		rutas.CalculaLargosRutas();
		rutas.CalculaTramificado(parametros->distanciaTramado);
		rutas.SimplificaRutas();


		reporte->tRutas = Cronometro::GetMilliSpan(nTimeStart) / 60000.0;
		cout << reporte->tRutas << "(min)" << endl;
	
		
}

void FuenteDatos::leeRedDeParadas()
{
	int nTimeStart = Cronometro::GetMilliCount();

	///Archivo de entrada Principal
	ifstream archivoParaderos;
	if(par_duplicados)
		archivoParaderos.open(parametros->nombreCarpetaGTFS + parametros->slash + "stops_nuevo.txt");
	else
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

	int stop_id, stop_name, stop_lat, stop_lon;
	for (int i = 0; i < cur.size(); i++) {
		if (cur[i] == "stop_id")
			stop_id = i;
		if (cur[i] == "stop_name")
			stop_name = i;
		if (cur[i] == "stop_lat")
			stop_lat = i;
		if (cur[i] == "stop_lon")
			stop_lon = i;
	}

	double x, y;
	char UTMZone[5];

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

		ConvertCoordinate::LLtoUTM(23, atof(cur[stop_lat].c_str()), atof(cur[stop_lon].c_str()), y, x, UTMZone);

		//stop_id,stop_code,stop_name,stop_lat,stop_lon
		Paradero par = Paradero(atof(cur[stop_lat].c_str()), atof(cur[stop_lon].c_str()),(int)x, (int)y, cur[stop_id], cur[stop_name]);

		redParaderos.red[par.codigo] = par;
	}

	//CorrigeParadasMismaPosicion();

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

	const char *UTMZone = "18H";//ConvertCoordinate::getUTMZONE();
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
	if (par_duplicados)
		archivoParaderos.open(parametros->nombreCarpetaGTFS + parametros->slash + "stops_times_nuevo.txt");
	else
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

	int trip_id, stop_id, stop_sequence;
	for (int i = 0; i < cur.size(); i++) {
		if (cur[i] == "trip_id")
			trip_id = i;
		if (cur[i] == "stop_id")
			stop_id = i;
		if (cur[i] == "stop_sequence")
			stop_sequence = i;
	}

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
		//vector<string> cod = StringFunctions::Explode(cur[trip_id], '_');
		//vector<string> cod_serv = StringFunctions::Explode(cod[0], '-');
		
		//string servicio = string(cod_serv[0]);//+ cod_serv[1]);
		/// servicio+sentido
		map<string, string>::iterator iservice = serviceByTrip.find(cur[trip_id]);
		string servicio = iservice->second;

		if (servicio.compare(servicio_ant) != 0)
			activo = true;

		if (servicio.compare(servicio_ant) == 0 && cur[stop_sequence].compare("0") == 0)
			activo = false;


		///Busco paradero en red de paradas
		Paradero par;
		ired = redParaderos.red.find(cur[stop_id]);
		if (ired != redParaderos.red.end())
		{
			par = (*ired).second;
		}
		else
		{
			cout << "ERROR : paradero " << cur[stop_id] << " de stops_times.txt no se encuentra en stops.txt!" << endl;
			continue;
		}

		//map<string, string>::iterator irouteID = routeBytrip.find(servicio);
		//map<string, string>::iterator ishort_name = map_servicios.find(irouteID->second);
		iRuta = rutas.mapeo->find(servicio);//ishort_name->second); // find short name
		Vector3D p = Vector3D((*ired).second.x, (*ired).second.y, 0.0);
		float distance = -1;
		if (iRuta != rutas.mapeo->end())
		{
			distance = (*iRuta).second.GetDistanceOnRoute(&p);
		}

		if (activo)
		{
			///Construccion de secuencia
			iserv = secParaderos.secuencias.find(servicio);//ishort_name->second);
			if (iserv == secParaderos.secuencias.end())
			{
				map<int, Paradero> tmp;
				tmp[int(distance)] = par;
				secParaderos.secuencias[servicio] = tmp;//ishort_name->second] = tmp;
			}
			else
			{
				(*iserv).second[int(distance)] = par;
			}
		}

		///Construccion de secuencia
		iserv = secParaderosTODOS.secuencias.find(servicio);//ishort_name->second);
		if (iserv == secParaderosTODOS.secuencias.end())
		{
			map<int, Paradero> tmp;
			tmp[int(distance)] = par;
			secParaderosTODOS.secuencias[servicio] = tmp;//ishort_name->second] = tmp;
		}
		else
		{
			(*iserv).second[int(distance)] = par;
		}
			
		servicio_ant = servicio; //ishort_name->second;
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
	reporte->tParadas = Cronometro::GetMilliSpan(nTimeStart) / 60000.0;
	cout << reporte->tParadas << "(min)" << endl;

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
		cout << "Los servicios quedaran con un horario nulo. "  << endl;

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

			if((*iser).second.destino.compare(str)==0)
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

void FuenteDatos::leePuntosDeCargaBip()
{
	int nTimeStart = Cronometro::GetMilliCount();

	///Archivo de entrada Principal
	ifstream archivoPuntosBips;
	archivoPuntosBips.open(parametros->nombreArchivoPuntosCargaBip.c_str());

	///Chequeo de archivo 
	if (!archivoPuntosBips.good())
		cout << "Error : No se encuentra el archivo " << parametros->nombreArchivoPuntosCargaBip << "!" << endl;
	else
		cout << " Cargando red de puntos de carga... " << endl;

	///Vector contenedor de la linea actual del archivo
	vector<string> cur;

	///Iterador para buscar los paraderos
	map< int, PuntoBip >::iterator itPar;

	int id = 1;

	///Lectura del header
	cur = StringFunctions::ExplodeF(';', &archivoPuntosBips);

	double x, y;
	char UTMZone[5];

	///Lectura archivo primario
	while (archivoPuntosBips.good())
	{
		///Lectura de linea del archivo
		cur = StringFunctions::ExplodeF(';', &archivoPuntosBips);

		///Condicion de salida, a veces no es suficiente solo la condicion del ciclo
		if (cur.size() == 0 || cur[0].compare("") == 0)
		{
			//mde_->Advertencia("Advertencia : se ha encontrado linea vacia en " + parametros->nombreArchivoRedParadas + "!\n");
			continue;
		}

		ConvertCoordinate::LLtoUTM(23, atof(cur[2].c_str()), atof(cur[1].c_str()), y, x, UTMZone);

		//stop_id,stop_code,stop_name,stop_lat,stop_lon
		PuntoBip punto = PuntoBip(id,cur[0],atof(cur[2].c_str()), atof(cur[1].c_str()), (int)x, (int)y);

		puntosDeCargaBip[id] = punto;

		id++;
	}


	///DEBUG
	ofstream fout;
	fout.open("puntosCargaBip.txt");
	for (map< int, PuntoBip >::iterator it = puntosDeCargaBip.begin(); it != puntosDeCargaBip.end(); it++)
	{
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

void FuenteDatos::PreprocesaGTFS() {
	///Archivo de entrada Principal
	ifstream archivoParaderos;
	archivoParaderos.open(parametros->nombreCarpetaGTFS + parametros->slash + "stops.txt");

	///Chequeo de archivo 
	if (!archivoParaderos.good())
		cout << "Error : No se encuentra el archivo " << parametros->nombreCarpetaGTFS + parametros->slash + "stops.txt" << "!" << endl;
	else
		cout << " Verificando existencia de paraderos duplicados... " << endl;

	///Vector contenedor de la linea actual del archivo
	vector<string> cur;

	///Lectura del header
	cur = StringFunctions::ExplodeF(',', &archivoParaderos);

	int stop_id, stop_lat, stop_lon;
	for (int i = 0; i < cur.size(); i++) {
		if (cur[i] == "stop_id")
			stop_id = i;
		if (cur[i] == "stop_lat")
			stop_lat = i;
		if (cur[i] == "stop_lon")
			stop_lon = i;
	}

	map<pair<string,string>, string> stops;
	map<string, vector<string>> stops_id;
	map<string, string> mapeo_stops_id;
	vector<string> out;
	string aux = "";
	for (int i = 0; i < cur.size(); i++) {
		if (cur[i] == "-")
			aux += ",";
		else if (cur[i] != "")
			aux += cur[i] + ",";
	}
	out.push_back(aux);
	///Lectura archivo primario
	while (archivoParaderos.good())
	{
		cur = StringFunctions::ExplodeF(',', &archivoParaderos);

		///Condicion de salida, a veces no es suficiente solo la condicion del ciclo
		if (cur.size() == 0 || cur[0].compare("") == 0)
		{
			//mde_->Advertencia("Advertencia : se ha encontrado linea vacia en " + parametros->nombreArchivoRedParadas + "!\n");
			continue;
		}

		map<pair<string, string>, string>::iterator istops = stops.find(pair<string,string>(cur[stop_lat],cur[stop_lon]));
		

		if (istops == stops.end()) {
			mapeo_stops_id[cur[stop_id]] = cur[stop_id];
			stops[pair<string, string>(cur[stop_lat], cur[stop_lon])] = cur[stop_id];
			if (cur[stop_id] == "527") {
				cout << "ulala" << endl;
			}
			aux="";
			for (int i = 0; i < cur.size(); i++) {
				if (cur[i] == "-")
					aux +=",";
				else if(cur[i] != "")
					aux += cur[i] + ",";
			}
			out.push_back(aux);
		}
		else {
			mapeo_stops_id[cur[stop_id]] = istops->second;
			map<string, vector<string>>::iterator istops_id = stops_id.find(istops->second);
			if (istops_id == stops_id.end()) {
				vector<string> aux;
				aux.push_back(cur[stop_id]);
				stops_id[istops->second] = aux;
			}
			else {
				vector<string> aux = istops_id->second;
				aux.push_back(cur[stop_id]);
				stops_id[istops->second] = aux;
			}
		}
	}
	
	if (stops_id.size() > 0) {
		par_duplicados = true;
		cout << "El numero de paraderos (sin contar los repetidos): " << endl;
		cout << out.size() << endl;
		cout << "Los repetidos son: " << stops_id.size() << endl;

		ofstream fout;
		fout.open(parametros->nombreCarpetaGTFS + parametros->slash + "stops_nuevo.txt");
		for (int i = 0; i < out.size(); i++)
			fout << out[i] << endl;
		fout.close();


		///Archivo de entrada Principal
		ifstream archivoParadas;
		archivoParadas.open(parametros->nombreCarpetaGTFS + parametros->slash + "stop_times.txt");

		///Chequeo de archivo 
		if (!archivoParadas.good())
			cout << "Error : No se encuentra el archivo " << parametros->nombreCarpetaGTFS + parametros->slash + "stop_times.txt" << "!" << endl;
		else
			cout << "Quitando paraderos duplicados del archivo stop_times.txt " << endl;

		///Vector contenedor de la linea actual del archivo
		cur.clear();

		///Lectura del header
		cur = StringFunctions::ExplodeF(',', &archivoParadas);

		for (int i = 0; i < cur.size(); i++) {
			if (cur[i] == "stop_id")
				stop_id = i;
		}

		out.clear();
		aux = "";
		for (int i = 0; i < cur.size(); i++) {
			if (cur[i] == "-")
				aux += ",";
			else if (cur[i] != "")
				aux += cur[i] + ",";
		}
		out.push_back(aux);
		///Lectura archivo primario
		while (archivoParadas.good())
		{
			cur = StringFunctions::ExplodeF(',', &archivoParadas);

			///Condicion de salida, a veces no es suficiente solo la condicion del ciclo
			if (cur.size() == 0 || cur[0].compare("") == 0)
			{
				//mde_->Advertencia("Advertencia : se ha encontrado linea vacia en " + parametros->nombreArchivoRedParadas + "!\n");
				continue;
			}

			map<string, string>::iterator istops_id = mapeo_stops_id.find(cur[stop_id]);
			if (istops_id != mapeo_stops_id.end()) {
				aux = "";
				for (int i = 0; i < cur.size(); i++) {
					if (i == stop_id)
						aux += istops_id->second + ",";
					else if (cur[i] == "-")
						aux += ",";
					else if (cur[i] != "")
						aux += cur[i] + ",";
				}
				out.push_back(aux);
			}
			else {
				cout << "ERROR: No se ha encontrado el stop: " << cur[stop_id] << " en el archivo stop_times.txt" << endl;
			}

		}
		fout.open(parametros->nombreCarpetaGTFS + parametros->slash + "stops_times_nuevo.txt");
		for (int i = 0; i < out.size(); i++)
			fout << out[i] << endl;
		fout.close();
	}
}
