from zipfile import ZipFile
import xml.etree.ElementTree as ET

filename = 'valpoKmz/P.E UN 8.kmz'

kmz = ZipFile(filename, 'r')

kml = kmz.open('doc.kml', 'r')

contents = kml.read()

contenido = str(contents)
contenido.encode("utf-8")

contentes = contenido.replace("\\t", "")
contentes = contentes.replace("\\n", "")

print(contentes)

tree = ET.fromstring(contentes)

lineStrings = tree.findall('.//{http://www.opengis.net/kml/2.2}LineString')
descriptions = tree.findall('.//{http://www.opengis.net/kml/2.2}description')
placemarks = tree.findall('.//{http://www.opengis.net/kml/2.2}Placemark')



for attributes in placemarks:
    print (attributes.text)
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
                                        print(s0.text+','+coords[i].strip()+','+str(i))
                                    #i=i+1

