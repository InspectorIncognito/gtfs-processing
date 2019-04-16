/**
* Project: "pvm".
* Package: Utilidades
* Class: FuenteDatos
* Created by: Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* Terms: These coded instructions, statements, and computer programs are
* produced as part of a software project. They contain unpublished
* propietary information and are protected by copyright law. They may
* not be disclosed to third parties or copied or duplicated in any form,
* in whole or in part, without the prior written consent of Pragxis SpA.
* Copyright:  Pragxis (c) 2013
* Last modified : Mauricio Zuñiga 15-04-2013
*/
#include "TimeStampHandler.h"

TimeStampHandler::TimeStampHandler()
{
      vector<int> anoNormal;
      vector<int> anoBisiesto;

      anoNormal.push_back(31);anoNormal.push_back(28); anoNormal.push_back(31); anoNormal.push_back(30); anoNormal.push_back(31); anoNormal.push_back(30);
      anoNormal.push_back(31);anoNormal.push_back(31); anoNormal.push_back(30); anoNormal.push_back(31); anoNormal.push_back(30); anoNormal.push_back(31);

      anoBisiesto.push_back(31);anoBisiesto.push_back(29); anoBisiesto.push_back(31); anoBisiesto.push_back(30); anoBisiesto.push_back(31); anoBisiesto.push_back(30);
      anoBisiesto.push_back(31);anoBisiesto.push_back(31); anoBisiesto.push_back(30); anoBisiesto.push_back(31); anoBisiesto.push_back(30); anoBisiesto.push_back(31);
      
//       dias.insert(pair< int , vector<int> >(2008,anoBisiesto));
//       dias.insert(pair< int , vector<int> >(2009,ano));
      dias.insert(pair< int , vector<int> >(2010,anoNormal));
      dias.insert(pair< int , vector<int> >(2011,anoNormal));
      dias.insert(pair< int , vector<int> >(2012,anoBisiesto));
      dias.insert(pair< int , vector<int> >(2013,anoNormal));
      dias.insert(pair< int , vector<int> >(2014,anoNormal));
      dias.insert(pair< int , vector<int> >(2015,anoNormal));
      dias.insert(pair< int , vector<int> >(2016,anoBisiesto));
      dias.insert(pair< int , vector<int> >(2017,anoNormal));
      dias.insert(pair< int , vector<int> >(2018,anoNormal));
      dias.insert(pair< int , vector<int> >(2019,anoNormal));
      dias.insert(pair< int , vector<int> >(2020,anoBisiesto));
}

TimeStampHandler::~TimeStampHandler()
{
}

string TimeStampHandler::TimeStampStandar2(string timestamp)
{
	vector<string> fecha_hora = StringFunctions::Explode(timestamp, ' ' );
	vector<string> fecha = StringFunctions::Explode(fecha_hora[0], '-' );
	vector<string> hora = StringFunctions::Explode(fecha_hora[1], '-' );

	return string(fecha[2]+"-"+fecha[1]+"-"+fecha[0]+" "+hora[0]+":"+hora[1]+":"+hora[2]);
}

string TimeStampHandler::TimeStampStandar(string t)
{
	string out=string("1111-11-11 11:11:11");
	///11112233445566

	out.at(0) = t.at(0);
	out.at(1) = t.at(1);
	out.at(2) = t.at(2);
	out.at(3) = t.at(3);

	out.at(5) = t.at(4);
	out.at(6) = t.at(5);

	out.at(8) = t.at(6);
	out.at(9) = t.at(7);

	out.at(11) = t.at(8);
	out.at(12) = t.at(9);

	out.at(14) = t.at(10);
	out.at(15) = t.at(11);

	out.at(17) = t.at(12);
	out.at(18) = t.at(13);

	return out;
	//return t.substr(0,4)+"-"+t.substr(4,2)+"-"+t.substr(6,2)+" "+t.substr(8,2)+":"+t.substr(10,2)+":"+t.substr(12,2);
}

