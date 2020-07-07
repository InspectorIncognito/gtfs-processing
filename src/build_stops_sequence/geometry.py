import math

class Vector3D:

        def __init__(self, x,y,z):
                self.x = x
                self.y = y
                self.z = z

class Segment:

        def __init__(self,v1,v2):
                self.v1 = v1
                self.v2 = v2
				
def magnitude(v):
        return math.sqrt( (v.x * v.x) + (v.y * v.y) + (v.z * v.z) )

def cross(v1,v2):
        return Vector3D((v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z),(v1.x * v2.y) - (v1.y * v2.x))

def dot(v1,v2):
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z

def normalize(v):
        m = magnitude(v)

        if m == 0:
                print("Error : Can't obtain normalize vector :",v.x,v.y,v.z)
                exit()
        else:
                return Vector3D(v.x / m, v.y / m, v.z / m)

def normal(v1,v2,v3):
        u = Vector3D(v3.x - v1.x, v3.y - v1.y,v3.z - v1.z)
        v = Vector3D(v2.x - v1.x, v2.y - v1.y,v2.z - v1.z)
        c = cross(u,v)
        return normalize(c)

def perp2D(u, v):
        return u.x * v.y - u.y * v.x

def deg2rad(deg):
        return deg * PI / 180.0

def rad2deg(rad):
        return rad * 180.0 / PI

def getAngle(v1,v2):
        return math.acosf(float((dot(v1,v2)) / (magnitude(v1)*magnitude(v2))))

def compare(v1,v2):
        return v1.x == v2.x and v1.y == v2.y and v1.z == v2.z

def distPoint2Point(p1,p2):
        return magnitude(Vector3D(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z))

def distPointSegment(p, v1, v2):
        v = Vector3D(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z)
        w = Vector3D(p.x - v1.x, p.y - v1.y, p.z - v1.z)

        c1 = dot( w , v )

        if c1 <= 0 :
                t = dot( w, w)
                return math.sqrt(t)

        c2 = dot( v, v)

        if c2 <= c1:
                d = Vector3D(p.x - v2.x, p.y - v2.y, p.z - v2.z)
                t = dot( d, d)
                return math.sqrt(t)

        b = c1 / c2

        pb = Vector3D(v1.x + b * v.x, v1.y + b * v.y, v1.z + b * v.z)

        dif = Vector3D(p.x - pb.x, p.y - pb.y, p.z - pb.z)

        return math.sqrt( dot( dif, dif) )


def pointProjectToSegment(p, v1, v2):
        v = Vector3D(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z)
        w = Vector3D(p.x - v1.x, p.y - v1.y, p.z - v1.z)

        c1 = dot( w, v)

        if c1 <= 0:
                return Vector3D(v1.x, v1.y, v1.z)

        c2 = dot( v, v)

        if c2 <= c1:
                return Vector3D(v2.x, v2.y, v2.z)

        b = c1 / c2

        return Vector3D(v1.x + b * v.x, v1.y + b * v.y, v1.z + b * v.z)

# inSegment(): determine if a point is inside a segment
#    Input:  a point P, and a collinear segment S
#    Return: 1 = P is inside S
#            0 = P is not inside S
def inSegment( p , s ):

    if s.v1.x != s.v2.x:        # S is not vertical
        if s.v1.x <= p.x and p.x <= s.v2.x :
            return 1
        if s.v1.x >= p.x and p.x >= s.v2.x :
            return 1

    else:                       # S is vertical, so test y coordinate
        if s.v1.y <= p.y and p.y <= s.v2.y :
            return 1
        if s.v1.y >= p.y and p.y >= s.v2.y :
            return 1

    return 0


