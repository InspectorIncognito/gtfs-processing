/***************************************************************************
 *   Copyright (C) 2009 by Mauricio Zuñiga   *
 *   mzuniga@gmail.com   *
 ***************************************************************************/
#include "Geometry2.h"

Geometry::Geometry()
{
}

Geometry::~Geometry()
{
}

double Geometry::Magnitude(Vector3D *v)
{
	return sqrt( (v->x * v->x) + (v->y * v->y) + (v->z * v->z) );
}

Vector3D Geometry::Normal(Vector3D *v1,Vector3D *v2, Vector3D *v3)
{
	Vector3D u = Vector3D(v3->x - v1->x,v3->y - v1->y,v3->z - v1->z);
	Vector3D v = Vector3D(v2->x - v1->x,v2->y - v1->y,v2->z - v1->z);

	Vector3D c = Cross(&u,&v);
	
	return Normalize(&c);
}

Vector3D Geometry::Normalize(Vector3D *v1)
{
	double mag = Magnitude(v1);

	return Vector3D(v1->x/mag, v1->y/mag, v1->z/mag);
}

double Geometry::Dot(Vector3D *v1,Vector3D *v2)
{
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

Vector3D Geometry::Cross(Vector3D *v1,Vector3D *v2)
{
	return Vector3D((v1->y * v2->z) - (v1->z * v2->y),(v1->z * v2->x) - (v1->x * v2->z),(v1->x * v2->y) - (v1->y * v2->x));
}

double Geometry::DistPointSegment(Vector3D *p, Vector3D *v1, Vector3D *v2)
{
	Vector3D v = Vector3D(v2->x-v1->x,v2->y-v1->y,v2->z-v1->z);
	Vector3D w = Vector3D(p->x-v1->x,p->y-v1->y,p->z-v1->z);
	
	double c1 = Dot(&w,&v);
	
	if(c1 <= 0)
	{
		double t = Dot(&w,&w);
		return sqrt(t);
	}
	
	double c2 = Dot(&v,&v);
	
	if(c2 <= c1)
	{
		Vector3D dif = Vector3D(p->x-v2->x,p->y-v2->y,p->z-v2->z);
		double t = Dot(&dif,&dif);
		return sqrt(t);
	}
	
	double b = c1/c2;
	
	Vector3D Pb = Vector3D(v1->x + b*v.x, v1->y + b*v.y, v1->z + b*v.z);
	
	Vector3D dif = Vector3D(p->x - Pb.x, p->y - Pb.y, p->z - Pb.z);
	
	double t = Dot(&dif,&dif);
	
	return sqrt(t);
}

Vector3D Geometry::PointProjectToSegment(Vector3D *p, Vector3D *v1, Vector3D *v2)
{
	Vector3D v = Vector3D(v2->x-v1->x,v2->y-v1->y,v2->z-v1->z);
	Vector3D w = Vector3D(p->x-v1->x,p->y-v1->y,p->z-v1->z);
	
	double c1 = Dot(&w,&v);
	
	if(c1 <= 0)
	{
		double t = Dot(&w,&w);
		return Vector3D(v1->x,v1->y,v1->z);
	}
	
	double c2 = Dot(&v,&v);
	
	if(c2 <= c1)
	{
		Vector3D dif = Vector3D(p->x-v2->x,p->y-v2->y,p->z-v2->z);
		double t = Dot(&dif,&dif);
		return Vector3D(v2->x,v2->y,v2->z);
	}
	
	double b = c1/c2;
	
	return Vector3D(v1->x + b*v.x,v1->y + b*v.y,v1->z + b*v.z);
}

double Geometry::DistPoint2Point(Vector3D *p1,Vector3D *p2)
{
	Vector3D vv = Vector3D(p1->x-p2->x,p1->y-p2->y,p1->z-p2->z);
	return Magnitude(&vv);
}

double Geometry::DistPoint2Point2D(double x1,double y1, double x2, double y2)
{
	Vector3D vv = Vector3D(x1-x2,y1-y2,0);
	return Magnitude(&vv);
}

double Geometry::GetAngle(Vector3D *v1,Vector3D *v2)
{
	return acosf((Dot(v1,v2)) / (Magnitude(v1)*Magnitude(v2)));
}

Vector3D Geometry::NormalSegment(Vector3D *v1,Vector3D *v2)
{
	Vector3D vv = Vector3D((v2->y-v1->y),-(v2->x-v1->x),0.0);
	return Normalize(&vv);
}

int Geometry::poly_simplify( double tol, vector<Vector3D> *V, vector<Vector3D> *sV )
{
	int    i, k, m, pv;		// misc counters
	double  tol2 = tol * tol;       // tolerance squared
	vector<Vector3D> vt; 		//Point* vt = new Point[n];      // vertex buffer
	vector<int> mk;			//int*   mk = new int[n] = {0};  // marker buffer

 	for(int j=0; j < V->size() ; j++)
		mk.push_back(0);
	
	// STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
	vt.push_back(V->at(0)); // start at the beginning

	for (i=k=1, pv=0; i<V->size(); i++) 
	{
		Vector3D t = Vector3D(V->at(i).x-V->at(pv).x,V->at(i).y-V->at(pv).y,0.0);
		
		if (Dot(&t,&t) < tol2)
			continue;
		
		vt.push_back(V->at(i));
		k++;
		pv = i;
	}
	
	if (pv < V->size()-1)
	{
		vt.push_back(V->at(V->size()-1));      // finish at the end
		k++;
	}
	
	// STAGE 2.  Douglas-Peucker polyline simplification
	mk[0] = mk[k-1] = 1;       // mark the first and last vertices
 	
 	Geometry::simplifyDP( tol, &vt, 0, k-1, &mk );
 	
	// copy marked vertices to the output simplified polyline
 	for (i=m=0; i<k; i++) {
 		if (mk[i]==1)
 		{
 			sV->push_back(vt[i]);
 			m++;
 		}
 	}
	
	return m;         // m vertices in simplified polyline
}

double Geometry::deg2rad(double deg) {
  return (deg * PI / 180.0);
}


double Geometry::rad2deg(double rad) {
  return (rad * 180.0 / PI);
}


/**
 * 
 * @param tol = approximation tolerance
 * @param V  = polyline array of vertex points
 * @param n the number of points in V[]
 * @param sV  = (output) simplified polyline vertices (max is n)
 * @return m = (return) the number of points in sV[]
 */
//int Geometry::poly_simplify( double tol, vector<Vector3D> V, vector<Vector3D> *sV )
//{
//	int    i, k, m, pv;		// misc counters
//	double  tol2 = tol * tol;       // tolerance squared
//	vector<Vector3D> vt; 		//Point* vt = new Point[n];      // vertex buffer
//	vector<int> mk;			//int*   mk = new int[n] = {0};  // marker buffer
//
// 	for(int j=0; j < V.size() ; j++)
//		mk.push_back(0);
//	
//	// STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
//	vt.push_back(V[0]); // start at the beginning
//
//	for (i=k=1, pv=0; i<V.size(); i++) 
//	{
//		Vector3D t = Vector3D(V[i].x-V[pv].x,V[i].y-V[pv].y,0.0);
//		
//		if (Dot(t,t) < tol2)
//			continue;
//		
//		vt.push_back(V[i]);
//		k++;
//		pv = i;
//	}
//	
//	if (pv < V.size()-1)
//	{
//		vt.push_back(V[V.size()-1]);      // finish at the end
//		k++;
//	}
//	
//	// STAGE 2.  Douglas-Peucker polyline simplification
//	mk[0] = mk[k-1] = 1;       // mark the first and last vertices
// 	
// 	Geometry::simplifyDP( tol, &vt, 0, k-1, &mk );
// 	
//	// copy marked vertices to the output simplified polyline
// 	for (i=m=0; i<k; i++) {
// 		if (mk[i]==1)
// 		{
// 			sV->push_back(vt[i]);
// 			m++;
// 		}
// 	}
//	
//	return m;         // m vertices in simplified polyline
//}

/**
 * This is the Douglas-Peucker recursive simplification routine
 * It just marks vertices that are part of the simplified polyline
 * for approximating the polyline subchain v[j] to v[k].
 * @param tol = tolerancia aproximada
 * @param v = polilinea arreglo de vertices 
 * @param j indices de la sub cadena v[j] a v[k]
 * @param k indices de la sub cadena v[j] a v[k]
 * @param mk salida: arrelgo de vertices marcados
 */
void Geometry::simplifyDP( double tol, vector<Vector3D> *v, int j, int k, vector<int> *mk )
{
	if (k <= j+1) // there is nothing to simplify
		return;
	
	// check for adequate approximation by segment S from v[j] to v[k]
	int    maxi = j;          // index of vertex farthest from S
	double maxd2 = 0.0;         // distance squared of farthest vertex
	double tol2 = tol * tol;  // tolerance squared
 
	Vector3D  u = Vector3D(v->at(k).x - v->at(j).x, v->at(k).y - v->at(j).y,0.0);   // segment direction vector
	double  cu = Dot(&u,&u);     // segment length squared
	
	// test each vertex v[i] for max distance from S
	// compute using the Feb 2001 Algorithm's dist_Point_to_Segment()
	// Note: this works in any dimension (2D, 3D, ...)
	Vector3D  w;
	Vector3D  Pb;                // base of perpendicular from v[i] to S
	double  b, cw, dv2;        // dv2 = distance v[i] to S squared
	
	for (int i=j+1; i<k; i++)
	{
		// compute distance squared
		dv2 = Geometry::DistPointSegment(&(v->at(i)), &(v->at(j)), &(v->at(k)));
 		// test with current max distance squared
 		if (dv2 <= maxd2)
 		continue;
 		// v[i] is a new max vertex
 		maxi = i;
 		maxd2 = dv2;
 	}
 	if (maxd2 > tol2)        // error is worse than the tolerance
 	{
 		// split the polyline at the farthest vertex from S
 		mk->at(maxi) = 1;      // mark v[maxi] for the simplified polyline
 		// recursively simplify the two subpolylines at v[maxi]
 		simplifyDP( tol, v, j, maxi, mk );  // polyline v[j] to v[maxi]
 		simplifyDP( tol, v, maxi, k, mk );  // polyline v[maxi] to v[k]
 	}
 	// else the approximation is OK, so ignore intermediate vertices
	return;
}

double Geometry::PolyTramadoAndNormal( vector<Vector3D> *V,
					double distance,
					vector<Vector3D> *tV,
					vector<Vector3D> *nV,
					vector<Segmento> *tS,
					vector<int>	 *iS)
{
	vector<Vector3D> S;
	vector<Vector3D> N;
	vector<Segmento> segments;
	vector<int> id;
	double distAcumulada=0;
	int is=0;
	
	//El primero punto es el mismo
	S.push_back(V->at(0));
	N.push_back(NormalSegment(&(V->at(0)),&(V->at(1))));
	int s=0;
	int v=1;
	
	while(v < V->size())
	{
		Vector3D t1 = Vector3D(V->at(v).x - S[s].x,V->at(v).y - S[s].y,0.0);
	
		if(Geometry::Magnitude(&t1) > distance)
		{
			Vector3D dir = Normalize(&t1);
			dir.x = distance*dir.x;
			dir.y = distance*dir.y;
			dir.z = distance*dir.z;
	
			Vector3D np = Vector3D(S[s].x+dir.x,S[s].y+dir.y,S[s].z+dir.z);
			
			S.push_back(np);
			N.push_back(NormalSegment(&(V->at(v)),&(V->at(v-1))));

			//Almacenamiento de todos los segmentos, identificando los que pertenecen a cada tramo
			segments.push_back(Segmento(S[s],np));
			id.push_back(is);
 			is++;

			s++;
	

		}
		else if(Geometry::Magnitude(&t1) == distance)
		{
			S.push_back(V->at(v));
			N.push_back(NormalSegment(&(V->at(v)),&(V->at(v-1))));

			//Almacenamiento de todos los segmentos, identificando los que pertenecen a cada tramo
			segments.push_back(Segmento(S[s],V->at(v)));
			id.push_back(is);
			is++;

			v++;
			s++;
			//Falta caso en que coincida en el ultimo punto.

		}
		else
		{

			//Almacenamiento de todos los segmentos, identificando los que pertenecen a cada tramo
			segments.push_back(Segmento(S[s],V->at(v)));
			id.push_back(is);
// 			is++;

			int n=0;
			double dist_acu = Geometry::Magnitude(&t1);
			while(dist_acu < distance && v < V->size())
			{
				Vector3D t3 = Vector3D(V->at(v+1).x - V->at(v).x,V->at(v+1).y - V->at(v).y,0.0);
				dist_acu+=Geometry::Magnitude(&t3);

				//Almacenamiento de todos los segmentos, identificando los que pertenecen a cada tramo
				segments.push_back(Segmento(V->at(v),V->at(v+1)));
				id.push_back(is);

				v++;
				n++;
			}
			
			if(dist_acu == distance)
			{

				S.push_back(V->at(v+1));
				N.push_back(NormalSegment(&(V->at(v+1)),&(V->at(v))));

				//Almacenamiento de todos los segmentos, identificando los que pertenecen a cada tramo
				segments.push_back(Segmento(V->at(v),V->at(v+1)));
				id.push_back(is);
				is++;

				s++;
				v++;
				
				continue;
			}
			if(v==V->size())
			{

				S.push_back(V->at(v-1));
				N.push_back(NormalSegment(&(V->at(v-1)),&(V->at(v-2))));

				//Almacenamiento de todos los segmentos, identificando los que pertenecen a cada tramo
// 				segments.push_back(Segmento(V[v-1],V[v-2]));
// 				id.push_back(is);
// 				is++;

			segments.pop_back();
			id.pop_back();

				s++;
				v++;
				continue;
			}
		
			Vector3D t2 = Vector3D(V->at(v).x - V->at(v-1).x,V->at(v).y - V->at(v-1).y,0.0);

			dist_acu-=Geometry::Magnitude(&t2);

			//Almacenamiento de todos los segmentos, identificando los que pertenecen a cada tramo
			segments.pop_back();
			id.pop_back();

			Vector3D dir = Normalize(&t2);
			
			dir.x = (distance-dist_acu)*dir.x;
			dir.y = (distance-dist_acu)*dir.y;
			dir.z = (distance-dist_acu)*dir.z;

			Vector3D np = Vector3D(V->at(v-1).x+dir.x,V->at(v-1).y+dir.y,V->at(v-1).z+dir.z);
		
			S.push_back(np);
			N.push_back(NormalSegment(&(V->at(v)),&(V->at(v-1))));

			//Almacenamiento de todos los segmentos, identificando los que pertenecen a cada tramo
			segments.push_back(Segmento(V->at(v-1),np));
			id.push_back(is);
			is++;

			s++;
		}
	}
		
	// copy marked vertices to the output simplified polyline
	for (int i=0; i<S.size(); i++) 
	{
		tV->push_back(S[i]);
		nV->push_back(N[i]);
// 		cout << nV->at(i).x << "|" << nV->at(i).y << "\n";
	}	
	
	for (int i=0; i<segments.size(); i++) 
	{
		tS->push_back(segments[i]);
		iS->push_back(id[i]);	
	}
	return distAcumulada;
	
}

double Geometry::PolyTramado( vector<Vector3D> *V,double distance,vector<Vector3D> *tV)
{
	vector<Vector3D> S;
	double distAcumulada=0;
	
	//El primero punto es el mismo
	S.push_back(V->at(0));
	int s=0;
	int v=1;
	
	while(v < V->size())
	{
		Vector3D t1 = Vector3D(V->at(v).x - S[s].x,V->at(v).y - S[s].y,0.0);
	
		if(Geometry::Magnitude(&t1) > distance)
		{
			Vector3D dir = Normalize(&t1);
			dir.x = distance*dir.x;
			dir.y = distance*dir.y;
			dir.z = distance*dir.z;
	
			Vector3D np = Vector3D(S[s].x+dir.x,S[s].y+dir.y,S[s].z+dir.z);
			
			S.push_back(np);
			s++;
		}
		else if(Geometry::Magnitude(&t1) == distance)
		{
			S.push_back(V->at(v));
			v++;
			s++;
			//Falta caso en que coincida en el ultimo punto.
		}
		else
		{
			double dist_acu = Geometry::Magnitude(&t1);
			while(dist_acu < distance && v < V->size())
			{
				Vector3D t3 = Vector3D(V->at(v+1).x - V->at(v).x,V->at(v+1).y - V->at(v).y,0.0);
				dist_acu+=Geometry::Magnitude(&t3);
// 				cout << dist_acu << "|" << distance << "|" << v << "\n";
				v++;
			}
			
			if(dist_acu == distance)
			{
				S.push_back(V->at(v+1));
				s++;
				v++;
				continue;
			}
			if(v==V->size())
			{
				S.push_back(V->at(v-1));
				s++;
				v++;
				continue;
			}
			
			Vector3D t2 = Vector3D(V->at(v).x - V->at(v-1).x,V->at(v).y - V->at(v-1).y,0.0);

			dist_acu-=Geometry::Magnitude(&t2);

			Vector3D dir = Normalize(&t2);
			
			dir.x = (distance-dist_acu)*dir.x;
			dir.y = (distance-dist_acu)*dir.y;
			dir.z = (distance-dist_acu)*dir.z;

			Vector3D np = Vector3D(V->at(v-1).x+dir.x,V->at(v-1).y+dir.y,V->at(v-1).z+dir.z);
		
			S.push_back(np);
			s++;
		}
	}
		
	// copy marked vertices to the output simplified polyline
	for (int i=0; i<S.size(); i++) 
	{
		tV->push_back(S[i]);
	}	
	
	return distAcumulada;
	
}

double Geometry::Perp2D(Vector3D *u, Vector3D *v)
{
	return (u->x*v->y - u->y*v->x);
}

// intersect2D_2Segments(): the intersection of 2 finite 2D segments
//    Input:  two finite segments S1 and S2
//    Output: *I0 = intersect point (when it exists)
//            *I1 = endpoint of intersect segment [I0,I1] (when it exists)
//    Return: 0=disjoint (no intersect)
//            1=intersect in unique point I0
//            2=overlap in segment from I0 to I1
int Geometry::intersect2D_Segments( Segmento S1, Segmento S2, Vector3D *I0, Vector3D *I1 )
{
	Vector3D u = Vector3D(S1.v2.x - S1.v1.x,S1.v2.y - S1.v1.y,S1.v2.z - S1.v1.z);
	Vector3D v = Vector3D(S2.v2.x - S2.v1.x,S2.v2.y - S2.v1.y,S2.v2.z - S2.v1.z);
	Vector3D w = Vector3D(S1.v1.x - S2.v1.x,S1.v1.y - S2.v1.y,S1.v1.z - S2.v1.z);
 	double     D = Perp2D(&u,&v);

    // test if they are parallel (includes either being a point)
    if (fabs(D) < SMALL_NUM) {          // S1 and S2 are parallel
        if (Perp2D(&u,&w) != 0 || Perp2D(&v,&w) != 0) {
            return 0;                   // they are NOT collinear
        }
        // they are collinear or degenerate
        // check if they are degenerate points
        double du = Dot(&u,&u);
        double dv = Dot(&v,&v);
        if (du==0 && dv==0) {           // both segments are points
            if (!S1.v1.Compare(S2.v1))         // they are distinct points
                return 0;
            *I0 = S1.v1;                // they are the same point
            return 1;
        }
        if (du==0) {                    // S1 is a single point
            if (inSegment(S1.v1, S2) == 0)  // but is not in S2
                return 0;
            *I0 = S1.v1;
            return 1;
        }
        if (dv==0) {                    // S2 a single point
            if (inSegment(S2.v1, S1) == 0)  // but is not in S1
                return 0;
            *I0 = S2.v1;
            return 1;
        }
        // they are collinear segments - get overlap (or not)
        double t0, t1;                   // endpoints of S1 in eqn for S2
	Vector3D w2 = Vector3D(S1.v2.x - S2.v1.x,S1.v2.y - S2.v1.y,S1.v2.z - S2.v1.z);
//         Vector w2 = S1.P1 - S2.P0;
        if (v.x != 0) {
                t0 = w.x / v.x;
                t1 = w2.x / v.x;
        }
        else {
                t0 = w.y / v.y;
                t1 = w2.y / v.y;
        }
        if (t0 > t1) {                  // must have t0 smaller than t1
                double t=t0; t0=t1; t1=t;    // swap if not
        }
        if (t0 > 1 || t1 < 0) {
            return 0;     // NO overlap
        }
        t0 = t0<0? 0 : t0;              // clip to min 0
        t1 = t1>1? 1 : t1;              // clip to max 1
        if (t0 == t1) {                 // intersect is a point
		*I0 = Vector3D(S2.v1.x + t0*v.x,S2.v1.y + t0*v.y,S2.v1.z + t0*v.z);
//             *I0 = S2.P0 + t0 * v;
            return 1;
        }

        // they overlap in a valid subsegment
	*I0 = Vector3D(S2.v1.x + t0*v.x,S2.v1.y + t0*v.y,S2.v1.z + t0*v.z);
	*I1 = Vector3D(S2.v1.x + t1*v.x,S2.v1.y + t1*v.y,S2.v1.z + t1*v.z);
//         *I0 = S2.P0 + t0 * v;
//         *I1 = S2.P0 + t1 * v;
        return 2;
    }

    // the segments are skew and may intersect in a point
    // get the intersect parameter for S1
    double     sI = Perp2D(&v,&w) / D;
    if (sI < 0 || sI > 1)               // no intersect with S1
        return 0;

    // get the intersect parameter for S2
    float     tI = Perp2D(&u,&w) / D;
    if (tI < 0 || tI > 1)               // no intersect with S2
        return 0;

//     *I0 = S1.P0 + sI * u;               // compute S1 intersect point
	*I0 = Vector3D(S1.v1.x + sI*u.x,S1.v1.y + sI*u.y,S1.v1.z + sI*u.z);
    return 1;
}
//===================================================================

// inSegment(): determine if a point is inside a segment
//    Input:  a point P, and a collinear segment S
//    Return: 1 = P is inside S
//            0 = P is not inside S
int Geometry::inSegment( Vector3D P, Segmento S)
{
    if (S.v1.x != S.v2.x) {    // S is not vertical
        if (S.v1.x <= P.x && P.x <= S.v2.x)
            return 1;
        if (S.v1.x >= P.x && P.x >= S.v2.x)
            return 1;
    }
    else {    // S is vertical, so test y coordinate
        if (S.v1.y <= P.y && P.y <= S.v2.y)
            return 1;
        if (S.v1.y >= P.y && P.y >= S.v2.y)
            return 1;
    }
    return 0;
}

bool Vector3D::Compare(Vector3D v)
{
	return this->x == v.x && this->y == v.y && this->z == v.z;
}

Vector3D::Vector3D(double x, double y, double z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}

Vector3D::Vector3D()
{
}

Vector3D::~Vector3D()
{
}

Segmento::Segmento()
{
}

Segmento::Segmento(Vector3D u1, Vector3D u2)
{
	this->v1 = u1;
	this->v2 = u2;
}

Segmento::~Segmento()
{
}

