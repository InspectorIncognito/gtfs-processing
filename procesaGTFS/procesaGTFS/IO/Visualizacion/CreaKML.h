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

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include "ColorScale.h"
#include "../../Utilidades/ConvertCoordinate.h"


using namespace std;

/**
* Title: KML
* Description:	Modulo que genera un archivo de salida en formato KML para visualizar en Google Earth
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* @version 0.0
*/
class CreaKML{
public:
	CreaKML();
	CreaKML(const char *fileName,const char *nombre);

	void DefaultStyles();

	void InsertaExtrude(double x, double y,double ancho,double alto);

	void InsertaEstilos(const char *nombreEstilo,
			     const char *nombreNormalEstilo,
			     const char *nombreHighlightEstilo,
			     const char *icon1,
			     const char *iconEscala1,
			     const char *iconColor,
			     const char *colorLabelStyle1,
			     const char *escalaLabelStyle1,
			     const char *colorLineStyle1,
			     const char *anchoLineStyle1,
			     const char *icon2,
			     const char *iconEscala2,
			     const char *colorLabelStyle2,
			     const char *escalaLabelStyle2,
			     const char *colorLineStyle2,
			     const char *anchoLineStyle2);

	void InsertaEstilo(const char *nombreEstilo,
			    const char *icon,
			    const char *iconEscala,
 			    const char *iconColor,
			    const char *colorLabelStyle,
			    const char *escalaLabelStyle,
			    const char *colorLineStyle,
			    const char *anchoLineStyle);


	void InsertaPunto(const char *nombre,
			  const char *descripcion,
			  const char *latitud,
                          const char *longitud,
			  const char *style,
			  int visibility);


	void InsertaSegmento(const char *nombre,
			      const char *lat1,
                              const char *lon1,
			      const char *lat2,
                              const char *lon2,
			      const char *style,
			      int visibility);

	void CierraArchivo();
	void InsertaSetEstilosColor(int iniEscala,int finEscala,int nIniEstilos,int nFinEstilos);
	void InsertaEstiloColor(const char* color,const char* codigo);
	void InsertaEstilo(const char* color,const char* codigo, const char *porte);

	void AbreFolder(const char* nombre);
	void CierraFolder();

	
	 ~CreaKML();
	 ofstream archivo_kml;

};
