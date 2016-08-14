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
#include "ReporteFuenteDatos.h"


ReporteFuenteDatos::ReporteFuenteDatos(string path_) : Reporte(path_)
{
	///Creacion de objeto para manejo de tiempo
	tsh = TimeStampHandler();

	nRegistrosDiccionario=0;
	nSSnoEncontradosEnDiccionario=0;

	nSitios22=0;
	nExp22=0;
	nRegistros22=0;
	nSitiosEn196NoEn22=0;
	nExp22Menos2Segundos=0;

	nRegistros196=0;
	nSitios196=0;
	nExp196=0;
	nRegistrosNulos196=0;

	nSitiosEnPosNoEnExps=0;
	nSitiosEnPos=0;
	nRegistrosGPS=0;
	nRegistrosGPSRepetidos=0;
	nRegistrosGPSPatenteNula=0;

	nRutas=0;
	nRutasNodosRepetidos=0;
	nRutasNodosFueraDeRuta=0;
	nRutasNodosFueraDeSantiago=0;

	nSecParadas=0;

	nRedParadas=0;
	nRedParadasInactivas=0;
	nRedParadasRepetidas=0;

	nEstacionesMetro=0;
	nEstacionesMetroRepetidas=0;

	nEvasionRegistros=0;
	nEvasionRegistrosRepetidos=0;

	nValidadoresZonasPagasTabla=0;
	nValidadoresRepetidosZonasPagasTabla=0;
	nValidadoresSinZonaPaga=0;

	tDiccionario=0.0;
	tR22=0.0;
	tR196=0.0;
	tPosicionamiento=0.0;
	tRutas=0.0;
	tParadas=0.0;
	tZonasPagas=0.0;
	tEvasion=0.0;
	tEstacionesMetro=0.0;
	tGrafoMetro=0.0;
	tValidadores=0.0;

	nZonasPagas=0;
	nValidadoresZonasPagas=0;

	this->hist22 = Histogram(0,200,10,this->path);
	this->histTGps = Histogram(0,600,5,this->path);
	this->histGpsPorHora = Histogram(0,86400,3600,this->path);

	nPatentesEnCapacidadesNoEnResto=0;
	nPatentesEnRestoNoEnCapacidades=0;

}

ReporteFuenteDatos::~ReporteFuenteDatos() 
{
}

string ReporteFuenteDatos::Imprime(int iCapitulo, int TipoReporte)
{
	this->tipoReporte_ = TipoReporte;

	if(TipoReporte==ASCII)
		return ImprimeASCII(iCapitulo);
	else if(TipoReporte==LATEX)
		return ImprimeLATEX();
	else
		return string("No se reconocio tipo de reporte (ASCII,LATEX...)\n");
}

string ReporteFuenteDatos::ImprimeLATEX()
{
	string output;
	
	output.append("\\section{Seccion Datos de Entrada (Lectura) \\medskip } \n");

	output.append(ImprimeDiccionarioLATEX());

	output.append(ImprimeReporte22LATEX());

	//output.append(ImprimeReporte196LATEX());

	output.append(ImprimePosicionamientoLATEX());

	output.append(ImprimeRutasLATEX());

	output.append(ImprimeParadasLATEX());

	output.append(ImprimeRedParadasLATEX());

	output.append(ImprimeZPsLATEX());

	output.append(ImprimeEvasionLATEX());

	output.append(ImprimeMetroLATEX());

	output.append(ImprimeValidadoresLATEX());

	output.append(ImprimeNoExistenciaLATEX());

	output.append(ImprimeZonificacionesLATEX());

	return output;
}

string ReporteFuenteDatos::ImprimeDiccionarioLATEX()
{
	string output("");
	output.append("\\subsection{Diccionario \\medskip} \n" );
	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | c | }\n");
	output.append("\\hline                 \n");
	output.append("Descripcion & Valor  \\\\ \n");
	output.append("\\hline      \n");
	output.append("\\# registros & " + StringFunctions::Int2String(nRegistrosDiccionario) + "\\\\ \n");
	output.append("\\# codigos de SS en reporte 2.2 no encontrados en diccionario & " + StringFunctions::Int2String(nSSnoEncontradosEnDiccionario) + " \\\\ \n");
	output.append("\\hline  \n");
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");

	output.append("Lista de servicios no encontrados en diccionario de servicios : \\medskip \n");

	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | }\n");
	output.append("\\hline                 \n");
	output.append("Servicio (Reporte 2.2)  \\\\ \n");
	output.append("\\hline      \n");
	int i=0;
	for(map< string , int >::iterator it = ssNoEncontradosEnDiccionario.begin();it!=ssNoEncontradosEnDiccionario.end();it++,++i)
	{
		if(i < 10)
		{
			output.append((*it).first + "  \\\\ \n");
			output.append("\\hline  \n");
		}
	}
	if(i >= 10)
	{
		output.append(" Continua en reporte.txt \\\\ \n");
		output.append("\\hline  \n");
	}
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");

	return output;
}

string ReporteFuenteDatos::ImprimeReporte22LATEX()
{
	string output("");
	
	output.append("\\subsection{Reporte Simplificado 1.96 \\medskip}\n");

	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | c | }\n");
	output.append("\\hline             \n");    
	output.append("  Descripcion & Valor  \\\\ \n");
	output.append("\\hline      \n");
	output.append("\\# de registros & " + StringFunctions::Int2String(nRegistros22)+"  \\\\ \n");
	output.append("\\# de expediciones & " + StringFunctions::Int2String(nExp22)+" \\\\ \n");
	output.append("\\# de patentes & " + StringFunctions::Int2String(nSitios22)+" \\\\ \n");
	output.append("\\# de expediciones nulas & " + StringFunctions::Int2String(nExp22Menos2Segundos)+" \\\\ \n");
	output.append("\\hline  \n");
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");
	output.append(this->hist22.Imprime("Estadisticas tiempos de expedicion",LATEX));



	return output;
}

string ReporteFuenteDatos::ImprimeReporte196LATEX()
{
	string output("");

	output.append("\\subsection{Reporte 1.96 \\medskip}\n");

	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | c | }\n");
	output.append("\\hline             \n");    
	output.append("  Descripcion & Valor  \\\\ \n");
	output.append("\\hline      \n");
	output.append("\\# de registros & "+StringFunctions::Int2String(nRegistros196)+"  \\\\ \n");
	output.append("\\# de registros nulos & "+StringFunctions::Int2String(nRegistrosNulos196)+"  \\\\ \n");
	output.append("\\# de expediciones & "+StringFunctions::Int2String(nExp196)+" \\\\ \n");
	output.append("\\# de patentes & "+StringFunctions::Int2String(nSitios196)+" \\\\ \n");
	output.append("\\# de patentes no encontradas en 2.2 & "+StringFunctions::Int2String(nSitiosEn196NoEn22)+" \\\\ \n");
	output.append("\\hline  \n");
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");
	

	return output;
	
}

