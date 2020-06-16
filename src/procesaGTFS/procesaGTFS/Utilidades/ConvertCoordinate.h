/**
* Project: "TransData".
* Package:
* Class:
* Created by: Mauricio Zuñiga G. <mzuniga@pragxis.cl> <mzuniga@gmail.com>
//converts UTM coords to lat/long.  Equations from USGS Bulletin 1532
//East Longitudes are positive, West longitudes are negative.
//North latitudes are positive, South latitudes are negative
//Lat and Long are in decimal degrees.
//Written by Chuck Gantz- chuck.gantz@globalstar.com
* Last modified : Mauricio Zuñiga 01-04-2013
*/
#ifndef CONVERTCOORDINATE_H
#define CONVERTCOORDINATE_H

#include <math.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>

//#define UTMZONE "19H"

using namespace std;

namespace ConvertCoordinate {

	void LLtoUTM(int ReferenceEllipsoid, const double Lat, const double Long, double &UTMNorthing, double &UTMEasting, string& UTMZone);

	void UTMtoLL(int ReferenceEllipsoid, const double UTMNorthing, const double UTMEasting, string UTMZone, double& Lat, double& Long);

	char UTMLetterDesignator(double Lat);

}

#endif