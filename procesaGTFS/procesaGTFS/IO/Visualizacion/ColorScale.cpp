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
#include "ColorScale.h"

namespace ColorScale {

string int2ColorHex(double min,double max,double delta,double valor)
{
	string output("");
	double escala=(valor-min)/(max-min);
	
	output.append(int2Hex(scalaAVR(escala,1)));	
	output.append(int2Hex(scalaAVR(escala,2)));
	output.append(int2Hex(scalaAVR(escala,3)));
 	
	

	return output;
}

int int2ColorInt(double min,double max,double delta,double valor)
{
	double escala=(valor-min)/(max-min);

	return scalaAVR(escala,1)*65536 + scalaAVR(escala,2)*256 + scalaAVR(escala,3);	
}

string int2Hex(int N)
{
	string hexa("0123456789ABCDEF");
	string output("");
	
	if (N==0)
		return string("00");
	
	N=max(0,N);
	N=min(N,255);
	
	output.append(1,hexa.at((N-N%16)/16));
	output.append(1,hexa.at(N%16));
	
	return output;

}

int scalaAVR(double porcentaje, int color)
{
	if(color==1) // Rojo
	{
		if(porcentaje <= 0.5)
			return 0;
		else
			return int((porcentaje-0.5)*2*255);
	}
	else if(color==2) //verde
	{
		if(porcentaje <= 0.5)
			return int(porcentaje*2*255);
		else
			return int(255 - (porcentaje-0.5)*2*255);
	}
	else if(color==3) //azul
	{
		if(porcentaje <= 0.5)
			return int(255-porcentaje*2*255);
		else
			return 0;
	}		
}

}