string ReporteFuenteDatos::ImprimePosicionamientoLATEX()
{
	string output("");
	output.append("\\subsection{Posicionamiento \\medskip}\n");

	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | r | r |}\n");
	output.append("\\hline             \n");    
	output.append("  Descripcion & Valor & \\% \\\\ \n");
	output.append("\\hline      \n");
	output.append("\\# de patentes & "+StringFunctions::Int2String(nSitiosEnPos)+" & \\\\ \n");
	output.append("\\# de patentes no encontradas en expediciones & "+StringFunctions::Int2String(nSitiosEnPosNoEnExps)+" & \\\\ \n");
	output.append("\\# de registros GPS & "+StringFunctions::Int2String(nRegistrosGPS)+" & \\\\ \n");
	output.append("\\# de registros GPS sin ppu & "+StringFunctions::Int2String(nRegistrosGPSPatenteNula)+" & \\\\ \n");
	output.append("\\# de registros GPS repetidos & "+StringFunctions::Int2String(nRegistrosGPSRepetidos)+" & " + StringFunctions::Double2String(nRegistrosGPSRepetidos*100.0/(nRegistrosGPS),5) +" \\\\ \n");
	output.append("\\# de registros GPS utiles & "+StringFunctions::Int2String(nRegistrosGPS-nRegistrosGPSRepetidos)+" & " + StringFunctions::Double2String((nRegistrosGPS-nRegistrosGPSRepetidos)*100.0/(nRegistrosGPS),5) +"\\\\ \n");
	output.append("\\hline  \n");
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");

	output.append(this->histTGps.Imprime("Estadisticas tiempos entre GPS",LATEX));
	output.append(this->histGpsPorHora.Imprime("Histograma GPS por hora",LATEX));
	
	return output;
}

string ReporteFuenteDatos::ImprimeRutasLATEX()
{
	string output;
	output.append("\\subsection{Rutas \\medskip}\n");

	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | r | r |}\n");
	output.append("\\hline             \n");    
	output.append("  Descripcion & Valor & \\% \\\\ \n");
	output.append("\\hline      \n");
	output.append("\\# de rutas & "+StringFunctions::Int2String(nRutas)+" & \\\\ \n");
	output.append("Ruta mas larga & "+rutaMasLarga.first+" & "+StringFunctions::Double2String(float(rutaMasLarga.second)/1000.0,5)+"(Kms) \\\\ \n");
	output.append("Ruta mas corta & "+rutaMasCorta.first+" & "+StringFunctions::Double2String(float(rutaMasCorta.second)/1000.0,5)+"(Kms)\\\\ \n");
	output.append("\\hline  \n");
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");

	output.append("Rutas con posibles problemas  : \\medskip \n");	

	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | r | r | r |}\n");
	output.append("\\hline             \n");    
	output.append(" Ruta & \\#NodosRepetidos & \\#NodosFueraDeRuta & \\#NodosFueraDeSantiago \\\\ \n");
	output.append("\\hline      \n");
	for(map< string , ErroresRuta >::iterator it = rutasErradas.begin();it!=rutasErradas.end();it++)
	{
			output.append((*it).first + " & ");
			output.append(StringFunctions::Int2String((*it).second.nNodosRepetidos) + " & ");
			output.append(StringFunctions::Int2String((*it).second.nNodosFueraDeRuta) + " & ");
			output.append(StringFunctions::Int2String((*it).second.nNodosFueraDeSantiago));
			output.append(" \\\\ \n");
	}
	output.append("\\hline  \n");
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");

	return output;
}

string ReporteFuenteDatos::ImprimeParadasLATEX()
{
	string output("");
	output.append("\\subsection{Secuencia de paradas \\medskip}\n");

	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | r | r |}\n");
	output.append("\\hline             \n");    
	output.append("  Descripcion & Valor & Cantidad \\\\ \n");
	output.append("\\hline      \n");
	output.append("\\# de secuencias & "+StringFunctions::Int2String(this->nSecParadas)+" & \\\\ \n");
	output.append("Secuencia mas larga & "+nMaxParadas.first+" & "+StringFunctions::Int2String(nMaxParadas.second)+" \\\\ \n");
	output.append("Secuencia corta & "+nMinParadas.first+" & "+StringFunctions::Int2String(nMinParadas.second)+" \\\\ \n");
	output.append("\\hline  \n");
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");

	output.append("Secuencia sin ruta encontrada  : ");
	
	if(secuenciasSinRutas.empty())
	{
		output.append("(NO EXISTEN) \\\\ \n");
	}
	else
	{
		output.append("\\medskip \n");
		output.append("\\begin{center}\n");
		output.append("\\begin{tabular}{ | l | }\n");
		output.append("\\hline             \n");    
		output.append("  Servicio  \\\\ \n");
		output.append("\\hline      \n");
		int i=0;
		for(map<string,int>::iterator it=secuenciasSinRutas.begin();it!=secuenciasSinRutas.end();it++,++i)
		{
			if(i < 10)
			{
				output.append((*it).first + "\\\\ \n");
				output.append("\\hline  \n");
			}
		}
		if(i>=10)
		{
				output.append("Continua en reporte.txt \\\\ \n");
				output.append("\\hline  \n");
		}
		output.append("\\hline  \n");
		output.append("\\end{tabular}\n");
		output.append("\\end{center}\n");
	}
	output.append("Paradas sin codigo conocido  : ");

	if(paradasSinFormato.empty())
	{
			output.append("(NO EXISTEN) \\\\ \n");
	}
	else
	{
		output.append("\\medskip \n");
		output.append("\\begin{center}\n");
		output.append("\\begin{tabular}{ | l | r | }\n");
		output.append("\\hline             \n");    
		output.append("  Servicio & Cantidad \\\\ \n");
		output.append("\\hline      \n");
		for(map<string,int>::iterator it=paradasSinFormato.begin();it!=paradasSinFormato.end();it++)
		{
			output.append((*it).first + " & " + StringFunctions::Int2String((*it).second) + "\\\\ \n");
		}
		output.append("\\hline  \n");
		output.append("\\end{tabular}\n");
		output.append("\\end{center}\n");
	}

	output.append("Secuencias con posibles problemas  : ");	
	if(paradasErradas.empty())
	{
			output.append("(NO EXISTEN) \\\\ \n");
	}
	else
	{
		output.append("\\medskip \n");
		output.append("\\begin{center}\n");
		output.append("\\begin{tabular}{ | l | r | r | r | }\n");
		output.append("\\hline             \n");    
		output.append("Secuencia & Parada & FueraDeRuta(mts) & FueraDeSantiago \\\\ \n");
		output.append("\\hline      \n");
		int i=0;
		for(map<string, vector < ErroresParadas > >::iterator it = paradasErradas.begin();it!=paradasErradas.end();it++)
		{
			for(vector< ErroresParadas >::iterator it2=(*it).second.begin();it2!=(*it).second.end();it2++,++i)
			{
				if(i < 10)
				{
					output.append((*it).first + " & ");
					output.append((*it2).codigoParada + " & ");
					output.append(StringFunctions::Int2String((*it2).fueraDeRuta) + " & ");

					if((*it2).fueraDeSantiago)
						output.append("X ");
					else
						output.append("- ");

					output.append(" \\\\ \n");
				}
			}
		}
		if(i>=10)
		{
				output.append("Continua en reporte.txt & & & \\\\ \n");
				output.append("\\hline  \n");
		}
		output.append("\\hline  \n");
		output.append("\\end{tabular}\n");
		output.append("\\end{center}\n");
	}

	output.append("Servicios y paradas con incosistencias en secuencia de avance  : ");
	if(inconsistenciasSecuencia.empty())
	{
		output.append("(NO EXISTEN) \\\\ \n");
	}
	else
	{
		output.append(StringFunctions::Int2String(int(inconsistenciasSecuencia.size())) + " \\medskip \n");
		output.append("\\begin{center}\n");
		output.append("\\begin{tabular}{ | l | r | }\n");
		output.append("\\hline             \n");    
		output.append("Servicio & Parada \\\\ \n");
		output.append("\\hline      \n");
		int i=0;
		for(map<string, string >::iterator it = inconsistenciasSecuencia.begin();it!=inconsistenciasSecuencia.end();it++,++i)
		{
			if(i<10)
			{
				///Chequeo que no es un paradero que tiene problemas en el formato del codigo
				if(int(StringFunctions::Explode((*it).second,'-').size())!=5)
					continue;

				output.append((*it).first + " & ");
				output.append((*it).second + " \\\\ \n");
			}
		}
		if(i>=10)
		{
				output.append("Continua en reporte.txt & \\\\ \n");
				output.append("\\hline  \n");
		}
		output.append("\\hline  \n");
		output.append("\\end{tabular}\n");
		output.append("\\end{center}\n");
	}

	return output;
}