string TimeStampHandler::Seconds2TimeStamp(int seconds)
{
      int ndias=0;

	  if(seconds < 0)
		  return string("-");
      
	string out;
	double ndiasInput = seconds/(24*60*60);
	int segundos = seconds%(24*60*60);
	
	
	for(map< int , vector<int> >::iterator idias=dias.begin();idias!=dias.end();idias++)
	{
	      for(int i=0 ;i < (*idias).second.size(); i++)	
	      {
			  if(double(ndias+(*idias).second.at(i)) >= ndiasInput )
			  {
				///Agno
				out.append(StringFunctions::Int2String((*idias).first));
				out.append("-");
			
				///Mes
				if(i+1 >=0 && i+1 <=9)
					out.append("0");	
				out.append(StringFunctions::Int2String(i+1));
				out.append("-");
			
				///Dia
				int dia = int(floor(ndiasInput)) - ndias;
				if(dia >=0 && dia <=9)
					out.append("0");	
				out.append(StringFunctions::Int2String(dia));
				out.append(" ");
			
				out.append(Seconds2TimeStampInDay(segundos));
			
			
				return out;
			  }
			  ndias+=(*idias).second.at(i);
	      }
	}
	return out;
}

vector<int> TimeStampHandler::Seconds2TimeStampInVector(int seconds)
{
	  vector<int> out;

      int ndias=0;

	  if(seconds < 0)
	  {
		  cout << "Error : segundos negativos en el tiempo : " << seconds << endl;
	  }
      
	//string out;
	double ndiasInput = seconds/(24*60*60);
	int segundos = seconds%(24*60*60);
	
	
	for(map< int , vector<int> >::iterator idias=dias.begin();idias!=dias.end();idias++)
	{
	      for(int i=0 ;i < (*idias).second.size(); i++)	
	      {
			  if(double(ndias+(*idias).second.at(i)) >= ndiasInput )
			  {
				///Agno
				out.push_back((*idias).first);
			
				///Mes
				out.push_back(i+1);
			
				///Dia
				int dia = int(floor(ndiasInput)) - ndias;
				out.push_back(dia);
			
				//out.append(Seconds2TimeStampInDay(segundos));
				int segundosEnDia;
				if(segundos >= 86400 )
					segundosEnDia= segundos%(24*60*60);
				else
					segundosEnDia= segundos;

				int hora = segundosEnDia/(60*60);
				int min = (segundosEnDia - hora*60*60)/60;
				int seg = (segundosEnDia - hora*60*60)%60;

				out.push_back(hora);
		
				out.push_back(min);
		
				out.push_back(seg);
			
				return out;
			  }
			  ndias+=(*idias).second.at(i);
	      }
	}
	return out;
}

int TimeStampHandler::TimeStamp2Seconds(string time)
{
	//en date_time[0] fecha, date_time[1] hora
	vector<string> date_time;
	vector<string> date;
	vector<string> time1;

	date_time = StringFunctions::Explode(time, ' ' );

	int ano,mes,dia,hora,min,seg;
	
	if(date_time.size() == 2)
	{
		date = StringFunctions::Explode(date_time[0],'-');
		if(date.size()==3)
		{
			ano=atoi(date[0].c_str());
			mes=atoi(date[1].c_str());
			dia=atoi(date[2].c_str());
		}
		else
		{
			date = StringFunctions::Explode(date_time[0],'/');

			if(date.size()!=3)
			{
				cout << "ERROR : Error en formato de fecha : " << date_time[0] << endl;
				exit(1);
			}
			ano=atoi(date[2].c_str());
			mes=atoi(date[1].c_str());
			dia=atoi(date[0].c_str());
		}
		
		time1 = StringFunctions::Explode(date_time[1],':');	
	
		hora=atoi(time1[0].c_str());
		min=atoi(time1[1].c_str());
		seg=atoi(time1[2].c_str());
	
		return TimeStampHandler::nSeconds(ano,mes,dia,hora,min,seg);
	}
	else
	{
		cout << "ERROR : TimeStampHandler::TimeStamp2Seconds(string time) : Falta un termino " << time << endl;
	}
	
	return -1;
}

