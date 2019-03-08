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

#include <math.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;


/**
* Title: Color Scale
* Description:	Clase para el manejo de colores
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga G.
* @version 0.0
*/
namespace ColorScale{

	string int2ColorHex(double min,double max,double delta,double valor);
	int int2ColorInt(double min,double max,double delta,double valor);
	int scalaAVR(double porcentaje, int color);				//Escala que va del Azul al Rojo pasando por el verde
	string int2Hex(int N);

}