string ReporteFuenteDatos::ImprimeZPsLATEX()
{
	string output("");
	output.append("\\subsection{Zonas Pagas \\medskip}\n");

	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | r |}\n");
	output.append("\\hline             \n");    
	output.append("  Descripcion & Valor \\\\ \n");
	output.append("\\hline      \n");
	output.append("\\# Zonas Pagas & "+StringFunctions::Int2String(nZonasPagas)+" \\\\ \n");
	output.append("\\# Validadores & "+StringFunctions::Int2String(nValidadoresZonasPagas)+" \\\\ \n");
	output.append("\\hline  \n");
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");

	output.append("\\# Paraderos de zonas pagas no encontrados  : ");
	if(paraderosNoEncontradosDeZonasPagas.empty())
	{
		output.append("(NO EXISTEN) \\medskip \n");
	}
	else
	{
		output.append(StringFunctions::Int2String(paraderosNoEncontradosDeZonasPagas.size()) + " \\medskip \n");
		output.append("\\begin{center}\n");
		output.append("\\begin{tabular}{ | l | r |}\n");
		output.append("\\hline             \n");    
		output.append("  Parada & ZP \\\\ \n");
		output.append("\\hline      \n");
		for(map<string, string >::iterator it = paraderosNoEncontradosDeZonasPagas.begin();it!=paraderosNoEncontradosDeZonasPagas.end();it++)
		{
			output.append((*it).first + " & " + (*it).second + " \\\\ \n");
		}
		output.append("\\hline  \n");
		output.append("\\end{tabular}\n");
		output.append("\\end{center}\n");
	}


	output.append("\\# Paraderos de distribuciones no encontrados en zonas pagas : ");
	if(paraderosNoEncontradosDeDistribuciones.empty())
	{
		output.append("(NO EXISTEN) \\medskip \n");
	}
	else
	{
		output.append(StringFunctions::Int2String(paraderosNoEncontradosDeDistribuciones.size()) + " \\medskip \n");
		output.append("\\begin{center}\n");
		output.append("\\begin{tabular}{ | l | r |}\n");
		output.append("\\hline             \n");    
		output.append("  Parada & ZP \\\\ \n");
		output.append("\\hline      \n");
		for(map<string, int >::iterator it = paraderosNoEncontradosDeDistribuciones.begin();it!=paraderosNoEncontradosDeDistribuciones.end();it++)
		{
			output.append((*it).first + " & " + StringFunctions::Int2String((*it).second) + " \\\\ \n");
		}
		output.append("\\hline  \n");
		output.append("\\end{tabular}\n");
		output.append("\\end{center}\n");
	}

	output.append("\\# Validadores-MediaHora con rara distribucion : ");
	if(paraderosNoEncontradosDeDistribuciones.empty())
	{
		output.append("(NO EXISTEN) \\medskip \n");
	}
	else
	{
		output.append(StringFunctions::Int2String(distribucionesMalas.size()) + " \\medskip \n");
		output.append("\\begin{center}\n");
		output.append("\\begin{tabular}{ | l | c | r | r |}\n");
		output.append("\\hline             \n");    
		output.append("  Validador & mediahora & distribucion \\\\ \n");
		output.append("\\hline      \n");
		for(map< string , map<int, map<string,double> > >::iterator ival = distribucionesMalas.begin();ival!=distribucionesMalas.end();ival++)
		{
			for(map<int, map<string,double> >::iterator imh = (*ival).second.begin();imh!=(*ival).second.end();imh++)
			{
				for(map<string,double>::iterator iserv = (*imh).second.begin();iserv!=(*imh).second.end();iserv++)
				{
					output.append((*ival).first + " & " + tsh.Seconds2TimeStampInDay((*imh).first)+ " & " + (*iserv).first + " & " + StringFunctions::Double2String((*iserv).second,5) + " \\\\ \n");
				}
			}
		}
		output.append("\\hline  \n");
		output.append("\\end{tabular}\n");
		output.append("\\end{center}\n");
	}

	output.append("\\# Patentes con capacidad que no se usan : " + StringFunctions::Int2String(nPatentesEnCapacidadesNoEnResto) + " \\medskip \n");

	return output;
}

