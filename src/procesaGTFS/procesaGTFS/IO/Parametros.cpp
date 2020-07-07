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

#include "Parametros.h"

Parametros::Parametros()
{
}

Parametros::~Parametros()
{
}

Parametros::Parametros(const char *file)
{
	///Archivo de parametros
	ifstream ap;	
	ap.open(file);
	vector<string> columnas;
	vector<string> columnas1;

	///Las 3 primeras lineas no tienen valores
	columnas = StringFunctions::ExplodeF('*',&ap);
	columnas = StringFunctions::ExplodeF('*',&ap);
	columnas = StringFunctions::ExplodeF('*',&ap);

	
	columnas = StringFunctions::ExplodeF('\t', &ap);	this->gtfs_id = columnas[0];

	///Lectura de Parametros
	columnas = StringFunctions::ExplodeF('\t', &ap);	this->nombreCarpetaGTFS = columnas[0];

	columnas = StringFunctions::ExplodeF('\t', &ap);	this->carpetaKmls = columnas[0];
	columnas = StringFunctions::ExplodeF('\t', &ap);	this->carpetaReportes = columnas[0];
	columnas = StringFunctions::ExplodeF('\t', &ap);	this->carpetaOutput = columnas[0];

	columnas = StringFunctions::ExplodeF('\t', &ap);	this->resourceIdPath = columnas[0];

	
    columnas = StringFunctions::ExplodeF('\t', &ap);	this->withBip = atoi(columnas[0].c_str())==1;
    columnas = StringFunctions::ExplodeF('\t', &ap);	this->withMetro = atoi(columnas[0].c_str())==1;
	
	columnas = StringFunctions::ExplodeF('\t', &ap);	this->distanciaTramado = atof(columnas[0].c_str());
	columnas = StringFunctions::ExplodeF('\t', &ap);	this->version = columnas[0];
	
	
	///Lectura de Colores
	//columnas = StringFunctions::ExplodeF('\t', &ap);	this->nColores = atoi(columnas[0].c_str());

	//for (int i = 0; i < nColores ; i++)
	//{
	//	columnas = StringFunctions::ExplodeF('\t', &ap); columnas1 = StringFunctions::Explode(columnas[0], ' ');
	//	mapeoColores[columnas1.at(0)] = columnas1.at(1);
	//}

	///debug
	//for (map<string, string>::iterator it = mapeoColores.begin(); it != mapeoColores.end(); it++)
	//{
	//	cout << (*it).first << "|" << (*it).second << endl;
	//}
	//exit(1);

	///Imprimo el diccionario para los codigos de servicio-sentido
	///Windows
	if(T3_PLATFORM==1)
		slash = "\\";
	///LINUX
	else if(T3_PLATFORM==2)
		slash = "/";
}

void Parametros::Imprime()
{

}
