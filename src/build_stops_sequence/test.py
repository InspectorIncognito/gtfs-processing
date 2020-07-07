from geometry import *

p1 = Vector3D(1.0,3.0,0.0)
p2 = Vector3D(5.0,3.0,0.0)
p3 = Vector3D(4.0,1.0,0.0)
p4 = Vector3D(7.0,4.0,0.0)
p5 = Vector3D(5.0,2.0,0.0)
p6 = Vector3D(3.0,4.0,0.0)
p7 = Vector3D(6.0,7.0,0.0)
p8 = Vector3D(2.0,5.0,0.0)
p9 = Vector3D(5.0,9.0,0.0)
p10 = Vector3D(2.0,10.0,0.0)
p11 = Vector3D(4.0,10.0,0.0)
p12 = Vector3D(7.0,3.0,0.0)
p13 = Vector3D(3.0,9.0,0.0)
p14 = Vector3D(3.0,0.0,0.0)


s1 = Segment(p1,p2)
s2 = Segment(p3,p4)
s3 = Segment(p8,p5)
s4 = Segment(p6,p7)
s5 = Segment(p8,p9)
s6 = Segment(p10,p11)
s7 = Segment(p12,p13)
s8 = Segment(p14,p5)

I0 = Vector3D(0.0,0.0,0.0)
I1 = Vector3D(0.0,0.0,0.0)

if intersect2D_SegmentsEfficient( s1, s3 ) == 1:
    print ("test 1 : OK")
else:
    print ("test 1 : FAIL")

if intersect2D_SegmentsEfficient( s1, s2 ) == 0:
    print ("test 2 : OK")
else:
    print ("test 2 : FAIL")

if intersect2D_SegmentsEfficient( s3, s2 ) == 1:
    print ("test 3 : OK")
else:
    print ("test 3 : FAIL")

if intersect2D_SegmentsEfficient( s7, s5 ) == 1:
    print ("test 4 : OK")
else:
    print ("test 4 : FAIL")

if intersect2D_SegmentsEfficient( s7, s4 ) == 1:
    print ("test 5 : OK")
else:
    print ("test 5 : FAIL")

if intersect2D_SegmentsEfficient( s7, s2 ) == 1:
    print ("test 6 : OK")
else:
    print ("test 6 : FAIL")

if intersect2D_SegmentsEfficient( s7, s3 ) == 0:
    print ("test 7 : OK")
else:
    print ("test 7 : FAIL")

if intersect2D_SegmentsEfficient( s7, s1 ) == 0:
    print ("test 8 : OK")
else:
    print ("test 8 : FAIL")

if intersect2D_SegmentsEfficient( s7, s6 ) == 0:
    print ("test 9 : OK")
else:
    print ("test 9 : FAIL")

if intersect2D_SegmentsEfficient( s4, s3 ) == 1:
    print ("test 10 : OK")
else:
    print ("test 10 : FAIL")

if intersect2D_SegmentsEfficient( s5, s3 ) == 1:
    print ("test 11 : OK")
else:
    print ("test 11 : FAIL")

if intersect2D_SegmentsEfficient( s6, s1 ) == 0:
    print ("test 12 : OK")
else:
    print ("test 12 : FAIL")

if intersect2D_SegmentsEfficient( s6, s2 ) == 0:
    print ("test 13 : OK")
else:
    print ("test 13 : FAIL")

if intersect2D_SegmentsEfficient( s8, s2 ) == 2:
    print ("test 14 (intersection segment) : OK")
else:
    print ("test 14 (intersection segment) : FAIL")

#p1 = Vector3D(2,1,0.0)
#v1 = Vector3D(0.0,0.0,0.0)
#v2 = Vector3D(1.0,0.0,0.0)
#j = 2
#d = 3

#print(distPointSegment(p1,v1,v2))
#c = pointProjectToSegment(p,v1,v2)
#print (c.x,c.y,c.z)

#print ( inSegment(j,d) )