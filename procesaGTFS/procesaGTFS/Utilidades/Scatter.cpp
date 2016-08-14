/**
* Project: "pvm".
* Package: Utilidades
* Class: Scatter
* Created by: Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* Terms: These coded instructions, statements, and computer programs are
* produced as part of a software project. They contain unpublished
* propietary information and are protected by copyright law. They may
* not be disclosed to third parties or copied or duplicated in any form,
* in whole or in part, without the prior written consent of Pragxis SpA.
* Copyright:  Pragxis (c) 2013
* Last modified : Mauricio Zuñiga 15-04-2013
*/
#include "Scatter.h"

Scatter::Scatter()
{

}

Scatter::~Scatter()
{
}

Scatter::Scatter(double maxsize_,string patfilein_, int ix_, int iy_, string pathout_)
{
	this->pathIn = patfilein_;
	this->maxsize = maxsize_;
	this->pathOut = pathout_;
	this->ix = ix_;
	this->iy = iy_;
	//this->minX = minx_;
	//this->maxX = maxx_;
	//for(double i=minX; i < maxX ; i+=delta)
	//{
	//	values.insert(pair< double , int >(i,0));
	//}
}


string Scatter::Imprime(string titulo,int tipoReporte)
{
	string output=string("");

	if(tipoReporte==ASCII)
		return ImprimeASCII(titulo);
	else if(tipoReporte==LATEX)
		return ImprimeLATEX(titulo);

	return output;
}

string Scatter::ImprimeLATEX(string titulo)
{
	string output=string("");

	GraficaPython(titulo);

	string pathLinux = StringFunctions::ReplaceChar(pathOut, '\\', '/');
	string titulo_ = StringFunctions::ReplaceChar(titulo, ' ', '_');


	output.append("\\begin{figure}[h]\n");
	output.append("\\centering\n");
	output.append("\\includegraphics[width=1.2\\textwidth]{" + pathLinux + titulo_  + ".png}\n");
	output.append("\\caption{"+titulo+"}\n");
	output.append("\\label{fig:"+titulo+"}\n");
	output.append("\\end{figure}\n");

	return output;
}

string Scatter::ImprimeASCII(string titulo)
{
	string output=string("");

	//output.append(titulo);
	//output.append("\n");
	//output.append("----------------------------");
	//output.append("\n");
	//output.append("Histograma : \n");
	//for(map< double , int >::iterator iaxis=values.begin();iaxis!=values.end();iaxis++)
	//	output.append(StringFunctions::Double2String((*iaxis).first,5) + "|");
	//output.append("\n");
	//for(map< double , int >::iterator iaxis=values.begin();iaxis!=values.end();iaxis++)
	//	output.append(StringFunctions::Int2String((*iaxis).second) + "|");
	//output.append("\n");
	//output.append("N : " + StringFunctions::Int2String(this->n) + "\n");
	//output.append("Sum : " + StringFunctions::Double2String(this->sum,5) + "\n");
	//output.append("Min : " + StringFunctions::Double2String(this->minValue,5) + "\n");
	//output.append("Max : " + StringFunctions::Double2String(this->maxValue,5) + "\n");
	//output.append("Media : " + StringFunctions::Double2String(this->sum/double(this->n),5) + "\n");
	//output.append("\n");

	return output;
}

void Scatter::GraficaPython(string titulo)
{
	string pathTmp = string(this->pathOut + "scatter.py");
	ofstream output;
	output.open(pathTmp.c_str());

	output << "import csv" << endl;
	output << "import numpy as np" << endl;
	output << "import scipy as sp" << endl;
	output << "import matplotlib.pyplot as plt" << endl;
	output << "import matplotlib.mlab as mlab" << endl;

	string pathLinuxIn = StringFunctions::ReplaceChar(this->pathIn, '\\', '/');
	output << "spamReader = csv.reader(open('" << pathLinuxIn << "', 'rb'), delimiter='|', quotechar=' ')" << endl;

	output << "x0 = []" << endl;
	output << "y0 = []" << endl;
	output << "x1 = []" << endl;
	output << "y1 = []" << endl;
	output << "xcol=10" << endl;
	output << "ycol=11" << endl;
	output << "ccol=6" << endl;
	output << "i=0" << endl;
	output << "for row in spamReader:" << endl;
	output << "	if(i!=0):" << endl;
	output << "		if row[ccol]=='C' :" << endl;
	output << "			x0.append(float(row[xcol]))" << endl;
	output << "			y0.append(float(row[ycol]))" << endl;
	output << "		else :" << endl;
	output << "			x1.append(float(row[xcol]))" << endl;
	output << "			y1.append(float(row[ycol]))" << endl;
	output << "		" << endl;
	output << "	i+=1" << endl;
	output << "plt.subplot(121);" << endl;
	output << "plt.xlim(0, 600)" << endl;
	output << "plt.ylim(0, 600)" << endl;
	output << "plt.title('Tiempos de Expediciones Cumplen')" << endl;
	output << "plt.scatter(x0, y0, c='g', marker='o',label='C')" << endl;
	output << "plt.subplot(122);" << endl;
	output << "plt.xlim(0, 600)" << endl;
	output << "plt.ylim(0, 600)" << endl;
	output << "plt.title('Tiempos de Expediciones No Cumplen')" << endl;
	output << "plt.scatter(x1, y1, c='r', marker='o',label='NC')" << endl;
	output << "fig = plt.gcf()" << endl;
	output << "fig.set_size_inches(25.5,10.5)" << endl;
	output << "plt.savefig('Tiempos_de_Expediciones.png')" << endl;


	string pathLinuxOut = StringFunctions::ReplaceChar(this->pathOut, '\\', '/');
	string titulo_ = StringFunctions::ReplaceChar(titulo, ' ', '_');

	output << "plt.savefig('" << pathLinuxOut << titulo_ << ".png')" << endl;
	output.close();

	system(string("python " + pathTmp).c_str());
}

//void Histogram::Insert(double value)
//{
//	n+=1.0;
//	sum+=value;
//
//	if(minValue > value)
//		minValue = value;
//
//	if(maxValue < value )
//		maxValue = value;
//
//	//bool encontro=false;
//	for(map< double , int >::iterator it=values.begin(); it!=values.end(); it++)
//	{
//		//cout << (*it).first << "|" << value << "|" << (*it).first + delta << endl;
//		if((*it).first <= value && value < (*it).first + delta)
//		{
//			(*it).second++;
//			//encontro=true;
//			break;
//		}
//	}
//	//if(!encontro)
//	//	cout << value << endl;
//}
