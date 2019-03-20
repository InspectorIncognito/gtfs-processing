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

#define MAXLINE 1024

#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>

using namespace std;

/**
* Title: StringFunctions
* Description:	Clase que contiene metodos para manejo de strings
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga <mzuniga@pragxis.cl>
* @version 0.0
*/
class StringFunctions{
public:
	/**
	 * Constructor por defecto
	 **/
	StringFunctions();
	/**
	 * Destructor por defecto
	 **/
	~StringFunctions();

	/**
	 * Metodo que transforma un double en un string
	 * @param d numero de tipo double a transformar
	 * @param p nivel de precision del numero
	 * @return el numero pero ahora como string
	 */
	static string Double2String(double d,int p);
	
	/**
	 * Metodo que transforma un int en un string
	 * @param d numero de tipo double a transformar
	 * @return el numero pero ahora como string
	 */	
	static string Int2String(int d);

	/**
	 * Metodo que transforma un int en un string
	 * @param d numero de tipo double a transformar
	 * @return el numero pero ahora como string
	 */	
	static string Long2String(long d);

	/**
	* Metodo que transforma un char en string
	*/
	static string Char2String(char c);

	/**
	* Metodo que reemplaza todas las ocurrencias de un character dentro de un 
	* string por otro
	* @param str string donde se reemplazaran los caracteres
	* @param ch1 caracter a reemplazar
	* @param ch2 caracter que reemplazara
	* @return un string con los caracteres cambiados
	*/
	static string ReplaceChar(string str, char ch1, char ch2);

	/**
	* Metodo que escapa los caracteres especiales en Latex de un string
	*/
	static string toLatex(string str);

	/** 
	*	Metodo para extraer las columnas de un string
	*	@param str string que contiene la linea a la cual se le separaran los elementos de cada columna
	*	@param separator caracter que separa las columnas del registro
	*	@return vector que contiene en cada elemento el contenido en una columna de datos
	**/
	static vector<string> Explode(string str, char separator );
	
	/**
	*	Metodo para extraer las columnas de un archivo
	*	@param separator caracter que separa las columnas del registro
	*	@param f referencia al archivo de lectura
	*	@return vector que contiene en cada elemento el contenido en una columna de datos
	**/
	static vector<string> ExplodeF(char separator , ifstream *f);

	/**
	*	Metodo para extraer las columnas de un archivo
	*	@param separator caracter que separa las columnas del registro
	*	@param f referencia al archivo de lectura
	*	@return vector que contiene en cada elemento el contenido en una columna de datos
	**/
	static inline void ExplodeFN(char separator , ifstream *f, vector<string> *linea);
    
    static string toCamelCase(string in);
    static string EliminaCadenasBlancos(string in);

};

inline void StringFunctions::ExplodeFN(char separator , ifstream *f, vector<string> *linea)
{
	char line[MAXLINE];
	f->getline(line,MAXLINE);
	int i=0;

	///Solucion 3 0.35 + inline ==> 0.29
	linea->at(0).clear();
	for(int ic= 0;ic < MAXLINE && line[ic]!='\0';ic++)
	{
		if(line[ic]!=separator)
		  linea->at(i)+=line[ic];
		else
 		  linea->at(++i).clear();
	}

}

//inline vector<string> StringFunctions::Explode(string str, char separator )
//{
//	vector<string>  result;
//	string tmp=string("");
//	for(int ic= 0; str[ic]!='\0' ;ic++)
//	{
//		if(str[ic]!=separator)
//		{
//			tmp+=str[ic];
//		}
//		else
//		{
//			result.push_back( tmp );
//			tmp.clear();
//		}
//	}
//	result.push_back( tmp );
//
//	return result;
//}

inline vector<string> StringFunctions::Explode(string str, char separator )
{
	vector<string>  result;
	size_t pos1 = 0;
	size_t pos2 = 0;
	while ( pos2 != str.npos )
	{
		pos2 = str.find(separator, pos1);
		if ( pos2 != str.npos )
		{
			if ( pos2 > pos1 )
				result.push_back( str.substr(pos1, pos2-pos1) );
			pos1 = pos2+1;
		}
	}
	result.push_back( str.substr(pos1, str.size()-pos1) );

	return result;
}
