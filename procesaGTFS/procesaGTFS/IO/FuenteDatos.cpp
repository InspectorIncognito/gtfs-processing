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

	///Inicializacion de reporte
	this->reporte = new ReporteFuenteDatos(parametros->carpetaReportes);

	///Lectura de diccionario de codigos servicio-sentido
	leeDiccionarioServicios();

	///Lectura de las rutas
	leeRutas();

	///Lectura de la red de paraderos
	leeRedDeParadas();

	///Instancia de lectura de secuencia de paraderos
	leeSecuenciaDeParadas();

	///Lectura de horarios de servicios
	leeHorarios();

	///Creacion de directorio para almacenar los kmls	
	system(string("mkdir " + parametros->carpetaKmls).c_str());
	system(string("mkdir " + parametros->carpetaReportes).c_str());
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

	///Lectura del header
	cur = StringFunctions::ExplodeF(',', &archivoDiccionario);


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

		std::locale loc;
		std::string str = cur[3];
		for (std::string::size_type i = 0; i < cur[3].length(); ++i)
			str[i] = std::toupper(cur[3][i], loc);

		vector<string> od = StringFunctions::Explode(str, '-');

		Servicio ser;
		if (od.size() == 2)
			ser = Servicio(cur[0], od[0].substr(0,od[0].length()-1), od[1].substr(1, od[1].length()), cur[7]);
		else if (od.size() == 1)
			ser = Servicio(cur[0], od[0].substr(0, od[0].length() - 1), "", cur[7]);
		else
			cout << "ERROR : Servicio no bien definido en datos de entrada(routes.txt)!" << endl;

		servicios[ser.nombre] = ser;

		///Insercion en diccionario de servicios-sentidos global
		dicSS.servicios[string(cur[0] + "I")] = cur[0];
		dicSS.servicios[string(cur[0] + "R")] = cur[0];

		///identificacion de colores
		map<string, string>::iterator icolor = parametros->mapeoColores.find(cur[7]);

		if (icolor != parametros->mapeoColores.end())
		{
			dicSS.colores[cur[0]] = (*icolor).second;

		}
			
		

	}


	///DEBUG
	ofstream fout;
	fout.open("dicSS.csv");
	for (map<string, string>::iterator it = dicSS.servicios.begin(); it != dicSS.servicios.end(); it++)
		fout << (*it).first << ";" << (*it).second << endl;
	fout.close();

	reporte->tDiccionario = Cronometro::GetMilliSpan(nTimeStart) / 60000.0;

	cout << reporte->tDiccionario << "(min)" << endl;

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


		///Generacion del codigo servicio-sentido concatenando las 3 columnas servicio-sentido-variante
		vector<string> cod = StringFunctions::Explode(cur[0],'_');

		if (cod.size() != 2)
			continue;

		dicSS.servicios_rutas[cod.at(0)] = cod.at(0);

		///Transformacion de coordenadas
		ConvertCoordinate::LLtoUTM(23, atof(cur[1].c_str()), atof(cur[2].c_str()), y, x, UTMZone);

		map< string, Ruta >::iterator iserv = rutas.mapeo->find(cod.at(0));

		if (iserv == rutas.mapeo->end())
		{
			Ruta ruta = Ruta(cod.at(0));

			bool ok = ruta.AgregarNodo(Vector3D(x,y,0.0),atoi(cur[3].c_str()));

			rutas.mapeo->insert(pair<string, Ruta >(cod.at(0), ruta));

			///Variable para reporte
			reporte->nRutas++;

			///Chequeo del nodo respecto a una zona cuadrada que encierra la zona metropolitana de santiago
			if (!estaEnSantiago(x, y) )
				reporte->InsertaRutaError(cod.at(0), 0, 0, 1);
		}
		else
		{
			bool ok = (*iserv).second.AgregarNodo(Vector3D(x,y,0.0),atoi(cur[3].c_str()));

			///Chequeo del nodo respecto a una zona cuadrada que encierra la zona metropolitana de santiago
			if (!estaEnSantiago(x, y))
				reporte->InsertaRutaError(cod.at(0), 0, 0, 1);

			///Chequeo nodos repetidos
			if (!ok)
				reporte->InsertaRutaError(cod.at(0), 1, 0, 0);
		}

		///Parche para usar el ultimo nodo (Nodo Fin)
		if (unavez)
		{
			antServicio = cod.at(0);
			iserv_ant = rutas.mapeo->find(cod.at(0));
			unavez = false;
		}

		if (antServicio.compare(cod.at(0)) != 0)
		{
			antServicio = cod.at(0);
			bool ok = (*iserv_ant).second.AgregarNodo(nodo_ant, corr_ant + 1);
			iserv_ant = rutas.mapeo->find(cod.at(0));

			///Chequeo del nodo respecto a una zona cuadrada que encierra la zona metropolitana de santiago
			if (!estaEnSantiago(int(nodo_ant.x), int(nodo_ant.y)))
				reporte->InsertaRutaError(cod.at(0), 0, 0, 1);

			///Chequeo nodos repetidos
			if (!ok)
				reporte->InsertaRutaError(cod.at(0), 1, 0, 0);
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

	reporte->tRutas = Cronometro::GetMilliSpan(nTimeStart) / 60000.0;
	cout << reporte->tRutas << "(min)" << endl;

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

		ConvertCoordinate::LLtoUTM(23, atof(cur[3].c_str()), atof(cur[4].c_str()), y, x, UTMZone);

		//stop_id,stop_code,stop_name,stop_lat,stop_lon
		Paradero par = Paradero(atof(cur[3].c_str()), atof(cur[4].c_str()),(int)x, (int)y, cur[0], cur[2]);

		redParaderos.red[par.codigo] = par;
	}

	///DEBUG
	ofstream fout;
	fout.open("redParadas.csv");
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
		vector<string> cod = StringFunctions::Explode(cur[0], '_');
		vector<string> cod_serv = StringFunctions::Explode(cod[0], '-');
		
		string servicio = string(cod_serv[0] + cod_serv[1]);

		if (servicio.compare(servicio_ant) != 0)
			activo = true;

		if (servicio.compare(servicio_ant) == 0 && cur[4].compare("1") == 0)
			activo = false;

		if (activo)
		{
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

			///Construccion de secuencia
			iserv = secParaderos.secuencias.find(servicio);
			if (iserv == secParaderos.secuencias.end())
			{
				map<int, Paradero> tmp;
				tmp[ atoi(cur[4].c_str()) ] = par;
				secParaderos.secuencias[servicio] = tmp;
			}
			else
			{
				(*iserv).second[atoi(cur[4].c_str())] = par;
			}
		}


			
		servicio_ant = servicio;
	}

	///DEBUG
	ofstream fout;
	fout.open("secParadas.csv");
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

			//cout << (*iser).second.destino << "|" << str << endl;
			if((*iser).second.destino.compare(str)==0)
				(*iser).second.horarioI.append(cur.at(2) + "," + cur.at(3) + "," + cur.at(4) + "-");
			else
				(*iser).second.horarioR.append(cur.at(2) + "," + cur.at(3) + "," + cur.at(4) + "-");
		}
	}


	///DEBUG
	ofstream fout;
	fout.open("servicios_horario.csv");
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


