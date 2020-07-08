import xml.etree.ElementTree as ET
filePath = r'vista.kml'
tree = ET.parse(filePath)
lineStrings = tree.findall('.//{http://www.opengis.net/kml/2.2}LineString')

for attributes in lineStrings:
    for subAttribute in attributes:
        if subAttribute.tag == '{http://www.opengis.net/kml/2.2}coordinates':
            print (subAttribute.tag, subAttribute.text)