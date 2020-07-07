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
#include "TablaServiciosPorParadasPorSecuencia.h"

TablaServiciosPorParadasPorSecuencia::TablaServiciosPorParadasPorSecuencia(FuenteDatos *fdd)
{

	this->fdd_ = fdd;

	Crear();
}

TablaServiciosPorParadasPorSecuencia::~TablaServiciosPorParadasPorSecuencia()
{

}

void TablaServiciosPorParadasPorSecuencia::Crear()
{
	int nTimeStart = Cronometro::GetMilliCount();

	cout << "Proceso X : Creando e imprimiendo tabla secuencias por servicio... " << endl;

	buildSequenceByScheduleItinerary();


	cout << Cronometro::GetMilliSpan( nTimeStart )/60000.0 << "(min)" << endl;
}

void TablaServiciosPorParadasPorSecuencia::buildSequenceByScheduleItinerary()
{
	///Ordenando secuencias por servicio
	map< string, map<string,FuenteDatos::Secuencia> > secuenciasPorServicio;
	map< string, map<string, FuenteDatos::Secuencia> >::iterator itsecps;
	for (map<string, FuenteDatos::Secuencia >::iterator isec = fdd_->secuencias.begin(); isec != fdd_->secuencias.end(); isec++)
	{
		string key = (*isec).second.route_id+"_"+(*isec).second.route_direction_id + "_" + (*isec).second.tipodia;
		itsecps = secuenciasPorServicio.find(key);
		if (itsecps == secuenciasPorServicio.end())
		{
			map<string, FuenteDatos::Secuencia> tmp;
			tmp[(*isec).first] = (*isec).second;
			secuenciasPorServicio[key] = tmp;
		}
		else
		{
			(*itsecps).second[(*isec).first] = (*isec).second;
		}
	}

	
	ofstream dbg;
	dbg.open("secuenciasPorServicio.txt");
	for (itsecps = secuenciasPorServicio.begin(); itsecps != secuenciasPorServicio.end(); itsecps++)
	{
		for (map<string, FuenteDatos::Secuencia>::iterator it = (*itsecps).second.begin(); it != (*itsecps).second.end(); it++)
		{
			vector<string> servicio = StringFunctions::Explode((*itsecps).first, '_');
			map<string, Servicio>::iterator iserv = fdd_->servicios.find(servicio.at(0));

			dbg << (*itsecps).first << "|";
			dbg << (*it).first << "|";
			dbg << (*it).second.shape_id << "|";
			dbg << (*it).second.hora_ini << "|";
			dbg << (*it).second.hora_fin << "|";
			dbg << (*iserv).second.route_short_name << endl;
		}
	}
	dbg.close();

	///Deteccion de horarios por servicio
	map< string, map<string,int> > horariosPorServicio;
	map< string, map<string,int> >::iterator ithorario;
	for (itsecps = secuenciasPorServicio.begin(); itsecps != secuenciasPorServicio.end(); itsecps++)
	{
		for (map<string, FuenteDatos::Secuencia>::iterator isec = (*itsecps).second.begin(); isec != (*itsecps).second.end(); isec++)
		{
			ithorario = horariosPorServicio.find((*itsecps).first);
			if (ithorario == horariosPorServicio.end())
			{
				map<string,int> tmp;
				tmp[(*isec).second.hora_ini] = 1;
				horariosPorServicio[(*itsecps).first] = tmp;
			}
			else
			{
				map<string, int>::iterator itmh = (*ithorario).second.find((*isec).second.hora_ini);
				if (itmh == (*ithorario).second.end())
					(*ithorario).second[(*isec).second.hora_ini] = 1;
				else
					(*ithorario).second[(*isec).second.hora_ini]++;
			}
		}
	}

	ofstream dbg0;
	dbg0.open("mediasHorasPorServicio.txt");
	for (ithorario = horariosPorServicio.begin(); ithorario != horariosPorServicio.end(); ithorario++)
	{
		for (map<string, int>::iterator imh = (*ithorario).second.begin(); imh != (*ithorario).second.end(); imh++)
		{
			dbg0 << (*ithorario).first << "|";
			dbg0 << (*imh).first << "|";
			dbg0 << (*imh).second << endl;
		}
	}
	dbg0.close();

	///Construccion de horarrios por servicio
	map<string, map< int, int> > bloquesHorariosPorServicio;
	map<string, map< int, int> >::iterator itbloquepser;
	map<int, int>::iterator itbloque;
	for (ithorario = horariosPorServicio.begin(); ithorario != horariosPorServicio.end(); ithorario++)
	{
		map<int, int> tmp;
		map<string, int>::iterator imh_next;
		int ultimo_bloque;
		for (map<string, int>::iterator imh = (*ithorario).second.begin(); imh != (*ithorario).second.end(); imh++)
		{
			///construyo el primer bloque
			if (imh == (*ithorario).second.begin())
			{
				map<int, int> tmp;
				tmp[fdd_->tsh.Time2Seconds((*imh).first)] = fdd_->tsh.Time2Seconds((*imh).first) + 3600;
				bloquesHorariosPorServicio[(*ithorario).first] = tmp;
				ultimo_bloque = fdd_->tsh.Time2Seconds((*imh).first);
			}
			else
			{
				itbloquepser = bloquesHorariosPorServicio.find((*ithorario).first);

				if (itbloquepser == bloquesHorariosPorServicio.end())
				{
					cout << "ERROR : no se encuentra bloques para el servicio : " << (*ithorario).first << endl;
					exit(1);
				}

				imh_next = imh;
				imh_next++;

				if (imh_next != (*ithorario).second.end())
				{
					int mh_sec = fdd_->tsh.Time2Seconds((*imh).first);
					int mh_sec_next = fdd_->tsh.Time2Seconds((*imh_next).first);
					
					//si son colindantes
					if (mh_sec_next - mh_sec == 3600)
					{
						itbloque = (*itbloquepser).second.find(ultimo_bloque);
						if (itbloque != (*itbloquepser).second.end())
						{
							(*itbloque).second = fdd_->tsh.Time2Seconds((*imh_next).first)+3600;
						}
						else
						{
							cout << "ERROR : caso raro #1 " << endl;
						}
					}
					else
					{
						(*itbloquepser).second[fdd_->tsh.Time2Seconds((*imh_next).first)] = fdd_->tsh.Time2Seconds((*imh_next).first) + 3600;
						ultimo_bloque = fdd_->tsh.Time2Seconds((*imh_next).first);
					}
				}
				else
				{

				}
			}
		}
	}

	ofstream dbg1;
	dbg1.open("BloquesPorServicio.txt");
	for (itbloquepser = bloquesHorariosPorServicio.begin(); itbloquepser != bloquesHorariosPorServicio.end(); itbloquepser++)
	{
		for (map<int, int>::iterator imh = (*itbloquepser).second.begin(); imh != (*itbloquepser).second.end(); imh++)
		{
			dbg1 << (*itbloquepser).first << "|";
			dbg1 << fdd_->tsh.Seconds2TimeStampInDay((*imh).first) << "|";
			dbg1 << fdd_->tsh.Seconds2TimeStampInDay((*imh).second) << endl;
		}
	}
	dbg1.close();
	
	///Construccion de dato para imprimir archivo de salida
	for (itsecps = secuenciasPorServicio.begin(); itsecps != secuenciasPorServicio.end(); itsecps++)
	{
		map<string, FuenteDatos::Secuencia>::iterator itsec0 = (*itsecps).second.begin();

		//cout << (*itsec0).second.route_id << "|";
		//cout << (*itsec0).second.route_direction_id << "|";
		//cout << (*itsec0).second.route_short_name << "|";
		//cout << (*itsec0).second.shape_id << "|";
		//cout << (*itsec0).second.tipodia << endl;

		//vector<string> servicioSentido = StringFunctions::Explode((*itsecps).first, '_');

		//if (servicioSentido.size() != 3)
		//	cout << "ERROR : codificacion de servicio con problemas : " << (*itsecps).first << endl;

		busStopSequence bss;

		///Obtencion de color, modo y nombre
		map< string, Servicio >::iterator iserv = fdd_->servicios.find((*itsec0).second.route_id);
		if (iserv != fdd_->servicios.end())
		{
			bss.color = (*iserv).second.route_color;
			bss.modo = (*iserv).second.route_type;
			bss.nombre = (*iserv).second.route_long_name;
			bss.resources_id = (*iserv).second.route_resource_id;
		}
		else
		{
			cout << "ERROR : No se encuentra servicio en estructura de servicios : " << (*itsec0).second.route_id << endl;
			exit(1);
		}
		
		///Obtencion de horario
		itbloquepser = bloquesHorariosPorServicio.find((*itsecps).first);
		if (itbloquepser != bloquesHorariosPorServicio.end())
		{
			for (map<int, int>::iterator itbloque = (*itbloquepser).second.begin(); itbloque != (*itbloquepser).second.end(); itbloque++)
			{
				if(itbloque == (*itbloquepser).second.begin())
					bss.horario = fdd_->tsh.Seconds2TimeStampInDay((*itbloque).first)+"-"+ fdd_->tsh.Seconds2TimeStampInDay((*itbloque).second);
				else
					bss.horario += "|" + fdd_->tsh.Seconds2TimeStampInDay((*itbloque).first) + "-" + fdd_->tsh.Seconds2TimeStampInDay((*itbloque).second);
			}
		}
		else
		{
			cout << "ERROR : No se encuentra servicio en listado de bloques de horarios : " << (*itsecps).first << endl;
		}
		
		///Obtener secuencia de paradas
		for (map<int, string>::iterator ipar = (*itsec0).second.paradas.begin(); ipar != (*itsec0).second.paradas.end(); ipar++)
		{
			if (ipar == (*itsec0).second.paradas.begin())
				bss.paradas = (*ipar).second;
			else
				bss.paradas += "|" + (*ipar).second;

		}
		
		bss.servicio = (*itsec0).second.route_short_name;
		
		if((*itsec0).second.route_direction_id.compare("0")==0)
			bss.sentido = "I";
		else
			bss.sentido = "R";

		bss.tipodia = (*itsec0).second.tipodia;
		bss.shape_id = (*itsec0).second.shape_id;
		bss.destino = (*itsec0).second.destino;

		
		string key = bss.servicio + ";" + bss.sentido + ";" + bss.tipodia + ";" + bss.paradas;
		
		itseq = secuenciasPorHorario.find(key);
		if (itseq == secuenciasPorHorario.end())
		{
			//cout << "FLAG 1 : " << key << endl;
			secuenciasPorHorario[key] = bss;
		}
		else
		{
			//cout << "FLAG 2 : " << key << endl;
			cout << "Que onda, otra vez el mismo servicio : " << key << endl;
		}
	}


	ofstream fout;
	fout.open(string(fdd_->parametros->carpetaOutput + "/" + fdd_->parametros->version + "/" + "PhoneStopsSequences.csv").c_str());
	map< string, busStopSequence>::iterator itseq_ant;
	int variante_int = 0;
	fout << "modo;servicio;sentido;variante;tipodia;shape_id;horario;color_id;direccion;paradas;resources_id" << endl;
	for (itseq = secuenciasPorHorario.begin(); itseq != secuenciasPorHorario.end(); itseq++)
	{

		itseq_ant = itseq;
		itseq_ant--;
		vector<string> tmp = StringFunctions::Explode((*itseq).first, ';');
		fout << (*itseq).second.modo << ";";
		fout << tmp[0] << ";";
		fout << tmp[1] << ";";
		fout << "-" << ";";
		fout << (*itseq).second.tipodia << ";";
		fout << (*itseq).second.shape_id << ";";
		fout << (*itseq).second.horario << ";";
		fout << (*itseq).second.color << ";";
		//fout << toCamelCase(string((*itseq).second.nombre + ";"));
		fout << (*itseq).second.destino << ";";
		fout << tmp[3] << ";";
		fout << (*itseq).second.resources_id << endl;
	}
	fout.close();
	
}