int TimeStampHandler::Time2Seconds(string time)
{
	vector<string> time1 = StringFunctions::Explode(time,':');
		
	if(time1.size() == 3 )
	{
		int hora=atoi(time1[0].c_str());
		int min =atoi(time1[1].c_str());
		int seg =atoi(time1[2].c_str());
		
		return seg+min*60+hora*3600;
	}
	else
	{
		cout << "ERROR : Formato de hora no estandar : " << time << endl;
		exit(1);
	}
	return -1;
}

string TimeStampHandler::Seconds2TimeStampInDay(int seconds)
{
	string out;

	if(seconds < 0)
		return string("-");

	if (seconds == 86400)
		return string("24:00:00");

	int segundosEnDia;
	if(seconds >= 86400 )
		segundosEnDia= seconds%(24*60*60);
	else
		segundosEnDia= seconds;

	int hora = segundosEnDia/(60*60);
	int min = (segundosEnDia - hora*60*60)/60;
	int seg = (segundosEnDia - hora*60*60)%60;

	if(hora >=0 && hora <=9)
		out.append("0");	
	out.append(StringFunctions::Int2String(hora));
	out.append(":");
		
	if(min >=0 && min <=9)
		out.append("0");	
	out.append(StringFunctions::Int2String(min));
	out.append(":");
		
	if(seg >=0 && seg <=9)
		out.append("0");	
	out.append(StringFunctions::Int2String(seg));

	return out;
}

int TimeStampHandler::Seconds2SecondsInDay(int seconds)
{
	///86400 = 24*60*60
	if(seconds >= 86400 )
		return seconds%(24*60*60);
	else
		return seconds;
}

int TimeStampHandler::nSeconds(int ano, int mes, int dia, int hora, int min, int seg)
{
      return seg+min*60+hora*3600+nDias(ano,mes,dia)*24*3600;
}

int TimeStampHandler::nDias(int ano, int mes,int dia)
{
      int ndias=0;
      
      for(map< int , vector<int> >::iterator idias=dias.begin();idias!=dias.end();idias++)
      {
	      if((*idias).first < ano)
	      {
		    for(int i=0 ;i < (*idias).second.size(); i++)	
			ndias+=(*idias).second.at(i);
	      }
	      else if((*idias).first == ano)
	      {
		    for(int i=0 ;i < mes-1; i++)	
			ndias+=(*idias).second.at(i);
	      }
	      else
	      {
	      }
      }

      return ndias+dia;
}

string TimeStampHandler::RedondeaMediaHora(string time)
{
	vector<string> time1 = StringFunctions::Explode(time,':');
	
	int hora=atoi(time1[0].c_str());
	int min=atoi(time1[1].c_str());
	int seg=atoi(time1[2].c_str());

	if(min >= 0 && min <30)
		min = 0;
	else
		min = 30;

	seg = 0;

	string output("");
	output.append(time1[0]);
	output.append(":");
	
	if(min >=0 && min <=9)
		output.append("0");	
	output.append(StringFunctions::Int2String(min));
	output.append(":");
		
	if(seg >=0 && seg <=9)
		output.append("0");	
	output.append(StringFunctions::Int2String(seg));


	return output;

}

string TimeStampHandler::DateStandar(string date_)
{
	vector<string> date = StringFunctions::Explode(date_, '/' );

	return string(date[2]+"-"+date[1]+"-"+date[0]);
}

int TimeStampHandler::Time2Minutes(string time)
{
	vector<string> t;

	t = StringFunctions::Explode(time,':');

	return atoi(t[0].c_str())*60 + atoi(t[1].c_str());
}

string TimeStampHandler::DeleteMiliSeconds(string timestamp)
{
	vector<string> date_time= StringFunctions::Explode(timestamp, '.' );

	return date_time[0];
}

