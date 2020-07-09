import csv
import utm
import simplekml
import collections
from geometry import *
from visualization import *

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

    def __init__(self, code):
        self.nodes = []
        self.code = "-"

    def insertNode(self,lat,lon):
        u = utm.from_latlon(lat, lon)
        pos = Vector3D(float(u[0]),float(u[1]),-1)
        self.nodes.append(pos)

    def getDistances(self,node):
        nodeIni = self.nodes[0]
        nodeEnd = self.nodes[1]

        projection = pointProjectToSegment(node, nodeIni, nodeEnd)
        distToRoute_ = distPoint2Point( projection, node)
        distAcu = 0.0
        distEnRuta_ = 0.0

        i = 0
        for i in range(0,len(self.nodes)):

            if i+1 ==  len(self.nodes):
                break

            projectAux = pointProjectToSegment(node, self.nodes[i], self.nodes[i+1])

            dist = distPoint2Point( projectAux, node)

            if distToRoute_ >= dist :
                distToRoute_ = dist
                projection = projectAux
                distEnRuta_ = distPoint2Point( self.nodes[i], projection) + distAcu

            distAcu += distPoint2Point( self.nodes[i], self.nodes[i+1])

        return {"dtr" : distToRoute_, "dor" : distEnRuta_}


def readStops(filename,stops):
    file = csv.reader(open(filename, 'r'), delimiter=',')
    nline=0
    for row in file:
        if nline != 0:
            stops.append(Stop(row[4],row[5],row[0]))
        nline=nline+1

def readShapes(filename,shapes):
    file = csv.reader(open(filename, 'r'), delimiter=',')
    nline=0
    id_ant = "*"
    for row in file:
        if nline != 0:
            if id_ant != row[0] :
                shape = Shape(row[0])
                shape.insertNode(float(row[1]), float(row[2]))
                shapes[ row[0] ] = shape
            else:
                shapes[ row[0] ].insertNode(float(row[1]),float(row[2]))
            id_ant = row[0]
        nline=nline+1


def buildStopSequence(stops,shapes):

    stopSequence = dict()

    for stop in stops:
        for key in shapes:
            #i=0
            distances = shapes[key].getDistances(stop.pos)

            if distances["dtr"] < 10 :
                if key not in stopSequence :
                    stopSequence[key] = {}

                #stopSequence[key].append( ( distances["dor"] : stop ) )
                stopSequence[key][distances["dor"]] = stop

    for key in stopSequence :
        od = collections.OrderedDict(sorted(stopSequence[key].items()))
        stopSequence[key] = od


    return stopSequence



if __name__ == "__main__":
    stops = []
    shapes = dict()
    #shape = Shape()

    readStops("stops.txt",stops)
    readShapes("shapes.txt", shapes)


    stopSequences = buildStopSequence(stops, shapes)

    #for key in stopSequences :
    #    for k, v in stopSequences[key].items():
    #        print (key,k, v.code)

    #i=0
    #for node in shapes['A_I'].nodes:
    #    u = utm.to_latlon(node.x, node.y, 19, 'K')
    #    print(i, 'A_I', node.x, node.y, u[0], u[1])
    #    i=i+1


    #for key in shapes:
    #    i=0
    #    for node in shapes[key].nodes:
    #        print(i, key, node.x, node.y)
    #        i=i+1

    kml = simplekml.Kml()

    drawStops(kml,stops)
    drawShapes(kml, shapes)
    drawStopSequence(kml, stopSequences)



    kml.save("vista.kml")

    #for node in shape.nodes:
    #    print(node.x,node.y)

    #for stop in stops:
    #    print(stop.pos.x,stop.pos.y)