# intersect2D_2Segments(): the intersection of 2 finite 2D segments
#    Input:  two finite segments S1 and S2
#    Output: *I0 = intersect point (when it exists)
#            *I1 = endpoint of intersect segment [I0,I1] (when it exists)
#    Return: 0=disjoint (no intersect)
#            1=intersect in unique point I0
#            2=overlap in segment from I0 to I1
def intersect2D_Segments( s1, s2, I0, I1 ) :
        output = []
        u = Vector3D(s1.v2.x - s1.v1.x, s1.v2.y - s1.v1.y, s1.v2.z - s1.v1.z)
        v = Vector3D(s2.v2.x - s2.v1.x, s2.v2.y - s2.v1.y, s2.v2.z - s2.v1.z)
        w = Vector3D(s1.v1.x - s2.v1.x, s1.v1.y - s2.v1.y, s1.v1.z - s2.v1.z)

        D = perp2D(u,v)

        # test if they are parallel (includes either being a point)
        if abs(D) < 0.00000001 :           # S1 and S2 are parallel
                if perp2D(u,w) != 0 or perp2D(v,w) != 0 :
                        return 0                   # they are NOT collinear

                # they are collinear or degenerate
                # check if they are degenerate points
                du = dot(u,u)
                dv = dot(v,v)
                if du==0 and dv==0 :                            # both segments are points
                        if not compare(s1.v1,s2.v1) :           # they are distinct points
                                return 0
                        I0 = s1.v1                              # they are the same point
                        return 1

                if du==0 :                    # S1 is a single point
                        if inSegment(s1.v1, s2) == 0 : # but is not in S2
                                return 0
                        I0 = s1.v1
                        return 1

                if dv==0 :                    # S2 a single point
                        if inSegment(s2.v1, s1) == 0:  # but is not in S1
                                return 0
                        I0 = s2.v1
                        return 1

                # they are collinear segments - get overlap (or not)
                t0 = 0.0
                t1 = 0.0                   # endpoints of S1 in eqn for S2
                w2 = Vector3D(s1.v2.x - s2.v1.x, s1.v2.y - s2.v1.y, s1.v2.z - s2.v1.z)

                if v.x != 0 :
                        t0 = w.x / v.x
                        t1 = w2.x / v.x
                else:
                        t0 = w.y / v.y
                        t1 = w2.y / v.y

                if t0 > t1:                   # must have t0 smaller than t1
                        t=t0
                        t0=t1
                        t1=t    # swap if not

                if t0 > 1 or t1 < 0:
                        return 0     # NO overlap

                if t0 < 0 :             # clip to min 0
                        t0 = 0

                if t1 > 1 :             # clip to max 1
                        t1 = 1

                if t0 == t1:            # intersect is a point
                        I0 = Vector3D(s2.v1.x + t0*v.x, s2.v1.y + t0*v.y, s2.v1.z + t0*v.z)
                        return 1


                # they overlap in a valid subsegment
                I0 = Vector3D(s2.v1.x + t0*v.x, s2.v1.y + t0*v.y, s2.v1.z + t0*v.z)
                I1 = Vector3D(s2.v1.x + t1*v.x, s2.v1.y + t1*v.y, s2.v1.z + t1*v.z)
                return 2


        #the segments are skew and may intersect in a point
        #get the intersect parameter for S1
        sI = perp2D(v,w) / D
        if sI < 0 or sI > 1 :               # no intersect with S1
                return 0

        # get the intersect parameter for S2
        tI = perp2D(u,w) / D
        if tI < 0 or tI > 1 :               # no intersect with S2
                return 0

        I0 = Vector3D(s1.v1.x + sI*u.x, s1.v1.y + sI*u.y, s1.v1.z + sI*u.z)
        return 1

# intersect2D_2SegmentsEfficient(): the intersection of 2 finite 2D segments
#    Input:  two finite segments S1 and S2
#    Return: 0=disjoint (no intersect)
#            1=intersect in unique point I0
#            2=overlap in segment from I0 to I1
def intersect2D_SegmentsEfficient( s1, s2) :
        u = Vector3D(s1.v2.x - s1.v1.x, s1.v2.y - s1.v1.y, s1.v2.z - s1.v1.z)
        v = Vector3D(s2.v2.x - s2.v1.x, s2.v2.y - s2.v1.y, s2.v2.z - s2.v1.z)
        w = Vector3D(s1.v1.x - s2.v1.x, s1.v1.y - s2.v1.y, s1.v1.z - s2.v1.z)

        D = perp2D(u,v)

        # test if they are parallel (includes either being a point)
        if abs(D) < 0.00000001 :           # S1 and S2 are parallel
                if perp2D(u,w) != 0 or perp2D(v,w) != 0 :
                        return 0                   # they are NOT collinear

                # they are collinear or degenerate
                # check if they are degenerate points
                du = dot(u,u)
                dv = dot(v,v)
                if du==0 and dv==0 :                            # both segments are points
                        if not compare(s1.v1,s2.v1) :           # they are distinct points
                                return 0
                        return 1

                if du==0 :                    # S1 is a single point
                        if inSegment(s1.v1, s2) == 0 : # but is not in S2
                                return 0
                        return 1

                if dv==0 :                    # S2 a single point
                        if inSegment(s2.v1, s1) == 0:  # but is not in S1
                                return 0
                        return 1

                # they are collinear segments - get overlap (or not)
                t0 = 0.0
                t1 = 0.0                   # endpoints of S1 in eqn for S2
                w2 = Vector3D(s1.v2.x - s2.v1.x, s1.v2.y - s2.v1.y, s1.v2.z - s2.v1.z)

                if v.x != 0 :
                        t0 = w.x / v.x
                        t1 = w2.x / v.x
                else:
                        t0 = w.y / v.y
                        t1 = w2.y / v.y

                if t0 > t1:                   # must have t0 smaller than t1
                        t=t0
                        t0=t1
                        t1=t    # swap if not

                if t0 > 1 or t1 < 0:
                        return 0     # NO overlap

                if t0 < 0 :             # clip to min 0
                        t0 = 0

                if t1 > 1 :             # clip to max 1
                        t1 = 1

                if t0 == t1:            # intersect is a point
                        return 1


                # they overlap in a valid subsegment
                return 2


        #the segments are skew and may intersect in a point
        #get the intersect parameter for S1
        sI = perp2D(v,w) / D
        if sI < 0 or sI > 1 :               # no intersect with S1
                return 0

        # get the intersect parameter for S2
        tI = perp2D(u,w) / D
        if tI < 0 or tI > 1 :               # no intersect with S2
                return 0

        return 1
