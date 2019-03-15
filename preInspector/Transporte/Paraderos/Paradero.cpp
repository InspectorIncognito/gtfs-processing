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
#include "Paradero.h"

Paradero::Paradero(void)
{
	this->x=-1;
	this->y=-1;
	this->codigo = string("-");
	this->nombre = string("-");

}

Paradero::~Paradero(void)
{
}

Paradero::Paradero(double lat_, double lon_, int x_,int y_,string codigo_,string nombre_)
{
	this->lat = lat_;
	this->lon = lon_;
	this->x=x_;
	this->y=y_;
	this->codigo = codigo_;
	this->nombre = nombre_;
}
