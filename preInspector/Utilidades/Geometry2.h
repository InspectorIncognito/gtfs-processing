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

#include <vector>
#include <math.h>
#include <iostream>

#define SMALL_NUM  0.00000001 // anything that avoids division overflow

using namespace std;

/**
* Title: Vector3D
* Description:	Clase que modela un vector de tres dimensiones
* Copyright:  Pragxis (c) 2016
* Date: 18-06-2016
* @author Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* @version 0.0
*/
class Vector3D{
public:
	Vector3D();
	~Vector3D();
	Vector3D(double x, double y, double z);
	double x;
	double y;
	double z;
	bool Compare(Vector3D v);
};

/**
* Title: Segmento
* Description:	Clase que modela un segmento, como par de vector
* Copyright:  Pragxis (c) 2013
* Date: 15-04-2013
* @author Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* @version 0.0
*/
class Segmento{
	public:
		Segmento();
		Segmento(Vector3D u1,Vector3D u2);
		~Segmento();
		Vector3D v1;
		Vector3D v2;

	
		
};

/**
* Title: Geometry
* Description:	Clase contenedora de variados metodos de calculos geometricos
* Copyright:  Pragxis (c) 2013
* Date: 15-04-2013
* @author Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* @version 0.0
*/
class Geometry{
public:
	Geometry();
	
	#define PI 3.14159265

	static double Magnitude(Vector3D *v);
	static Vector3D Normal(Vector3D *v1,Vector3D *v2, Vector3D *v3);
	static Vector3D Normalize(Vector3D *v1);
	static double Dot(Vector3D *v1,Vector3D *v2);
	static Vector3D Cross(Vector3D *v1,Vector3D *v2);
	static double DistPointSegment(Vector3D *p, Vector3D *v1, Vector3D *v2);
	static Vector3D PointProjectToSegment(Vector3D *p, Vector3D *v1, Vector3D *v2);
	static double DistPoint2Point(Vector3D *p1,Vector3D *p2);
	static double DistPoint2Point2D(double x1,double y1, double x2, double y2);
	static double GetAngle(Vector3D *v1,Vector3D *v2);
	static int poly_simplify( double tol, vector<Vector3D> *V, vector<Vector3D> *sV );
	static void simplifyDP( double tol, vector<Vector3D> *v, int j, int k, vector<int> *mk );
	static double PolyTramado( vector<Vector3D> *V,double distance,vector<Vector3D> *tV);
	static double PolyTramadoAndNormal( vector<Vector3D> *V,
					double distance,
					vector<Vector3D> *tV,
					vector<Vector3D> *nV,
					vector<Segmento> *tS,
					vector<int>	 *iS);
	static Vector3D NormalSegment(Vector3D *v1,Vector3D *v2);
	static int intersect2D_Segments( Segmento S1, Segmento S2, Vector3D *I0, Vector3D *I1 );
	static int inSegment( Vector3D P, Segmento S);
	static double Perp2D(Vector3D *u, Vector3D *v);
	static double deg2rad(double deg);
	static double rad2deg(double rad);

	~Geometry();

};