string ReporteFuenteDatos::ImprimeNoExistenciaLATEX()
{
	string output("");
	output.append("\\subsection{Resumen problemas por servicio \\medskip}\n");

	if(noExistenciaDatos.empty())
	{
		output.append("(NO EXISTEN) \\\\ \n");
	}
	else
	{
		output.append("\\# Cantidad de casos : "+ StringFunctions::Int2String(noExistenciaDatos.size()) + "\\medskip \n");
		output.append("\\begin{center}\n");
		output.append("\\begin{tabular}{ | l | r | r | r | r |}\n");
		output.append("\\hline             \n");    
		output.append("Servicio & \\begin{sideways} Ruta Sin diccionario \\end{sideways} & \\begin{sideways} Secuencia sin diccionario \\end{sideways} & \\begin{sideways} No existe ruta \\end{sideways} & \\begin{sideways} No existe Secuencia \\end{sideways} \\\\ \n");
		output.append("\\hline      \n");
		int i=0;
		for(map< string , ErroresNoExistenciaDatos >::iterator ierr = noExistenciaDatos.begin();ierr!=noExistenciaDatos.end();ierr++,++i)
		{
			if(i<10)
			{
				output.append((*ierr).first + " & ");
				if((*ierr).second.noDicRuta)	output.append("X & ");
				else							output.append("- & ");

				if((*ierr).second.noDicSec)	output.append("X & ");
				else							output.append("- & ");

				if((*ierr).second.noRuta)	output.append("X & ");
				else							output.append("- & ");

				if((*ierr).second.noSec)	output.append("X");
				else							output.append("-");

				output.append("\\\\ \n");
			}
		}
		if(i>=10)
		{
			output.append(" Continua en reporte.txt & & & & \\\\ \n");
			output.append("\\hline  \n");
		}
		output.append("\\hline  \n");
		output.append("\\end{tabular}\n");
		output.append("\\end{center}\n");
	}

	return output;
}

string ReporteFuenteDatos::ImprimeEvasionLATEX()
{
	string output("");
	output.append("\\subsection{Evasion \\medskip} \n" );
	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | c | }\n");
	output.append("\\hline                 \n");
	output.append("Descripcion & Valor  \\\\ \n");
	output.append("\\hline      \n");
	output.append("\\# registros & " + StringFunctions::Int2String(nEvasionRegistros) + "\\\\ \n");
	output.append("\\# registros repetidos & " + StringFunctions::Int2String(nEvasionRegistrosRepetidos) + " \\\\ \n");
	output.append("\\hline  \n");
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");

	output.append("Lista de paraderos-servicios repetidos : \\medskip \n");

	if(evasionRepetida.empty())
	{
		output.append("(NO EXISTEN) \\medskip \n");
	}
	else
	{
		output.append(StringFunctions::Int2String(evasionRepetida.size()) + " \\medskip \n");
		output.append("\\begin{center}\n");
		output.append("\\begin{tabular}{ | l | r |}\n");
		output.append("\\hline             \n");    
		output.append(" Paradero-servicio & cantidad \\\\ \n");
		output.append("\\hline      \n");
		int i=0;
		for(map<string, int >::iterator it = evasionRepetida.begin();it!=evasionRepetida.end();it++,++i)
		{
			if(i<10)
			{
				output.append((*it).first + " & " + StringFunctions::Int2String((*it).second) + " \\\\ \n");
				output.append("\\hline  \n");
			}
		}
		if(i>=10)
		{
			output.append("Continua en reporte.txt & \\\\ \n");
			output.append("\\hline  \n");
		}
		output.append("\\hline  \n");
		output.append("\\end{tabular}\n");
		output.append("\\end{center}\n");
	}

	output.append("Lista de zonas repetidas : \\medskip \n");

	if(evasionRepetidaZonas.empty())
	{
		output.append("(NO EXISTEN) \\medskip \n");
	}
	else
	{
		output.append(StringFunctions::Int2String(evasionRepetidaZonas.size()) + " \\medskip \n");
		output.append("\\begin{center}\n");
		output.append("\\begin{tabular}{ | l | r |}\n");
		output.append("\\hline             \n");    
		output.append(" Paradero-servicio & cantidad \\\\ \n");
		output.append("\\hline      \n");
		int i=0;
		for(map<string, int >::iterator it = evasionRepetidaZonas.begin();it!=evasionRepetidaZonas.end();it++,++i)
		{
			if(i<10)
			{
				output.append((*it).first + " & " + StringFunctions::Int2String((*it).second) + " \\\\ \n");
				output.append("\\hline  \n");
			}
		}
		if(i>=10)
		{
			output.append("Continua en reporte.txt & \\\\ \n");
			output.append("\\hline  \n");
		}
		output.append("\\hline  \n");
		output.append("\\end{tabular}\n");
		output.append("\\end{center}\n");
	}


	return output;
}

string ReporteFuenteDatos::ImprimeMetroLATEX()
{
	string output("");
	output.append("\\subsection{Metro \\medskip} \n" );
	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | c | }\n");
	output.append("\\hline                 \n");
	output.append("Descripcion & Valor  \\\\ \n");
	output.append("\\hline      \n");
	output.append("\\# estaciones de metro & " + StringFunctions::Int2String(nEstacionesMetro) + "\\\\ \n");
	output.append("\\# estaciones repetidas & " + StringFunctions::Int2String(nEstacionesMetroRepetidas) + " \\\\ \n");
	output.append("\\hline  \n");
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");

	return output;
}

