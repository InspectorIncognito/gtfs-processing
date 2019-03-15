import sys
import csv

class trip:
    def __init__(self, route_id,service_id,trip_id,trip_headsign,direction_id,shape_id):
        self.route_id = route_id
        self.service_id = service_id
        self.trip_id = trip_id
        self.trip_headsign = trip_headsign
        self.direction_id = direction_id
        self.shape_id = shape_id