string TimeStampHandler::SumaMinutos(string t1,string t2)
{
	//en date_time[0] fecha, date_time[1] hora
	vector<string> date_time;
	vector<string> date;
	vector<string> time;
	string out("");
	
	date_time = StringFunctions::Explode(t1, ' ' );
	date = StringFunctions::Explode(date_time[0],'-');
	time = StringFunctions::Explode(date_time[1],':');
	
	int ano,mes,dia,hora,min,seg;
	
	ano=atoi(date[0].c_str());
	mes=atoi(date[1].c_str());
	dia=atoi(date[2].c_str());

	hora=atoi(time[0].c_str());
	min=atoi(time[1].c_str());
	seg=atoi(time[2].c_str());

	int dm = atoi(t2.c_str());
	
	if(dm+min >= 60)
	{
		hora+=int((dm+min)/60);
		min=(dm+min)%60;
		if(hora > 23)
		{
			hora = hora - 24;
			dia++;
			
			if(dia > nDiasDelMes(ano,mes))
			{
				dia = 1;
				mes++;
			}
		}
	}
	else
	{
		min=dm+min;
	}
	
	out.append(StringFunctions::Int2String(ano));
	out.append("-");
		
	if(mes >=0 && mes <=9)
		out.append("0");
	out.append(StringFunctions::Int2String(mes));
	out.append("-");
		
	if(dia >=0 && dia <=9)
		out.append("0");
	out.append(StringFunctions::Int2String(dia));	
	out.append(" ");
		
	if(hora >=0 && hora <=9)
		out.append("0");	
	out.append(StringFunctions::Int2String(hora));
	out.append(":");
		
	if(min >=0 && min <=9)
		out.append("0");	
	out.append(StringFunctions::Int2String(min));
	out.append(":");
		
	if(seg >=0 && seg <=9)
		out.append("0");	
	out.append(StringFunctions::Int2String(seg));
	
	return out;
}

string TimeStampHandler::SumaMinutosEnDia(string t1,string t2)
{
	//en date_time[0] fecha, date_time[1] hora
	vector<string> time;
	string out("");

	int hora,min,seg;

	time = StringFunctions::Explode(t1,':');

	hora=atoi(time[0].c_str());
	min=atoi(time[1].c_str());
	seg=atoi(time[2].c_str());

	int dm = atoi(t2.c_str());
	
	if(dm+min >= 60)
	{
		hora+=int((dm+min)/60);
		min=(dm+min)%60;
		if(hora > 23)
		{
			hora = hora - 24 ;
		}
	}
	else
	{
		min=dm+min;
	}
	
	if(hora >=0 && hora <=9)
		out.append("0");	
	out.append(StringFunctions::Int2String(hora));
	out.append(":");
		
	if(min >=0 && min <=9)
		out.append("0");	
	out.append(StringFunctions::Int2String(min));
	out.append(":");
		
	if(seg >=0 && seg <=9)
		out.append("0");	
	out.append(StringFunctions::Int2String(seg));
	
	return out;
}

int TimeStampHandler::getDeltaSeconds(string t1,string t2)
{
	int tt1 = TimeStamp2Seconds(t1);
	int tt2 = TimeStamp2Seconds(t2);
	return tt2 - tt1;
}