string ReporteFuenteDatos::ImprimeValidadoresLATEX()
{
	string output("");
	output.append("\\subsection{Validadores \\medskip} \n" );
	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | c | }\n");
	output.append("\\hline                 \n");
	output.append("Descripcion & Valor  \\\\ \n");
	output.append("\\hline      \n");
	output.append("\\# validadores & " + StringFunctions::Int2String(nValidadoresZonasPagasTabla) + "\\\\ \n");
	output.append("\\# validadores repetidos & " + StringFunctions::Int2String(nValidadoresRepetidosZonasPagasTabla) + " \\\\ \n");
	output.append("\\# validadores sin Zona paga & " + StringFunctions::Int2String(nValidadoresSinZonaPaga) + " \\\\ \n");
	output.append("\\hline  \n");
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");

	output.append("Lista de paraderos de validadores no encontrados en secuencia : \\medskip \n");

	if(ValidadoresConParaderoSinSecuencia.empty())
	{
		output.append("(NO EXISTEN) \\medskip \n");
	}
	else
	{
		output.append(StringFunctions::Int2String(ValidadoresConParaderoSinSecuencia.size()) + " \\medskip \n");
		output.append("\\begin{center}\n");
		output.append("\\begin{tabular}{ | l | r |}\n");
		output.append("\\hline             \n");    
		output.append(" Paradero & cantidad \\\\ \n");
		output.append("\\hline      \n");
		for(map<string, int >::iterator it = ValidadoresConParaderoSinSecuencia.begin();it!=ValidadoresConParaderoSinSecuencia.end();it++)
		{
			output.append((*it).first + " & " + StringFunctions::Int2String((*it).second) + " \\\\ \n");
		}
		output.append("\\hline  \n");
		output.append("\\end{tabular}\n");
		output.append("\\end{center}\n");
	}

	output.append("Lista de validadores con servicios repetidos : \\medskip \n");

	if(ValidadoresConServiciosRepetidos.empty())
	{
		output.append("(NO EXISTEN) \\medskip \n");
	}
	else
	{
		output.append(StringFunctions::Int2String(ValidadoresConServiciosRepetidos.size()) + " \\medskip \n");
		output.append("\\begin{center}\n");
		output.append("\\begin{tabular}{ | l | r |}\n");
		output.append("\\hline             \n");    
		output.append(" Paradero & cantidad \\\\ \n");
		output.append("\\hline      \n");
		for(map<string, int >::iterator it = ValidadoresConServiciosRepetidos.begin();it!=ValidadoresConServiciosRepetidos.end();it++)
		{
			output.append((*it).first + " & " + StringFunctions::Int2String((*it).second) + " \\\\ \n");
		}
		output.append("\\hline  \n");
		output.append("\\end{tabular}\n");
		output.append("\\end{center}\n");
	}
	
	return output;
}

string ReporteFuenteDatos::ImprimeRedParadasLATEX()
{
	string output("");
	output.append("\\subsection{Red de paradas \\medskip}\n");

	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | r |}\n");
	output.append("\\hline             \n");    
	output.append("  Descripcion & Valor  \\\\ \n");
	output.append("\\hline      \n");
	output.append("\\# Paradas & "+StringFunctions::Int2String(this->nRedParadas)+" \\\\ \n");
	output.append("\\# Paradas inactivas & "+StringFunctions::Int2String(this->nRedParadasInactivas)+" \\\\ \n");
	output.append("\\# Paradas repetidas & "+StringFunctions::Int2String(this->nRedParadasRepetidas)+" \\\\ \n");
	output.append("\\hline  \n");
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");

	return output;
}

string ReporteFuenteDatos::ImprimeRedParadasASCII(int iCapitulo)
{
	string output("");
	output.append(StringFunctions::Int2String(iCapitulo) + ".9.- Red de paradas\n");
	output.append("\n");
	output.append("# Paradas : " + StringFunctions::Int2String(this->nRedParadas) + "\n");
	output.append("# Paradas inactivas : " + StringFunctions::Int2String(this->nRedParadasInactivas) + "\n");
	output.append("# Paradas repetidas : " + StringFunctions::Int2String(this->nRedParadasRepetidas) + "\n");
	output.append("\n");
	output.append("\n");

	return output;
}

string ReporteFuenteDatos::ImprimeZonificacionesASCII(int iCapitulo)
{
	string output("");
	output.append(StringFunctions::Int2String(iCapitulo) + ".9.- Zonificaciones\n");
	output.append("\n");
	output.append(" zonificacion | cantidad \n");
	for(map<string, int >::iterator it = zonificaciones.begin();it!=zonificaciones.end();it++)
	{
		output.append((*it).first + " | " + StringFunctions::Int2String((*it).second) + "\n");
	}
	output.append(" zonificacion | # zonas menos 2 nodos \n");
	for(map<string, int >::iterator it = zonasMenos2Nodos.begin();it!=zonasMenos2Nodos.end();it++)
	{
		output.append((*it).first + " | " + StringFunctions::Int2String((*it).second) + "\n");
	}

	return output;
}

string ReporteFuenteDatos::ImprimeZonificacionesLATEX()
{
	string output("");
	output.append("\\subsection{Zonificaciones \\medskip}\n");

	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | r |}\n");
	output.append("\\hline             \n");    
	output.append(" Zonificacion & \\# Zonas \\\\ \n");
	output.append("\\hline      \n");
	int i=0;
	for(map<string, int >::iterator it = zonificaciones.begin();it!=zonificaciones.end();it++,++i)
	{
		output.append((*it).first + " & " + StringFunctions::Int2String((*it).second) + " \\\\ \n");
		output.append("\\hline  \n");
	}
	output.append("\\hline  \n");
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");

	output.append("\\medskip\n");

	output.append("Zonificaciones con zonas con menos de 2 nodos : \\medskip \n");

	if(zonasMenos2Nodos.empty())
	{
		output.append("(NO EXISTEN) \\medskip \n");
	}
	else
	{
		output.append(StringFunctions::Int2String(zonasMenos2Nodos.size()) + " \\medskip \n");
		output.append("\\begin{center}\n");
		output.append("\\begin{tabular}{ | l | r |}\n");
		output.append("\\hline             \n");    
		output.append(" Zonificacion & cantidad \\\\ \n");
		output.append("\\hline      \n");
		for(map<string, int >::iterator it = zonasMenos2Nodos.begin();it!=zonasMenos2Nodos.end();it++)
		{
			output.append((*it).first + " & " + StringFunctions::Int2String((*it).second) + " \\\\ \n");
		}
		output.append("\\hline  \n");
		output.append("\\end{tabular}\n");
		output.append("\\end{center}\n");
	}


	return output;
}

