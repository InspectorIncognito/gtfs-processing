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
#pragma once


#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include <math.h>
#include "StringFunctions.h"

using namespace std;

/**
* Title: TimeStampHandler
* Description:	Clase que contiene metodos para el manejo de tiempos
* Copyright:  Pragxis (c) 2013
* Date: 18-06-2016
* @author Mauricio Zuñiga <mzuniga@pragxis.cl>
* @version 0.0
*/
class TimeStampHandler{
public:
	/**
	 * Constructor por defecto
	 **/
	TimeStampHandler();
	/**
	 * Destructor por defecto
	 **/
	~TimeStampHandler();

	map< int , vector<int> > dias;

	int TimeStamp2Seconds(string time);
	int Time2Seconds(string time);
	int nSeconds(int ano,int mes,int dia, int hora, int min, int seg);
	int nDias(int ano,int mes,int dia);
	string Seconds2TimeStamp(int seconds);
	vector<int> Seconds2TimeStampInVector(int seconds);

	static int TimeStamp2SecondsInDay(string time);
	string Seconds2TimeStampInDay(int seconds);

	int Seconds2SecondsInDay(int seconds);

	/**
	 * Metodo que estandariza un formato de fecha
	 */
	string DateStandar(string date);
	string DeleteMiliSeconds(string timestamp);
	int Time2Minutes(string time);
	int ExtractHour(string time);
	int ExtractDay(string time);
	int getDeltaSeconds(string t1,string t2);
	string SumaMinutos(string t1,string t2);
	string SumaMinutosEnDia(string t1,string t2);
	//string SumaSegundosEnDia(string t1,int t2);
	string ExtractTime(string time);
	string ExtractDate(string time);
	string ExtractTime2Puntos(string time);
	string ChangeFormatTime(string time);
	string Redondea10Min(string time);
	string RedondeaMediaHora(string time);
	int nDiasDelMes(int ano,int mes);

	/**
	* Metodo que estandariza el formato (anomesdiahoraminseg)
	*/
	string TimeStampStandar(string timestamp);

	/**
	* Metodo que estandariza el formato (dia-mes-ano hora-min-seg)
	*/
	string TimeStampStandar2(string timestamp);

};
