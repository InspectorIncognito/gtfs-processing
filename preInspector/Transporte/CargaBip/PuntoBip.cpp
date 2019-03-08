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
#include "PuntoBip.h"

PuntoBip::PuntoBip(void)
{
	this->x=-1;
	this->y=-1;
	this->nombre = string("-");

}

PuntoBip::~PuntoBip(void)
{
}

PuntoBip::PuntoBip(int id_,string nombre_, double lat_, double lon_,int x_, int y_)
{
	this->id = id_;
	this->lat = lat_;
	this->lon = lon_;
	this->x=x_;
	this->y=y_;
	this->nombre = nombre_;
}
