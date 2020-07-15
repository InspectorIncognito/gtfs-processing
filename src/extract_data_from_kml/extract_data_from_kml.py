import xml.etree.ElementTree as ET

def extractShapes(filein,fileout):

    filePath = filein
    tree = ET.parse(filePath)
    lineStrings = tree.findall('.//{http://www.opengis.net/kml/2.2}LineString')
    descriptions = tree.findall('.//{http://www.opengis.net/kml/2.2}description')
    placemarks = tree.findall('.//{http://www.opengis.net/kml/2.2}Placemark')


    for attributes in placemarks:
        for s0 in attributes:
            if s0.tag == '{http://www.opengis.net/kml/2.2}name':
                if s0.text.find('_') > 0:
                    for s1 in attributes:
                        if s1.tag == '{http://www.opengis.net/kml/2.2}LineString':
                            for s2 in s1:
                                if s2.tag == '{http://www.opengis.net/kml/2.2}coordinates':
                                    coords = s2.text.split(' ')
                                    i=0
                                    for i in range(0,len(coords)):
                                        if i != len(coords) - 1 :
                                            c = coords[i].strip().split(',')
                                            fileout.write(s0.text+','+c[1]+','+c[0]+','+str(i)+'\n')

def extractStops(filein,fileout):

    filePath = filein
    tree = ET.parse(filePath)
    placemarks = tree.findall('.//{http://www.opengis.net/kml/2.2}Placemark')

    i = 1
    for attributes in placemarks:
        for s0 in attributes:
            if s0.tag == '{http://www.opengis.net/kml/2.2}name':
                for s1 in attributes:
                    if s1.tag == '{http://www.opengis.net/kml/2.2}Point':
                        for s2 in s1:
                            if s2.tag == '{http://www.opengis.net/kml/2.2}coordinates':
                                c = s2.text.split(',')
                                fileout.write(str(i) + ',' +str(i) + ','+ s0.text+',,'+c[1] + ',' + c[0] + ',,,,,'+'\n')
                                i=i+1

if __name__ == "__main__":
    fstops = open("stops.txt", "w")
    fstops.write("stop_id,stop_code,stop_name,stop_desc,stop_lat,stop_lon,zone_id,stop_url,location_type,parent_station,wheelchair_boarding\n");
    extractStops(r'valpo_paradas/Paraderos Gran Valparaíso/doc.kml', fstops)
    fstops.close()


    fshapes = open("shapes.txt", "w")
    fshapes.write("shape_id, shape_pt_lat, shape_pt_lon, shape_pt_sequence\n")
    extractShapes(r'valpoTrazadosKml/UN1.kml', fshapes)
    extractShapes(r'valpoTrazadosKml/UN2.kml', fshapes)
    extractShapes(r'valpoTrazadosKml/UN3.kml', fshapes)
    extractShapes(r'valpoTrazadosKml/UN4.kml', fshapes)
    extractShapes(r'valpoTrazadosKml/UN5.kml', fshapes)
    extractShapes(r'valpoTrazadosKml/UN6.kml', fshapes)
    extractShapes(r'valpoTrazadosKml/UN7.kml', fshapes)
    extractShapes(r'valpoTrazadosKml/UN8.kml', fshapes)
    extractShapes(r'valpoTrazadosKml/UN9.kml', fshapes)
    extractShapes(r'valpoTrazadosKml/UN10.kml', fshapes)
    fshapes.close()