void TablaServiciosPorParadasPorSecuencia::buildSequenceByScheduleFrequencies()
{
	map<string, FuenteDatos::Secuencia >::iterator isec;

	///Construyendo estrucutura de secuencias por servicio-horario
	int min_hora_ini = 999999;
	int max_hora_fin = -1;

	for (isec = fdd_->secuencias.begin(); isec != fdd_->secuencias.end(); isec++)
	{
		map<string, FuenteDatos::Secuencia >::iterator isec_ant;
		isec_ant = isec;
		isec_ant--;

		if (isec_ant == fdd_->secuencias.end())
			continue;

		if ((*isec).second.hora_ini.compare("-") == 0 || (*isec).second.hora_fin.compare("-") == 0 || (*isec_ant).second.hora_ini.compare("-") == 0 || (*isec_ant).second.hora_fin.compare("-") == 0)
			continue;

		if (isec == fdd_->secuencias.begin())
		{
			int ihora_ini = fdd_->tsh.Time2Seconds((*isec).second.hora_ini);
			int ihora_fin = fdd_->tsh.Time2Seconds((*isec).second.hora_fin);

			if (ihora_ini <= min_hora_ini) min_hora_ini = ihora_ini;
			if (ihora_fin >= max_hora_fin) max_hora_fin = ihora_fin;
		}
		else
		{
			vector<string> cod_ant = StringFunctions::Explode((*isec_ant).second.shape_id, '_');

			string servicio = cod_ant[0];
			string sentido = cod_ant[1];
			string tipoDia = (*isec_ant).second.tipodia;

			string shape_id = (*isec_ant).second.shape_id;

			//chequeo igualdad
			bool sonIguales = true;
			if ((int)(*isec).second.paradas.size() != (int)(*isec_ant).second.paradas.size())
			{
				sonIguales = false;
			}
			else
			{
				map<int, string>::iterator ipar1 = (*isec).second.paradas.begin();
				map<int, string>::iterator ipar2 = (*isec_ant).second.paradas.begin();
				for (; ipar1 != (*isec).second.paradas.end(); ipar1++, ipar2++)
				{
					if ((*ipar1).second.compare((*ipar2).second) != 0)
						sonIguales = false;
				}
			}

			///codigo anterior igual, se fusionan horarios
			if ((*isec).second.shape_id.compare((*isec_ant).second.shape_id) == 0 && sonIguales)
			{
				int ihora_ini_ant = fdd_->tsh.Time2Seconds((*isec_ant).second.hora_ini);
				int ihora_fin_ant = fdd_->tsh.Time2Seconds((*isec_ant).second.hora_fin);

				int ihora_ini = fdd_->tsh.Time2Seconds((*isec).second.hora_ini);
				int ihora_fin = fdd_->tsh.Time2Seconds((*isec).second.hora_fin);

				if (ihora_ini_ant <= min_hora_ini) min_hora_ini = ihora_ini_ant;
				if (ihora_ini <= min_hora_ini) min_hora_ini = ihora_ini;

				if (ihora_fin_ant >= max_hora_fin) max_hora_fin = ihora_fin_ant;
				if (ihora_fin >= max_hora_fin) max_hora_fin = ihora_fin;
			}
			else
			{
				string nombre = string("-");
				string color_ser = string("-");
				string modo = string("-");
				map< string, Servicio >::iterator iserv = fdd_->servicios.find(servicio);
				if (iserv != fdd_->servicios.end())
				{
					color_ser = (*iserv).second.route_color;
					modo = (*iserv).second.route_type;

					nombre = (*iserv).second.route_long_name;
					//if (sentido.compare("I") == 0)
					//	nombre = (*iserv).second.destino;
					//else
					//	nombre = (*iserv).second.origen;
				}

				if (min_hora_ini == 999999 || max_hora_fin == -1)
				{
					string secParadas;
					for (map<int, string>::iterator ipar = (*isec_ant).second.paradas.begin(); ipar != (*isec_ant).second.paradas.end(); ipar++)
					{
						if (ipar == (*isec_ant).second.paradas.begin())
							secParadas = (*ipar).second;
						else
							secParadas += "-" + (*ipar).second;
					}
					string horario = (*isec_ant).second.hora_ini + "-" + (*isec_ant).second.hora_fin;

					busStopSequence bss;
					bss.servicio = servicio;
					bss.sentido = sentido;
					bss.tipodia = tipoDia;
					bss.color = color_ser;
					bss.horario = horario;
					bss.nombre = nombre;
					bss.paradas = secParadas;
					bss.shape_id = shape_id;
					bss.modo = modo;

					string key = servicio + ";" + sentido + ";" + tipoDia + ";" + secParadas;

					itseq = secuenciasPorHorario.find(key);
					if (itseq == secuenciasPorHorario.end())
					{
						secuenciasPorHorario[key] = bss;
					}
					else
					{
						(*itseq).second.horario += "/" + horario;
					}
				}
				else
				{
					string color_ser = string("-");
					string modo = string("-");
					map< string, Servicio >::iterator iserv = fdd_->servicios.find(servicio);
					if (iserv != fdd_->servicios.end())
					{
						color_ser = (*iserv).second.route_color;
						modo = (*iserv).second.route_type;
					}

					string horario = fdd_->tsh.Seconds2TimeStampInDay(min_hora_ini);
					if (max_hora_fin == 86400)
					{
						horario += "-" + fdd_->tsh.Seconds2TimeStampInDay(max_hora_fin - 1);
					}
					else
					{
						horario += "-" + fdd_->tsh.Seconds2TimeStampInDay(max_hora_fin);
					}

					string secParadas;
					for (map<int, string>::iterator ipar = (*isec_ant).second.paradas.begin(); ipar != (*isec_ant).second.paradas.end(); ipar++)
					{
						if (ipar == (*isec_ant).second.paradas.begin())
							secParadas = (*ipar).second;
						else
							secParadas += "-" + (*ipar).second;

					}

					busStopSequence bss;
					bss.servicio = servicio;
					bss.sentido = sentido;
					bss.tipodia = tipoDia;
					bss.color = color_ser;
					bss.horario = horario;
					bss.nombre = nombre;
					bss.paradas = secParadas;
					bss.shape_id = shape_id;
					bss.modo = modo;


					string key = servicio + ";" + sentido + ";" + tipoDia + ";" + secParadas;
					//cout << "FLAG 0 : " << key << endl;
					itseq = secuenciasPorHorario.find(key);
					if (itseq == secuenciasPorHorario.end())
					{
						//cout << "FLAG 1 : " << key << endl;
						secuenciasPorHorario[key] = bss;
					}
					else
					{
						//cout << "FLAG 2 : " << key << endl;
						(*itseq).second.horario += "/" + horario;
					}
				}

				min_hora_ini = 999999;
				max_hora_fin = -1;
			}
		}
		//cout << "FLAG -2" << endl;
	}

	ofstream fout;
	fout.open(string(fdd_->parametros->carpetaOutput + "/" + fdd_->parametros->version + "/" + "PhoneStopsSequences.csv").c_str());
	//fout.open("Android_busstops_sequences" + fdd_->parametros->version + ".csv");
	map< string, busStopSequence>::iterator itseq_ant;
	int variante_int = 0;
	fout << "modo;servicio;sentido;variante;tipodia;shape_id;horario;color_id;direccion;paradas" << endl;
	for (itseq = secuenciasPorHorario.begin(); itseq != secuenciasPorHorario.end(); itseq++)
	{
		itseq_ant = itseq;
		itseq_ant--;
		vector<string> tmp = StringFunctions::Explode((*itseq).first, ';');

		vector<string> shape_id = StringFunctions::Explode((*itseq).second.shape_id, '_');
		vector<string> variante1 = StringFunctions::Explode(shape_id[0], 'I');
		vector<string> variante2 = StringFunctions::Explode(shape_id[0], 'R');

		fout << (*itseq).second.modo << ";";
		fout << tmp[0] << ";";
		fout << tmp[1] << ";";

		if (itseq_ant != secuenciasPorHorario.end())
		{
			vector<string> tmp_pre = StringFunctions::Explode((*itseq_ant).first, ';');
			vector<string> shape_id_pre = StringFunctions::Explode((*itseq_ant).second.shape_id, '_');
			vector<string> variante1_pre = StringFunctions::Explode(shape_id_pre[0], 'I');
			vector<string> variante2_pre = StringFunctions::Explode(shape_id_pre[0], 'R');

			string codigo_pre;
			string codigo_cur;

			codigo_cur = tmp[0] + "|" + tmp[1] + "|";
			if (variante1.size() == 2)
				codigo_cur += variante1[1];
			else if (variante2.size() == 2)
				codigo_cur += variante2[1];
			else if (variante1.size() == 3)
				codigo_cur += variante1[1] + "I" + variante1[2];
			else if (variante2.size() == 3)
				codigo_cur += variante2[1] + "R" + variante2[2];
			else
				codigo_cur += "-";
			codigo_cur += "|" + tmp[2];

			codigo_pre = tmp_pre[0] + "|" + tmp_pre[1] + "|";
			if (variante1_pre.size() == 2)
				codigo_pre += variante1_pre[1];
			else if (variante2_pre.size() == 2)
				codigo_pre += variante2_pre[1];
			else if (variante1_pre.size() == 3)
				codigo_pre += variante1_pre[1] + "I" + variante1_pre[2];
			else if (variante2_pre.size() == 3)
				codigo_pre += variante2_pre[1] + "R" + variante2_pre[2];
			else
				codigo_pre += "-";
			codigo_pre += "|" + tmp_pre[2];

			if (codigo_cur.compare(codigo_pre) == 0)
			{
				fout << variante_int++ << ";";
			}
			else
			{
				if (variante1.size() == 2)
				{
					fout << variante1[1] << ";";
				}
				else if (variante2.size() == 2)
				{
					fout << variante2[1] << ";";
				}
				else if (variante1.size() == 3)
				{
					fout << variante1[1] << "I" << variante1[2] << ";";
				}
				else if (variante2.size() == 3)
				{
					fout << variante2[1] << "R" << variante2[2] << ";";
				}
				else
					fout << "-" << ";";
			}

		}
		else
		{
			if (variante1.size() == 2)
			{
				fout << variante1[1] << ";";
			}
			else if (variante2.size() == 2)
			{
				fout << variante2[1] << ";";
			}
			else if (variante1.size() == 3)
			{
				fout << variante1[1] << "I" << variante1[2] << ";";
			}
			else if (variante2.size() == 3)
			{
				fout << variante2[1] << "R" << variante2[2] << ";";
			}
			else
				fout << "-" << ";";
		}

		fout << tmp[2] << ";";
		fout << shape_id[0] << ";";
		fout << (*itseq).second.horario << ";";
		fout << (*itseq).second.color << ";";
		//fout << toCamelCase(string((*itseq).second.nombre + ";"));
		fout << (*itseq).second.nombre << ";";
		fout << tmp[3] << endl;
	}
	fout.close();
}

