/**
* Project: "pvm".
* Package: Utilidades
* Class: Histogram
* Created by: Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* Terms: These coded instructions, statements, and computer programs are
* produced as part of a software project. They contain unpublished
* propietary information and are protected by copyright law. They may
* not be disclosed to third parties or copied or duplicated in any form,
* in whole or in part, without the prior written consent of Pragxis SpA.
* Copyright:  Pragxis (c) 2013
* Last modified : Mauricio Zuñiga 15-04-2013
*/
#include "Histogram.h"

Histogram::Histogram()
{
	this->minValue = numeric_limits<double>::max();
	this->maxValue = numeric_limits<double>::min();
	this->meanValue = 0.0;
	this->devstdValue = 0.0;
	this->n = 0.0;
	this->sum = 0.0;
}

Histogram::~Histogram()
{
}

Histogram::Histogram(double minx_,double maxx_,double delta_,string path_)
{
	this->path = path_;
	this->delta = delta_;
	this->minX = minx_;
	this->maxX = maxx_;
	for(double i=minX; i < maxX ; i+=delta)
	{
		values.insert(pair< double , int >(i,0));
	}
	this->minValue = numeric_limits<double>::max();
	this->maxValue = numeric_limits<double>::min();
	this->meanValue = 0.0;
	this->devstdValue = 0.0;
	this->n = 0.0;
	this->sum = 0.0;
}

void Histogram::Insert(double value)
{
	n+=1.0;
	sum+=value;

	if(minValue > value)
		minValue = value;

	if(maxValue < value )
		maxValue = value;

	//bool encontro=false;
	for(map< double , int >::iterator it=values.begin(); it!=values.end(); it++)
	{
		//cout << (*it).first << "|" << value << "|" << (*it).first + delta << endl;
		if((*it).first <= value && value < (*it).first + delta)
		{
			(*it).second++;
			//encontro=true;
			break;
		}
	}
	//if(!encontro)
	//	cout << value << endl;
}

string Histogram::Imprime(string titulo,int tipoReporte)
{
	string output=string("");

	if(tipoReporte==ASCII)
		return ImprimeASCII(titulo);
	else if(tipoReporte==LATEX)
		return ImprimeLATEX(titulo);

	return output;
}

string Histogram::ImprimeLATEX(string titulo)
{
	string output=string("");

	output.append("\\begin{center}\n");
	output.append("\\begin{tabular}{ | l | r | }\n");
	output.append("\\hline                 \n");
	output.append(titulo + " & \\\\ \n");
	output.append("\\hline      \n");
	output.append("Cantidad  & "+StringFunctions::Int2String(this->n)+"  \\\\ \n");
	output.append("Suma & " + StringFunctions::Double2String(this->sum,5) + " \\\\ \n");
	output.append("Minimo & "+StringFunctions::Double2String(this->minValue,5) +" \\\\ \n");
	output.append("Maximo & "+ StringFunctions::Double2String(this->maxValue,5) + "  \\\\ \n");
	output.append("Media & "+ StringFunctions::Double2String(this->sum/double(this->n),5) + "\\\\ \n");
	output.append("\\hline  \n");
	output.append("\\end{tabular}\n");
	output.append("\\end{center}\n");

	GraficaPython(titulo);

	string pathLinux = StringFunctions::ReplaceChar(path, '\\', '/');
	string titulo_ = StringFunctions::ReplaceChar(titulo, ' ', '_');


	output.append("\\begin{figure}[h]\n");
	output.append("\\centering\n");
	output.append("\\includegraphics[width=0.5\\textwidth]{" + pathLinux + titulo_  + ".png}\n");
	output.append("\\caption{"+titulo+"}\n");
	output.append("\\label{fig:"+titulo+"}\n");
	output.append("\\end{figure}\n");

	return output;
}

string Histogram::ImprimeASCII(string titulo)
{
	string output=string("");

	output.append(titulo);
	output.append("\n");
	output.append("----------------------------");
	output.append("\n");
	output.append("Histograma : \n");
	for(map< double , int >::iterator iaxis=values.begin();iaxis!=values.end();iaxis++)
		output.append(StringFunctions::Double2String((*iaxis).first,5) + "|");
	output.append("\n");
	for(map< double , int >::iterator iaxis=values.begin();iaxis!=values.end();iaxis++)
		output.append(StringFunctions::Int2String((*iaxis).second) + "|");
	output.append("\n");
	output.append("N : " + StringFunctions::Int2String(this->n) + "\n");
	output.append("Sum : " + StringFunctions::Double2String(this->sum,5) + "\n");
	output.append("Min : " + StringFunctions::Double2String(this->minValue,5) + "\n");
	output.append("Max : " + StringFunctions::Double2String(this->maxValue,5) + "\n");
	output.append("Media : " + StringFunctions::Double2String(this->sum/double(this->n),5) + "\n");
	output.append("\n");

	return output;
}

void Histogram::GraficaPython(string titulo)
{
	string pathTmp = string(path + "histograma.py");
	ofstream output;
	output.open(pathTmp.c_str());

	output << "import matplotlib.pyplot as plt" << endl;

	output << "plt.plot([" ;
	map< double , int >::iterator iaxisEnd ;
	for(map< double , int >::iterator iaxis=values.begin();iaxis!=values.end();iaxis++)
	{
		iaxisEnd = iaxis;
		iaxisEnd++;

		if(iaxisEnd==values.end())
			output << StringFunctions::Int2String((*iaxis).first) << "]" ;
		else
			output << StringFunctions::Int2String((*iaxis).first) << "," ;
	}
	output << ",[" ;

	for(map< double , int >::iterator iaxis=values.begin();iaxis!=values.end();iaxis++)
	{
		iaxisEnd = iaxis;
		iaxisEnd++;

		if(iaxisEnd==values.end())
			output << StringFunctions::Int2String((*iaxis).second) << "])" ;
		else
			output << StringFunctions::Int2String((*iaxis).second) << "," ;
	}
	output << endl;
	output << "plt.title('" << titulo << "')" << endl;

	string pathLinux = StringFunctions::ReplaceChar(path, '\\', '/');
	string titulo_ = StringFunctions::ReplaceChar(titulo, ' ', '_');

	output << "plt.savefig('" << pathLinux << titulo_ << ".png')" << endl;
	output.close();

	system(string("python " + pathTmp).c_str());
}