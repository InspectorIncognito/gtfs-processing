import sys
import csv

class Trip:
    def __init__(self, route_id,service_id,trip_id,trip_headsign,direction_id,shape_id):
        self.route_id = route_id
        self.service_id = service_id
        self.trip_id = trip_id
        self.trip_headsign = trip_headsign
        self.direction_id = direction_id
        self.shape_id = shape_id

def readTrips(path,trips):
    # Lectura de archivo con los datos
    spamReader = csv.reader(open(path+'/trips.txt', 'rb'), delimiter=',', quotechar=' ')

    # Ciclo de lectura de los datos base
    for row in spamReader:
        trip = Trip(row[0],row[1],row[2],row[3],row[4],row[5])
        if row[2] in trips:
            print "id trip repeated : ",row[2]
        else:
            print row[2],trip.trip_id
            trips.update({row[2]:trip})

trips = {}


mypath = 'GTFS_Transantiago_2019_02_28'

readTrips(mypath,trips)

for t in trips:
    print t