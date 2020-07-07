import csv
import utm
from geometry import *


class Stop:


    def __init__(self, lat, lon, text):
        self.code = text
        self.latitude = float(lat)
        self.longitude = float(lon)
        self.pos = Vector3D(-1,-1,-1)

        tmp = utm.from_latlon(float(lat), float(lon))

        self.pos.x = tmp[0]
        self.pos.y = tmp[1]

class Shape:
    nodes = []
    code = "-"

    def insertNode(self,lat,lon):
        u = utm.from_latlon(lat, lon)
        pos = Vector3D(float(u[0]),float(u[1]),-1)
        self.nodes.append(pos)

    def getDistances(self,node):
        nodeIni = self.nodes[0]
        nodeEnd = self.nodes[1]

        projection = pointProjectToSegment(node, nodeIni, nodeEnd)
        distToRoute = distPoint2Point( projection, node)
        distAcu = 0.0
        distEnRuta_ = 0.0

aqui voy....
        it = 0
        for it in range(0,len(self.nodes)):
            print (it)


        print(projection.x,projection.y, distToRoute)


def readStops(filename,stops):
    file = csv.reader(open(filename, 'r'), delimiter=',')
    nline=0
    for row in file:
        if nline != 0:
            stops.append(Stop(row[1],row[0],row[2]))
        nline=nline+1

def readShape(filename,shape):
    file = csv.reader(open(filename, 'r'), delimiter=',')
    nline=0
    for row in file:
        if nline != 0:
            shape.insertNode(float(row[1]),float(row[0]))
        nline=nline+1


def buildStopSequence(stops,shape):

    i=0
    for stop in stops:
        if i == 0:
            shape.getDistances(stop.pos)
        i = i + 1
        #print(stop.pos.x)

if __name__ == "__main__":
    stops = []
    shape = Shape()

    readStops("formales_e1.csv",stops)
    readShape("101_R.csv", shape)

    buildStopSequence(stops, shape)

    #for node in shape.nodes:
    #    print(node.x,node.y)

    #for stop in stops:
    #    print(stop.pos.x,stop.pos.y)

