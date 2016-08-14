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
#include "ReporteGlobal.h"

ReporteGlobal::ReporteGlobal(FuenteDatos *fdd )
{
	this->fdd_ = fdd;
	this->capituloActual = 0;

	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	fechaEjecucion=string("");
	fechaEjecucion.append(StringFunctions::Int2String( 1900 + timeinfo->tm_year)+"-");
	
	if(timeinfo->tm_mon+1 > 0 && timeinfo->tm_mon+1 < 10)
		fechaEjecucion.append("0");
	fechaEjecucion.append(StringFunctions::Int2String(1 + timeinfo->tm_mon)+"-");

	if(timeinfo->tm_mday > 0 && timeinfo->tm_mday < 10)
		fechaEjecucion.append("0");
	fechaEjecucion.append(StringFunctions::Int2String(timeinfo->tm_mday)+" ");

	if(timeinfo->tm_hour > 0 && timeinfo->tm_hour < 10)
		fechaEjecucion.append("0");
	fechaEjecucion.append(StringFunctions::Int2String(timeinfo->tm_hour)+":");

	if(timeinfo->tm_min > 0 && timeinfo->tm_min < 10)
		fechaEjecucion.append("0");
	fechaEjecucion.append(StringFunctions::Int2String(timeinfo->tm_min)+":");

	if(timeinfo->tm_sec > 0 && timeinfo->tm_sec < 10)
		fechaEjecucion.append("0");
	fechaEjecucion.append(StringFunctions::Int2String(timeinfo->tm_sec));

	///Se abre el archivo de reporte
	AbreArchivo();

	///Impresion de la portada del reporte
	ImprimePortada();
}

ReporteGlobal::~ReporteGlobal(void)
{
  
}

void ReporteGlobal::ImprimePortadaLATEX()
{
	*archivoLATEX << "\\documentclass[10pt,a4paper]{article}" << endl;
	*archivoLATEX << "\\usepackage[latin1]{inputenc}" << endl;
	*archivoLATEX << "\\usepackage[spanish]{babel}" << endl;
	*archivoLATEX << "\\usepackage{graphicx}" << endl;
	*archivoLATEX << "\\usepackage{fancyhdr}" << endl;
	*archivoLATEX << "\\usepackage{rotating}" << endl;
	*archivoLATEX << "\\usepackage{color}" << endl;
	

	*archivoLATEX << "\\pagestyle{fancy}" << endl;
	*archivoLATEX << "\\renewcommand{\\headrulewidth}{0.1pt}" << endl;
	*archivoLATEX << "\\renewcommand{\\footrulewidth}{0.4pt}" << endl;
	*archivoLATEX << "\\rfoot{Pragxis SpA}" << endl;
	*archivoLATEX << "\\rhead{" << "" << "}" << endl;
	*archivoLATEX << "\\lhead{Reporte PVM}" << endl;

	*archivoLATEX << "\\begin{document}" << endl;

	*archivoLATEX << "\\  \\\\" << endl;
	*archivoLATEX << "Reporte procesamiento validaciones para Transantiago \\\\" << endl;
	*archivoLATEX << "Fecha dia procesado : " << "" << " \\\\" << endl;
	*archivoLATEX << "Fecha ejecucion : " << fechaEjecucion << " \\\\" << endl;
	*archivoLATEX << "Software : PVMTS \\\\" << endl;
	*archivoLATEX << "Version : 3.0.2 \\\\" << endl;
	*archivoLATEX << "Copyright:  Pragxis SpA (c) 2013 \\\\" << endl;
	*archivoLATEX << "Terminos de licencia de uso : Este programa está protegido por las leyes de los derechos de autor. La reproducción y/o distribución de este programa o de cualquier parte del mismo, puede dar lugar a responsabilidades civiles y criminales, que serán perseguidas. \\\\" << endl;
	*archivoLATEX << "\\bigskip" << endl;
}

void ReporteGlobal::ImprimePortada()
{
	//if(fdd_->parametros->tipoReporte==ASCII)
		ImprimePortadaASCII();
	//else if(fdd_->parametros->tipoReporte==LATEX)
		ImprimePortadaLATEX();

}

void ReporteGlobal::ImprimePortadaASCII()
{
	*archivoASCII << "------------------------------------------------------------------------------------------------" << endl;
	*archivoASCII << "|Reporte de procesamiento de validaciones                                                       |" << endl;
	*archivoASCII << "|Fecha : " << "" << "                                                                             |" << endl;
	*archivoASCII << "|Fecha ejecucion : " << fechaEjecucion << "                                                                          |" << endl;
	*archivoASCII << "|Software : pvmts                                                                                 |" << endl;
	*archivoASCII << "|Version : v3.0.2                                                                               |" << endl;
	*archivoASCII << "|Copyright:  Pragxis SpA (c) 2013                                                               |" << endl;
	*archivoASCII << "|Terminos de licencia de uso : Este programa está protegido por las leyes de los derechos de    |" << endl;
	*archivoASCII << "|                              autor. La reproducción y/o distribución de este programa o de    |" << endl;
	*archivoASCII << "|                              cualquier parte del mismo, puede dar lugar a responsabilidades   |" << endl;
	*archivoASCII << "|                              civiles y criminales, que serán perseguidas.                     |" << endl;
	*archivoASCII << "------------------------------------------------------------------------------------------------" << endl;
	*archivoASCII  << endl;
}

void ReporteGlobal::InsertaTexto(string textoASCII,string textoLATEX)
{
	*archivoASCII << textoASCII << endl;
	*archivoLATEX << textoLATEX << endl;
}

void ReporteGlobal::AbreArchivo()
{
	this->archivoASCII = new ofstream();
	this->archivoLATEX = new ofstream();
	
	///Impresion de los reportes
	string nombreArchivoASCII;
	string nombreArchivoLATEX;
	if(T3_PLATFORM==1)	//Windows
	{
		nombreArchivoASCII = string(fdd_->parametros->carpetaReportes + "\\" + "reporte_pvm.txt");
		nombreArchivoLATEX = string(fdd_->parametros->carpetaReportes + "\\" + "reporte_pvm.tex");
	}
	else if(T3_PLATFORM==2)	//LINUX
	{
		nombreArchivoASCII = string(fdd_->parametros->carpetaReportes + "/" + "reporte_pvm.txt");
		nombreArchivoLATEX = string(fdd_->parametros->carpetaReportes + "/" + "reporte_pvm.tex");
	}
	
	///Abro el archivo
	cout << fdd_->parametros->carpetaReportes << endl;
	cout << nombreArchivoASCII << endl;
	archivoASCII->open(nombreArchivoASCII.c_str());
	archivoLATEX->open(nombreArchivoLATEX.c_str());
}

void ReporteGlobal::CierraArchivo()
{
	archivoASCII->close();

	*archivoLATEX << "\\end{document}" << endl;
	archivoLATEX->close();

	system(string("pdflatex  " + fdd_->parametros->carpetaReportes + "\\reporte_pvm.tex").c_str());

	
}

int ReporteGlobal::Capitulo()
{
	capituloActual++;
	return capituloActual;
}