string TablaServiciosPorParadasPorSecuencia::EliminaCadenasBlancos(string in)
{
	string out;
	int nBlancos = 0;
	for (int i = 0; i < in.size(); i++)
	{
		if (in.at(i) == ' ')
			nBlancos++;
		else
			nBlancos = 0;

		if (nBlancos < 2)
			out.push_back(in.at(i));
	}

	return out;
	//return in;
}	

string TablaServiciosPorParadasPorSecuencia::toCamelCase(string in)
{
	string out;
	std::locale loc;

	for (int i = 0; i < in.size(); i++)
	{
		if (i == 0)
		{
			out.push_back(std::toupper(in.at(i), loc));
		}
		else
		{
			int ant = i - 1;
			if (in.at(ant) == ' ' || in.at(ant) == '(')
				out.push_back(std::toupper(in.at(i), loc));
			else
				out.push_back(std::tolower(in.at(i), loc));

		}
	}

	//out = corrigeArticulos(out);

	return out;
}

string TablaServiciosPorParadasPorSecuencia::corrigeArticulos(string in)
{
	string out;

	
	if (in.compare("(M) U. De Chile;") == 0)
	{
		out = std::regex_replace(in, regex(" De "), " de ");
		cout << "FLUG : " << out << endl;
		out = std::regex_replace(in, regex(" La "), " la ");
		out = std::regex_replace(in, regex(" Las "), " las ");
		out = std::regex_replace(in, regex(" El "), " el ");
		out = std::regex_replace(in, regex(" Los "), " los ");
	}

	return out;
}