string ReporteFuenteDatos::ImprimeValidadoresASCII(int iCapitulo)
{
	string output("");
	output.append(StringFunctions::Int2String(iCapitulo) + ".9.- Validadores\n");
	output.append("\n");
	output.append("# validadores : " + StringFunctions::Int2String(nValidadoresZonasPagasTabla) + "\n");
	output.append("# validadores repetidos : " + StringFunctions::Int2String(nValidadoresRepetidosZonasPagasTabla) + "\n");
	output.append("# validadores sin Zona paga : " + StringFunctions::Int2String(nValidadoresSinZonaPaga) + "\n");
	output.append("\n");
	output.append("\n");

	output.append("Lista de paraderos de validadores no encontrados en secuencia : ");
	if(ValidadoresConParaderoSinSecuencia.empty())
	{
		output.append("(NO EXISTEN) \n");
	}
	else
	{
		output.append(StringFunctions::Int2String(ValidadoresConParaderoSinSecuencia.size()) + " \n");
		output.append(" Paradero | cantidad \n");
		for(map<string, int >::iterator it = ValidadoresConParaderoSinSecuencia.begin();it!=ValidadoresConParaderoSinSecuencia.end();it++)
			output.append((*it).first + " | " + StringFunctions::Int2String((*it).second) + "\n");
	}

	output.append("Lista de validadores con servicios repetidos : ");
	if(ValidadoresConServiciosRepetidos.empty())
	{
		output.append("(NO EXISTEN) \n");
	}
	else
	{
		output.append(StringFunctions::Int2String(ValidadoresConServiciosRepetidos.size()) + " \n");
		output.append(" Paradero | cantidad \n");
		for(map<string, int >::iterator it = ValidadoresConServiciosRepetidos.begin();it!=ValidadoresConServiciosRepetidos.end();it++)
			output.append((*it).first + " | " + StringFunctions::Int2String((*it).second) + "\n");
	}

	return output;
}

string ReporteFuenteDatos::ImprimeASCII(int iCapitulo)
{
	string output;

	output.append(StringFunctions::Int2String(iCapitulo) + ".- Seccion Datos de Entrada (Lectura)\n");
	output.append("------------------------------------------------------------------------------------------------\n");
	output.append("\n");

	output.append(ImprimeDiccionarioASCII(iCapitulo));

	output.append(ImprimeReporte22ASCII(iCapitulo));

	output.append(ImprimeReporte196ASCII(iCapitulo));

	output.append(ImprimePosicionamientoASCII(iCapitulo));

	output.append(ImprimeRutasASCII(iCapitulo));

	output.append(ImprimeParadasASCII(iCapitulo));

	output.append(ImprimeRedParadasASCII(iCapitulo));

	output.append(ImprimeZPsASCII(iCapitulo));

	output.append(ImprimeEvasionASCII(iCapitulo));

	output.append(ImprimeMetroASCII(iCapitulo));

	output.append(ImprimeNoExistenciaASCII(iCapitulo));

	output.append(ImprimeValidadoresASCII(iCapitulo));

	output.append(ImprimeZonificacionesASCII(iCapitulo));

	return output;
}

string ReporteFuenteDatos::ImprimeDiccionarioASCII(int iCapitulo)
{
	string output("");
	output.append(StringFunctions::Int2String(iCapitulo) + ".1.- Diccionario\n");
	output.append("\n");
	output.append("N° de regitros : " + StringFunctions::Int2String(nRegistrosDiccionario) + "\n");
	output.append("N° codigos de SS en reporte 2.2 no encontrados en diccionario : " + StringFunctions::Int2String(nSSnoEncontradosEnDiccionario) + "\n");
	for(map< string , int >::iterator it = ssNoEncontradosEnDiccionario.begin();it!=ssNoEncontradosEnDiccionario.end();it++)
	{
		output.append((*it).first + "|" + StringFunctions::Int2String((*it).second) + "\n");
	}
	output.append("\n");
	output.append("\n");

	return output;
}

string ReporteFuenteDatos::ImprimeReporte22ASCII(int iCapitulo)
{
	string output("");
	output.append(StringFunctions::Int2String(iCapitulo) + ".2.- Reporte 2.2\n");
	output.append("\n");
	output.append("N° de registros : " + StringFunctions::Int2String(nRegistros22) + "\n");
	output.append("N° de expediciones : " + StringFunctions::Int2String(nExp22) + "\n");
	output.append("N° de patentes : " + StringFunctions::Int2String(nSitios22) + "\n");
	output.append("N° de expediciones nulas : " + StringFunctions::Int2String(nExp22Menos2Segundos) + "\n");
	output.append("\n");
	output.append(this->hist22.Imprime("Estadisticas tiempos de expedicion",ASCII));
	output.append("\n");
	output.append("\n");
	return output;
}

string ReporteFuenteDatos::ImprimeReporte196ASCII(int iCapitulo)
{
	string output("");
	output.append(StringFunctions::Int2String(iCapitulo) + ".3.- Reporte 1.96\n");
	output.append("\n");
	output.append("N° de registros : " + StringFunctions::Int2String(nRegistros196) + "\n");
	output.append("N° de registros nulos : " + StringFunctions::Int2String(nRegistrosNulos196) + "\n");
	output.append("N° de expediciones : " + StringFunctions::Int2String(nExp196) + "\n");
	output.append("N° de patentes : " + StringFunctions::Int2String(nSitios196) + "\n");
	output.append("N° de patentes no encontradas en 2.2: " + StringFunctions::Int2String(nSitiosEn196NoEn22) + "\n");
	output.append("\n");
	output.append("\n");
	return output;
}

string ReporteFuenteDatos::ImprimePosicionamientoASCII(int iCapitulo)
{
	string output("");
	output.append(StringFunctions::Int2String(iCapitulo) + ".4.- Posicionamiento\n");
	output.append("\n");
	output.append("N° de patentes : " + StringFunctions::Int2String(nSitiosEnPos) + "\n");
	output.append("N° de patentes no encontradas en expediciones: " + StringFunctions::Int2String(nSitiosEnPosNoEnExps) + "\n");
	output.append("N° de registros GPS: " + StringFunctions::Int2String(nRegistrosGPS) + "\n");
	output.append("Nº de registros GPS sin ppu & "+StringFunctions::Int2String(nRegistrosGPSPatenteNula)+" & \\\\ \n");
	output.append("N° de registros GPS repetidos: " + StringFunctions::Int2String(nRegistrosGPSRepetidos) + "|" + StringFunctions::Double2String(nRegistrosGPSRepetidos*100.0/(nRegistrosGPS),5) + "%\n");
	output.append("N° de registros GPS utiles: " + StringFunctions::Int2String(nRegistrosGPS-nRegistrosGPSRepetidos) + "|" + StringFunctions::Double2String((nRegistrosGPS-nRegistrosGPSRepetidos)*100.0/(nRegistrosGPS),5) + "%\n");
	output.append("\n");
	output.append(this->histTGps.Imprime("Estadisticas tiempos entre GPS",ASCII));
	output.append("\n");
	output.append(this->histGpsPorHora.Imprime("GPS por Hora",ASCII));
	output.append("\n");
	return output;
}

