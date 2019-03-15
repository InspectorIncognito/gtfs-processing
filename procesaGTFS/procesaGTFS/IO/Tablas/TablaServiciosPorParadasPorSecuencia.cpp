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

	cout << "Proceso X : Creando e imprimiendo tabla secuencias por servicio... " ;

	map<string, FuenteDatos::Secuencia >::iterator isec;

	struct busStopSequence {
		string servicio;
		string sentido;
		string tipodia;
		string color;
		string horario;
		string nombre;
		string paradas;

		string shape_id;
	};


	///Construyendo estrucutura de secuencias por servicio-horario
	map< string, busStopSequence> secuenciasPorHorario;
	map< string, busStopSequence>::iterator itseq;
	int min_hora_ini = 999999;
	int max_hora_fin = -1;
	for (isec = fdd_->secuencias.begin(); isec != fdd_->secuencias.end(); isec++)
	{
		map<string, FuenteDatos::Secuencia >::iterator isec_ant;
		isec_ant = isec;
		isec_ant--;

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
			vector<string> cod_ant = StringFunctions::Explode((*isec_ant).second.codigo, '-');

			string servicio = cod_ant[0];
			string sentido = cod_ant[1];
			string tipoDia = cod_ant[2];

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
			if ((*isec).second.codigo.compare((*isec_ant).second.codigo) == 0 && sonIguales)
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
				map< string, Servicio >::iterator iserv = fdd_->servicios.find(servicio);
				if (iserv != fdd_->servicios.end())
				{
					if (sentido.compare("I") == 0)
						nombre = (*iserv).second.destino;
					else
						nombre = (*iserv).second.origen;
				}

				if (min_hora_ini == 999999 || max_hora_fin == -1)
				{
					string color;
					map<string, string>::iterator iiit = fdd_->dicSS.colores.find((*iserv).first);
					if (iiit != fdd_->dicSS.colores.end())
						color = (*iiit).second;
					else
					{
						//cout << "ERROR : No se encontro el servicio " << (*iserv).first << " en la tabla de colores." << endl;
						color = "0";
					}
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
					bss.color = color;
					bss.horario = horario;
					bss.nombre = nombre;
					bss.paradas = secParadas;
					bss.shape_id = shape_id;

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
				else
				{
					string color;
					map<string, string>::iterator iiit = fdd_->dicSS.colores.find((*iserv).first);
					if (iiit != fdd_->dicSS.colores.end())
						color = (*iiit).second;
					else
					{
						//cout << "ERROR : No se encontro el servicio " << (*iserv).first << " en la tabla de colores." << endl;
						color = "0" ;
					}

					//fout << fdd_->tsh.Seconds2TimeStampInDay(min_hora_ini) << ";";
					string horario = fdd_->tsh.Seconds2TimeStampInDay(min_hora_ini);
					if (max_hora_fin == 86400)
					{
						horario += "-" + fdd_->tsh.Seconds2TimeStampInDay(max_hora_fin - 1);
						//fout << fdd_->tsh.Seconds2TimeStampInDay(max_hora_fin - 1) << ";";
						//fout << nombre << " " << tipoDia << " (" << fdd_->tsh.Seconds2TimeStampInDay(min_hora_ini) << " - " << fdd_->tsh.Seconds2TimeStampInDay(max_hora_fin - 1) << ")" << ";";
					}
					else
					{
						horario += "-" + fdd_->tsh.Seconds2TimeStampInDay(max_hora_fin);
						//fout << fdd_->tsh.Seconds2TimeStampInDay(max_hora_fin) << ";";
						//fout << nombre << " " << tipoDia << " (" << fdd_->tsh.Seconds2TimeStampInDay(min_hora_ini) << " - " << fdd_->tsh.Seconds2TimeStampInDay(max_hora_fin) << ")" << ";";
					}

					string secParadas;
					for (map<int, string>::iterator ipar = (*isec_ant).second.paradas.begin(); ipar != (*isec_ant).second.paradas.end(); ipar++)
					{
						if (ipar == (*isec_ant).second.paradas.begin())
							secParadas = (*ipar).second;
						else
							secParadas += "-" + (*ipar).second;

					}
					//fout << endl;

					busStopSequence bss;
					bss.servicio = servicio;
					bss.sentido = sentido;
					bss.tipodia = tipoDia;
					bss.color = color;
					bss.horario = horario;
					bss.nombre = nombre;
					bss.paradas = secParadas;
					bss.shape_id = shape_id;

					
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
	fout.open("Android_busstops_sequences" + fdd_->parametros->version + ".csv");
	fout << "servicio;sentido;variante;tipodia;shape_id;horario;color_id;direccion;paradas" << endl;
	for (itseq = secuenciasPorHorario.begin(); itseq != secuenciasPorHorario.end(); itseq++)
	{
		vector<string> tmp = StringFunctions::Explode((*itseq).first, ';');

		vector<string> shape_id = StringFunctions::Explode((*itseq).second.shape_id, '_');
		vector<string> variante1 = StringFunctions::Explode(shape_id[0], 'I');
		vector<string> variante2 = StringFunctions::Explode(shape_id[0], 'R');

		map< string, Ruta >::iterator iruta = fdd_->rutas.mapeo->find(shape_id[0]);
		if (iruta == fdd_->rutas.mapeo->end())
			cout << "WTF : " << shape_id[0] << endl;

		fout << tmp[0] << ";";
		fout << tmp[1] << ";";

		if(variante1.size()==2)
			fout << variante1[1] << ";";
		else if (variante2.size() == 2)
			fout << variante2[1] << ";";
		else if (variante1.size() == 3)
			fout << variante1[1] << "I" << variante1[2] << ";";
		else if (variante2.size() == 3)
			fout << variante2[1] << "R" << variante2[2] << ";";
		else
			fout << "-" << ";";

		fout << tmp[2] << ";";
		fout << shape_id[0] << ";";
		fout << (*itseq).second.horario << ";";
		fout << (*itseq).second.color << ";";
		fout << toCamelCase(string((*itseq).second.nombre+";"));
		fout << tmp[3] << endl;
	}
	fout.close();

	/*
	ofstream fout;
	fout.open("Android_busstops_sequences" + fdd_->parametros->version + ".csv");
	fout << "servicio;sentido;tipodia;color_id;horario;destino;direccion;codigo_ruta;paradas" << endl;
	for (itseq = secuenciasPorHorario.begin(); itseq != secuenciasPorHorario.end(); itseq++)
	{
		vector<string> tmp = StringFunctions::Explode((*itseq).first, ';');

		fout << tmp[0] << ";";
		fout << tmp[1] << ";";
		fout << tmp[2] << ";";
		fout << (*itseq).second.color << ";";
		fout << (*itseq).second.horario << ";";
		fout << toCamelCase(string((*itseq).second.nombre + ";"));
		fout << toCamelCase(string((*itseq).second.nombre + " " + tmp[2] + " (" + (*itseq).second.horario + ");"));
		fout << "-" << ";";
		fout << tmp[3] << endl;
	}
	fout.close();
	*/

	/*
	///DEBUG
	ofstream fout;
	fout.open("Android_busstops_sequences" + fdd_->parametros->version + ".csv");
	fout << "servicio;sentido;tipodia;color_id;hora_ini;hora_fin;direccion;paradas" << endl;
	int min_hora_ini = 999999;
	int max_hora_fin = -1;
	for (isec = fdd_->secuencias.begin(); isec != fdd_->secuencias.end(); isec++)
	{
		map<string, FuenteDatos::Secuencia >::iterator isec_ant;
		map<string, FuenteDatos::Secuencia >::iterator isec_sgt;
		isec_ant = isec;
		isec_ant--;
		isec_sgt = isec;
		isec_sgt++;


		if (isec == fdd_->secuencias.begin())
		{
			int ihora_ini = fdd_->tsh.Time2Seconds((*isec).second.hora_ini);
			int ihora_fin = fdd_->tsh.Time2Seconds((*isec).second.hora_fin);

			if (ihora_ini <= min_hora_ini) min_hora_ini = ihora_ini;
			if (ihora_fin >= max_hora_fin) max_hora_fin = ihora_fin;
		}
		else
			//if (isec_ant != secuencias.end())
		{
			vector<string> cod_ant = StringFunctions::Explode((*isec_ant).second.codigo, '-');

			string servicio = cod_ant[0];
			string sentido = cod_ant[1];
			string tipoDia = cod_ant[2];

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
			if ((*isec).second.codigo.compare((*isec_ant).second.codigo) == 0 && sonIguales)
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
				map< string, Servicio >::iterator iserv = fdd_->servicios.find(servicio);
				if (iserv != fdd_->servicios.end())
				{
					if (sentido.compare("I") == 0)
						nombre = (*iserv).second.destino;
					else
						nombre = (*iserv).second.origen;
				}


				if (min_hora_ini == 999999 || max_hora_fin == -1)
				{
					//fout << (*isec_ant).first << ";";
					fout << servicio << ";";
					fout << sentido << ";";
					fout << tipoDia << ";";

					map<string, string>::iterator iiit = fdd_->dicSS.colores.find((*iserv).first);
					if (iiit != fdd_->dicSS.colores.end())
						fout << (*iiit).second << ";";
					else
					{
						cout << "ERROR : No se encontro el servicio " << (*iserv).first << " en la tabla de colores." << endl;
						fout << "0" << ";";
					}

					fout << (*isec_ant).second.hora_ini << ";";
					fout << (*isec_ant).second.hora_fin << ";";
					fout << nombre << " " << tipoDia << " (" << (*isec_ant).second.hora_ini << " - " << (*isec_ant).second.hora_fin << ")" << ";";
					for (map<int, string>::iterator ipar = (*isec_ant).second.paradas.begin(); ipar != (*isec_ant).second.paradas.end(); ipar++)
					{
						if (ipar == (*isec_ant).second.paradas.begin())
							fout << (*ipar).second;
						else
							fout << "-" << (*ipar).second;
					}
					fout << endl;
				}
				else
				{
					//fout << (*isec_ant).first << ";";
					fout << servicio << ";";
					fout << sentido << ";";
					fout << tipoDia << ";";
					map<string, string>::iterator iiit = fdd_->dicSS.colores.find((*iserv).first);
					if (iiit != fdd_->dicSS.colores.end())
						fout << (*iiit).second << ";";
					else
					{
						cout << "ERROR : No se encontro el servicio " << (*iserv).first << " en la tabla de colores." << endl;
						fout << "0" << ";";
					}

					fout << fdd_->tsh.Seconds2TimeStampInDay(min_hora_ini) << ";";

					if (max_hora_fin == 86400)
					{
						fout << fdd_->tsh.Seconds2TimeStampInDay(max_hora_fin - 1) << ";";
						fout << nombre << " " << tipoDia << " (" << fdd_->tsh.Seconds2TimeStampInDay(min_hora_ini) << " - " << fdd_->tsh.Seconds2TimeStampInDay(max_hora_fin - 1) << ")" << ";";
					}
					else
					{
						fout << fdd_->tsh.Seconds2TimeStampInDay(max_hora_fin) << ";";
						fout << nombre << " " << tipoDia << " (" << fdd_->tsh.Seconds2TimeStampInDay(min_hora_ini) << " - " << fdd_->tsh.Seconds2TimeStampInDay(max_hora_fin) << ")" << ";";
					}


					for (map<int, string>::iterator ipar = (*isec_ant).second.paradas.begin(); ipar != (*isec_ant).second.paradas.end(); ipar++)
					{
						if (ipar == (*isec_ant).second.paradas.begin())
							fout << (*ipar).second;
						else
							fout << "-" << (*ipar).second;

					}
					fout << endl;
				}

				min_hora_ini = 999999;
				max_hora_fin = -1;
			}
		}
	}
	fout.close();
	*/


	cout << Cronometro::GetMilliSpan( nTimeStart )/60000.0 << "(min)" << endl;
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

	return out;
}