int TimeStampHandler::nDiasDelMes(int ano, int mes)
{
      int ano2008[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
      int ano2009[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
      int ano2010[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
      int ano2011[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
      int ano2012[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
	  int ano2013[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	  int ano2014[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	  int ano2015[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	  int ano2016[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
      int ndias=0;
      
	if(ano==2008)
		return ano2008[mes-1];

	if(ano==2009)
		return ano2009[mes-1];
		
	if(ano==2010)
		return ano2010[mes-1];

	if(ano==2011)
		return ano2011[mes-1];
		
	if(ano==2012)
		return ano2012[mes-1];

	if(ano==2013)
		return ano2013[mes-1];

	if(ano==2014)
		return ano2014[mes-1];

	if(ano==2015)
		return ano2015[mes-1];

	if(ano==2016)
		return ano2016[mes-1];

	cout << "No existe año para este mes!!: TimeStampHandler::nDiasDelMes(int ano,int mes)" << endl;
	return -1;
}

string TimeStampHandler::ExtractTime(string time)
{
	//en date_time[0] fecha, date_time[1] hora
	vector<string> date_time;
	vector<string> date;
	vector<string> time1;
	
	date_time = StringFunctions::Explode(time, ' ' );
	
	date = StringFunctions::Explode(date_time[0],'-');
	time1 = StringFunctions::Explode(date_time[1],':');
	
	//int ano,mes,dia,hora,min,seg;
	
	string output("");
	output.append(time1[0]);
	output.append("_");
	output.append(time1[1]);
	output.append("_");
	output.append(time1[2]);


	return output;
}

string TimeStampHandler::ExtractDate(string time)
{
 	vector<string> date_time;
	
	date_time = StringFunctions::Explode(time, ' ' );
	
	return date_time[0]; 
}

string TimeStampHandler::ExtractTime2Puntos(string time)
{
	//en date_time[0] fecha, date_time[1] hora
	vector<string> date_time;
	vector<string> date;
	vector<string> time1;
	
	date_time = StringFunctions::Explode(time, ' ' );
	
	date = StringFunctions::Explode(date_time[0],'-');
	time1 = StringFunctions::Explode(date_time[1],':');
	
	//int ano,mes,dia,hora,min,seg;
	
	string output("");
	output.append(time1[0]);
	output.append(":");
	output.append(time1[1]);
	output.append(":");
	output.append(time1[2]);


	return output;
}

string TimeStampHandler::ChangeFormatTime(string time)
{
	vector<string> t = StringFunctions::Explode(time,':');
	string output(t[0]);
	output.append("_");
	output.append(t[1]);
	output.append("_");
	output.append(t[2]);
	
	return output;

}

int TimeStampHandler::ExtractHour(string time)
{
	//en date_time[0] fecha, date_time[1] hora
	vector<string> date_time;
	vector<string> date;
	vector<string> time1;
	
	date_time = StringFunctions::Explode(time, ' ' );
	
	date = StringFunctions::Explode(date_time[0],'-');
	time1 = StringFunctions::Explode(date_time[1],':');
	
	int ano,mes,dia,hora,min,seg;
	
	ano=atoi(date[0].c_str());
	mes=atoi(date[1].c_str());
	dia=atoi(date[2].c_str());

	hora=atoi(time1[0].c_str());
	min=atoi(time1[1].c_str());
	seg=atoi(time1[2].c_str());


	return hora;
}

int TimeStampHandler::ExtractDay(string time)
{
	//en date_time[0] fecha, date_time[1] hora
	vector<string> date_time;
	vector<string> date;
	vector<string> time1;
	
	date_time = StringFunctions::Explode(time, ' ' );
	
	date = StringFunctions::Explode(date_time[0],'-');
	time1 = StringFunctions::Explode(date_time[1],':');
	
	int ano,mes,dia,hora,min,seg;
	
	ano=atoi(date[0].c_str());
	mes=atoi(date[1].c_str());
	dia=atoi(date[2].c_str());

	hora=atoi(time1[0].c_str());
	min=atoi(time1[1].c_str());
	seg=atoi(time1[2].c_str());


	return dia;
}

string TimeStampHandler::Redondea10Min(string time)
{
	//en date_time[0] fecha, date_time[1] hora
	vector<string> date_time;
	vector<string> date;
	vector<string> time1;
	
	date_time = StringFunctions::Explode(time, ' ' );
	
	int ano,mes,dia,hora,min,seg;
	
	date = StringFunctions::Explode(date_time[0],'-');
	time1 = StringFunctions::Explode(date_time[1],':');
	
	ano=atoi(date[0].c_str());
	mes=atoi(date[1].c_str());
	dia=atoi(date[2].c_str());

	hora=atoi(time1[0].c_str());
	min=atoi(time1[1].c_str());
	seg=atoi(time1[2].c_str());
	
	min = int(min/10.0)*10;
	seg = 0;

	string output("");
	output.append(date[0]);
	output.append("-");
	output.append(date[1]);
	output.append("-");
	output.append(date[2]);
	output.append(" ");
	output.append(time1[0]);
	output.append(":");
	
	if(min >=0 && min <=9)
		output.append("0");	
	output.append(StringFunctions::Int2String(min));
	output.append(":");
		
	if(seg >=0 && seg <=9)
		output.append("0");	
	output.append(StringFunctions::Int2String(seg));


	return output;
// 	return -1;
}

int TimeStampHandler::TimeStamp2SecondsInDay(string time)
{
	//en date_time[0] fecha, date_time[1] hora
	vector<string> date_time;
	vector<string> date;
	vector<string> time1;
	date_time = StringFunctions::Explode(time, ' ' );
	
	int hora,min,seg;
	
	if(date_time.size() == 2)
	{
		time1 = StringFunctions::Explode(date_time[1],':');	
	
		hora=atoi(time1[0].c_str());
		min=atoi(time1[1].c_str());
		seg=atoi(time1[2].c_str());
	
		return seg+min*60+hora*3600;
	}
	else if(date_time.size() == 1)
	{
		time1 = StringFunctions::Explode(date_time[0],':');
		
		if(time1.size() == 3 )
		{
			hora=atoi(time1[0].c_str());
			min=atoi(time1[1].c_str());
			seg=atoi(time1[2].c_str());
		
			return seg+min*60+hora*3600;
		}
		else
		{
			return -1;
		}
	}

	return -1;	
}

//int TimeStampHandler::nDias(int ano, int mes,int dia)
//{
//      int ano2008[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
//      int ano2009[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
//      int ano2010[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
//      int ano2011[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
//      int ano2012[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
//      int ndias=0;
//      
//	if(ano==2008)
//		for(int i=0;i<mes-1;i++)	
//			ndias+=ano2008[i];
//
//	if(ano==2009)
//		for(int i=0;i<mes-1;i++)	
//			ndias+=ano2009[i];
//		
//	if(ano==2010)
//		for(int i=0;i<mes-1;i++)	
//			ndias+=ano2010[i];
//
//	if(ano==2011)
//		for(int i=0;i<mes-1;i++)	
//			ndias+=ano2011[i];
//		
//	if(ano==2012)
//		for(int i=0;i<mes-1;i++)	
//			ndias+=ano2012[i];
//
//	return ndias+dia;
//}
//
//int TimeStampHandler::nSeconds(int ano, int mes, int dia, int hora, int min, int seg)
//{
//      return seg+min*60+hora*3600+nDias(ano,mes,dia)*24*3600;
//}
//
//int TimeStampHandler::TimeStamp2Seconds(string time)
//{
//	//en date_time[0] fecha, date_time[1] hora
//	vector<string> date_time;
//	vector<string> date;
//	vector<string> time1;
//	
//	date_time = StringFunctions::Explode(time, ' ' );
//	
//	int ano,mes,dia,hora,min,seg;
//	
//	if(date_time.size() == 2)
//	{
//		date = StringFunctions::Explode(date_time[0],'-');
//		time1 = StringFunctions::Explode(date_time[1],':');	
//	
//		ano=atoi(date[0].c_str());
//		mes=atoi(date[1].c_str());
//		dia=atoi(date[2].c_str());
//
//		hora=atoi(time1[0].c_str());
//		min=atoi(time1[1].c_str());
//		seg=atoi(time1[2].c_str());
//	
//// 		return seg+min*60+hora*3600+dia*24*3600+mes*30*24*3600;
//		return TimeStampHandler::nSeconds(ano,mes,dia,hora,min,seg);
//	}
//	else if(date_time.size() == 1)
//	{
//		date = StringFunctions::Explode(date_time[0],'-');
//		time1 = StringFunctions::Explode(date_time[0],':');
//		
//		if(date.size() == 3)
//		{
//			ano=atoi(date[0].c_str());
//			mes=atoi(date[1].c_str());
//			dia=atoi(date[2].c_str());
//			
//			return dia*24*3600+mes*30*24*3600;
//		}
//		else if(time1.size() ==3)
//		{
//			hora=atoi(time1[0].c_str());
//			min=atoi(time1[1].c_str());
//			seg=atoi(time1[2].c_str());
//		
//			return seg+min*60+hora*3600;
//		}
//		else
//		{
//			return -1;
//		}
//	}
//
//	return -1;
//}