string ReporteFuenteDatos::ImprimeRutasASCII(int iCapitulo)
{
	string output;
	output.append(StringFunctions::Int2String(iCapitulo) + ".5.- Rutas\n");
	output.append("\n");
	output.append("N° de rutas : " + StringFunctions::Int2String(nRutas) + "\n");
	output.append("Ruta mas larga : " + rutaMasLarga.first + " | " + StringFunctions::Double2String(float(rutaMasLarga.second)/1000.0,5) + "(Kms).\n");
	output.append("Ruta mas corta : " + rutaMasCorta.first + " | " + StringFunctions::Double2String(float(rutaMasCorta.second)/1000.0,5) + "(Kms).\n");
	output.append("\n");
	output.append("Rutas con posibles problemas  : \n");	
	output.append("\n");
	output.append("Ruta|#NodosRepetidos|#NodosFueraDeRuta|#NodosFueraDeSantiago\n");
	for(map< string , ErroresRuta >::iterator it = rutasErradas.begin();it!=rutasErradas.end();it++)
	{
			output.append((*it).first + "|");
			output.append(StringFunctions::Int2String((*it).second.nNodosRepetidos) + "|");
			output.append(StringFunctions::Int2String((*it).second.nNodosFueraDeRuta) + "|");
			output.append(StringFunctions::Int2String((*it).second.nNodosFueraDeSantiago) + "|");
			output.append("\n");
	}
	output.append("\n");
	output.append("\n");
	return output;
}

string ReporteFuenteDatos::ImprimeParadasASCII(int iCapitulo)
{
	string output("");
	output.append(StringFunctions::Int2String(iCapitulo) + ".6.- Secuencia de paradas\n");
	output.append("\n");
	output.append("N° de secuencias : " + StringFunctions::Int2String(this->nSecParadas) + "\n");
	output.append("Secuencia mas larga : " + nMaxParadas.first + " | " + StringFunctions::Int2String(nMaxParadas.second) + " paradas.\n");
	output.append("Secuencia corta : " + nMinParadas.first + " | " + StringFunctions::Int2String(nMinParadas.second) + " paradas.\n");
	output.append("\n");
	output.append("Secuencia sin ruta encontrada  : ");
	if(secuenciasSinRutas.empty())
	{
		output.append("(NO EXISTEN) \n");
	}
	else
	{
		output.append("\n\n");
		for(map<string,int>::iterator it=secuenciasSinRutas.begin();it!=secuenciasSinRutas.end();it++)
		{
			output.append((*it).first);
			output.append("\n");
		}
	}
	output.append("Paradas sin codigo conocido  : ");
	if(paradasSinFormato.empty())
	{
			output.append("(NO EXISTEN) \n");
	}
	else
	{
		output.append("\n");
		for(map<string,int>::iterator it=paradasSinFormato.begin();it!=paradasSinFormato.end();it++)
		{
			output.append((*it).first + "|" + StringFunctions::Int2String((*it).second));
			output.append("\n");
		}
	}

	output.append("Secuencias con posibles problemas  : \n");	
	output.append("Secuencia|Parada|FueraDeRuta(mts)|FueraDeSantiago\n");
	for(map<string, vector < ErroresParadas > >::iterator it = paradasErradas.begin();it!=paradasErradas.end();it++)
	{
		for(vector< ErroresParadas >::iterator it2=(*it).second.begin();it2!=(*it).second.end();it2++)
		{
			output.append((*it).first + "|");
			output.append((*it2).codigoParada + "|");
			output.append(StringFunctions::Int2String((*it2).fueraDeRuta) + "|");

			if((*it2).fueraDeSantiago)
				output.append("X|");
			else
				output.append("-|");

			output.append("\n");
		}
	}
	output.append("\n");

	output.append("Servicios y paradas con incosistencias en secuencia de avance  : " + StringFunctions::Int2String(int(inconsistenciasSecuencia.size())) + "\n");	
	output.append("Servicio|Parada\n");
	for(map<string, string >::iterator it = inconsistenciasSecuencia.begin();it!=inconsistenciasSecuencia.end();it++)
	{
		///Chequeo que no es un paradero que tiene problemas en el formato del codigo
		if(int(StringFunctions::Explode((*it).second,'-').size())!=5)
			continue;

		output.append((*it).first + "|");
		output.append((*it).second + "\n");
	}
	output.append("\n");
	output.append("\n");

	return output;
}

string ReporteFuenteDatos::ImprimeZPsASCII(int iCapitulo)
{
	string output;
	output.append(StringFunctions::Int2String(iCapitulo) + ".7.- Zonas Pagas\n");
	output.append("\n");
	output.append("# Zonas Pagas  : " + StringFunctions::Int2String(nZonasPagas) + "\n");
	output.append("# Validadores  : " + StringFunctions::Int2String(nValidadoresZonasPagas) + "\n");
	output.append("# Paraderos de zonas pagas no encontrados  : " + StringFunctions::Int2String(paraderosNoEncontradosDeZonasPagas.size()) + "\n");
	for(map<string, string >::iterator it = paraderosNoEncontradosDeZonasPagas.begin();it!=paraderosNoEncontradosDeZonasPagas.end();it++)
	{
		output.append((*it).first + "|");
		output.append((*it).second + "\n");
	}
	output.append("# Paraderos de distribuciones no encontrados en zonas pagas : " + StringFunctions::Int2String(paraderosNoEncontradosDeDistribuciones.size()) + "\n");
	for(map<string, int >::iterator it = paraderosNoEncontradosDeDistribuciones.begin();it!=paraderosNoEncontradosDeDistribuciones.end();it++)
	{
		output.append((*it).first + "|");
		output.append((*it).second + "\n");
	}
	output.append("# Validadores-MediaHora con rara distribucion : " + StringFunctions::Int2String(distribucionesMalas.size()) + "\n");
	for(map< string , map<int, map<string,double> > >::iterator ival = distribucionesMalas.begin();ival!=distribucionesMalas.end();ival++)
	{
		for(map<int, map<string,double> >::iterator imh = (*ival).second.begin();imh!=(*ival).second.end();imh++)
		{
			output.append((*ival).first + "-");
			output.append(tsh.Seconds2TimeStampInDay((*imh).first) + "\n");
			for(map<string,double>::iterator iserv = (*imh).second.begin();iserv!=(*imh).second.end();iserv++)
			{
				output.append("	" + (*iserv).first + "|" + StringFunctions::Double2String((*iserv).second,5));
			}
		}
	}
	output.append("# Patentes con capacidad que no se usan : " + StringFunctions::Int2String(nPatentesEnCapacidadesNoEnResto) + "\n");
	//output.append("# Cruces en zonas pagas que no encontraron capacidad : " + StringFunctions::Int2String(nPatentesEnRestoNoEnCapacidades) + "\n");
	//for(map<string,int>::iterator it=patentesSinCapacidadEncontrada.begin();it!=patentesSinCapacidadEncontrada.end();it++)
	//{
	//	output.append((*it).first + "|" + StringFunctions::Int2String((*it).second)+ "\n");
	//}
	return output;
}

