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
		string modo; 
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
		
		if(isec_ant == fdd_->secuencias.end())
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
				//if ((*isec).second.codigo.compare("Rei3-I-L") == 0) cout << "aqui 10 : " << endl;

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
					color_ser = (*iserv).second.color;
					modo = (*iserv).second.tipo;

					if (sentido.compare("I") == 0)
						nombre = (*iserv).second.destino;
					else
						nombre = (*iserv).second.origen;
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
						color_ser = (*iserv).second.color;
						modo = (*iserv).second.tipo;
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

				min_hora_ini = 999999;
				max_hora_fin = -1;
				
				///caso de ultimo registro
				if (next(isec) == fdd_->secuencias.end())
				{
					cout << "ENTREEEE" << endl;

					vector<string> cod_cur = StringFunctions::Explode((*isec).second.codigo, '-');

					nombre = string("-");
					color_ser = string("-");
					modo = string("-");
					map< string, Servicio >::iterator iserv = fdd_->servicios.find(cod_cur[0]);
					if (iserv != fdd_->servicios.end())
					{
						color_ser = (*iserv).second.color;
						modo = (*iserv).second.tipo;

						if (cod_ant[1].compare("I") == 0)
							nombre = (*iserv).second.destino;
						else
							nombre = (*iserv).second.origen;
					}

					color_ser = string("-");
					modo = string("-");
					//map< string, Servicio >::iterator iserv = fdd_->servicios.find(cod_cur[0]);
					if (iserv != fdd_->servicios.end())
					{
						color_ser = (*iserv).second.color;
						modo = (*iserv).second.tipo;
					}

					string horario = (*isec).second.hora_ini + "-" + (*isec).second.hora_fin;

					string secParadas;
					for (map<int, string>::iterator ipar = (*isec_ant).second.paradas.begin(); ipar != (*isec_ant).second.paradas.end(); ipar++)
					{
						if (ipar == (*isec_ant).second.paradas.begin())
							secParadas = (*ipar).second;
						else
							secParadas += "-" + (*ipar).second;

					}

					busStopSequence bss;
					bss.servicio = cod_cur[0];
					bss.sentido = cod_cur[1];
					bss.tipodia = cod_cur[2];
					bss.color = color_ser;
					bss.horario = horario;
					bss.nombre = nombre;
					bss.paradas = secParadas;
					bss.shape_id = (*isec).second.shape_id;
					bss.modo = modo;

					string key = servicio + ";" + sentido + ";" + tipoDia + ";" + secParadas;
					itseq = secuenciasPorHorario.find(key);
					if (itseq == secuenciasPorHorario.end())
					{
						cout << "SIII : " << key << endl;
						secuenciasPorHorario[key] = bss;
					}
					else
					{
						(*itseq).second.horario += "/" + horario;
					}

				}
			}
		}


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
				codigo_cur += variante1[1] ;
			else if (variante2.size() == 2)
				codigo_cur += variante2[1] ;
			else if (variante1.size() == 3)
				codigo_cur += variante1[1] + "I" + variante1[2] ;
			else if (variante2.size() == 3)
				codigo_cur += variante2[1] + "R" + variante2[2] ;
			else
				codigo_cur += "-" ;
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
        fout << (*itseq).second.nombre << ";";
		fout << tmp[3] << endl;
	}
	fout.close();


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