string ReporteFuenteDatos::ImprimeEvasionASCII(int iCapitulo)
{
	string output("");
	output.append(StringFunctions::Int2String(iCapitulo) + ".8.- Evasion\n");
	output.append("# registros : " + StringFunctions::Int2String(nEvasionRegistros) + "\n");
	output.append("# registros repetidos : " + StringFunctions::Int2String(nEvasionRegistrosRepetidos) + "\n");

	output.append("Lista de paraderos-servicios repetidos : ");

	if(evasionRepetida.empty())
	{
		output.append("(NO EXISTEN) \n");
	}
	else
	{
		output.append(StringFunctions::Int2String(evasionRepetida.size()) + " \n");
		output.append(" Paradero-servicio | cantidad \n");
		for(map<string, int >::iterator it = evasionRepetida.begin();it!=evasionRepetida.end();it++)
		{
			output.append((*it).first + " | " + StringFunctions::Int2String((*it).second) + "\n");
		}
	}

	output.append("Lista de zonas repetidos : ");

	if(evasionRepetidaZonas.empty())
	{
		output.append("(NO EXISTEN) \n");
	}
	else
	{
		output.append(StringFunctions::Int2String(evasionRepetidaZonas.size()) + " \n");
		output.append(" Paradero-servicio | cantidad \n");
		for(map<string, int >::iterator it = evasionRepetidaZonas.begin();it!=evasionRepetidaZonas.end();it++)
		{
			output.append((*it).first + " | " + StringFunctions::Int2String((*it).second) + "\n");
		}
	}

	return output;
}

string ReporteFuenteDatos::ImprimeMetroASCII(int iCapitulo)
{
	string output("");
	output.append(StringFunctions::Int2String(iCapitulo) + ".9.- Metro\n");
	output.append("\n");
	output.append("# estaciones : " + StringFunctions::Int2String(nEstacionesMetro) + "\n");
	output.append("# estaciones repetidas : " + StringFunctions::Int2String(nEstacionesMetroRepetidas) + "\n");
	output.append("\n");
	output.append("\n");

	return output;
}

string ReporteFuenteDatos::ImprimeNoExistenciaASCII(int iCapitulo)
{
	string output;

	output.append("\n");
	output.append(StringFunctions::Int2String(iCapitulo) + ".10 Datos de entrada no encontrados : " + StringFunctions::Int2String(noExistenciaDatos.size()) + "\n");
	output.append("Servicio|Ruta Sin diccionario|Secuencia sin diccionario|No existe ruta|No existe Secuencia\n");
	for(map< string , ErroresNoExistenciaDatos >::iterator ierr = noExistenciaDatos.begin();ierr!=noExistenciaDatos.end();ierr++)
	{
		output.append((*ierr).first + "|");
		if((*ierr).second.noDicRuta)	output.append("X|");
		else							output.append("-|");

		if((*ierr).second.noDicSec)	output.append("X|");
		else							output.append("-|");

		if((*ierr).second.noRuta)	output.append("X|");
		else							output.append("-|");

		if((*ierr).second.noSec)	output.append("X");
		else							output.append("-");

		output.append("\n");
	}
	output.append("\n");
	output.append("\n");

	return output;
}

void ReporteFuenteDatos::InsertaRutaError(string codigoRuta,int nNodosRepetidos_,int nNodosFueraDeRuta_,int nNodosFueraDeSantiago_)
{
	map< string, ErroresRuta>::iterator it= rutasErradas.find(codigoRuta);
	
	/////Caso que no exista
	if(it==rutasErradas.end())
	{
		ErroresRuta e;
		e.nNodosFueraDeRuta=nNodosFueraDeRuta_;
		e.nNodosFueraDeSantiago=nNodosFueraDeSantiago_;
		e.nNodosRepetidos = nNodosRepetidos_;
		
		rutasErradas.insert(pair< string , ErroresRuta >(codigoRuta,e));
	}
	/////Caso que ya exista, es decir, en otro servicio tambien esta fuera de ruta
	else
	{
		(*it).second.nNodosFueraDeRuta+=nNodosFueraDeRuta_;
		(*it).second.nNodosFueraDeSantiago+=nNodosFueraDeSantiago_;
		(*it).second.nNodosRepetidos+=nNodosRepetidos_;
	}	
}

void ReporteFuenteDatos::InsertaParadaError(string codigoSec,string codigoParada,int fueraDeRuta,bool fueraDeSantiago)
{
	map<string, vector < ErroresParadas > >::iterator it= paradasErradas.find(codigoSec);
	
	///Chequeo que no es un paradero que tiene problemas en el formato del codigo
	if(int(StringFunctions::Explode(codigoParada,'-').size())!=5)
		return;

	/////Caso que no exista
	if(it==paradasErradas.end())
	{
		ErroresParadas e;

		e.codigoParada = codigoParada;
		e.fueraDeRuta = fueraDeRuta;
		e.fueraDeSantiago = fueraDeSantiago;

		vector< ErroresParadas > ep;
		ep.push_back(e);
		
		paradasErradas.insert(pair<string, vector < ErroresParadas > >(codigoSec,ep));
	}
	/////Caso que ya exista, es decir, en otro servicio tambien esta fuera de ruta
	else
	{
		ErroresParadas e;
		e.codigoParada = codigoParada;
		e.fueraDeRuta = fueraDeRuta;
		e.fueraDeSantiago = fueraDeSantiago;

		(*it).second.push_back(e);
	}
}

void ReporteFuenteDatos::InsertaDistribucionMala(string validador,int mediahora,map<string,double> distribucion)
{
	map< string , map<int, map<string,double> > >::iterator ival=distribucionesMalas.find(validador);

	if(ival==distribucionesMalas.end())
	{
		map< int, map< string , double> > tmp;
		tmp.insert(pair< int , map< string , double > >(mediahora,distribucion));
		distribucionesMalas.insert(pair< string, map< int, map< string , double> > >(validador, tmp ));
	}
	else
	{
		map<int, map<string,double> >::iterator imh = (*ival).second.find(mediahora);

		if(imh==(*ival).second.end())
		{
			(*ival).second.insert(pair<int , map< string , double> >(mediahora,distribucion));
		}
		else
		{
			cout << "ERROR : Raro val-mediahora repetido!" << endl;
		}
	}
}