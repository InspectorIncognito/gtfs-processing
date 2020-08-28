import utm
import simplekml

stopStyleR5 = simplekml.Style()
stopStyleR5.color = simplekml.Color.blue
stopStyleR5.iconstyle.color = simplekml.Color.blue
stopStyleR5.iconstyle.scale = 1
stopStyleR5.labelstyle.color = simplekml.Color.white  # Make the text red
stopStyleR5.labelstyle.scale = 0.8  # Make the text twice as big
stopStyleR5.iconstyle.icon.href = 'http://maps.google.com/mapfiles/kml/paddle/wht-circle.png' #5

stopStyleI5 = simplekml.Style()
stopStyleI5.color = simplekml.Color.red
stopStyleI5.iconstyle.color = simplekml.Color.red
stopStyleI5.iconstyle.scale = 1
stopStyleI5.labelstyle.color = simplekml.Color.white  # Make the text red
stopStyleI5.labelstyle.scale = 0.8  # Make the text twice as big
stopStyleI5.iconstyle.icon.href = 'http://maps.google.com/mapfiles/kml/paddle/wht-circle.png' #5

stopStyleR10 = simplekml.Style()
stopStyleR10.color = simplekml.Color.blue
stopStyleR10.iconstyle.color = simplekml.Color.blue
stopStyleR10.iconstyle.scale = 1
stopStyleR10.labelstyle.color = simplekml.Color.white  # Make the text red
stopStyleR10.labelstyle.scale = 0.8  # Make the text twice as big
stopStyleR10.iconstyle.icon.href = 'http://maps.google.com/mapfiles/kml/paddle/wht-stars.png' #5

stopStyleI10 = simplekml.Style()
stopStyleI10.color = simplekml.Color.red
stopStyleI10.iconstyle.color = simplekml.Color.red
stopStyleI10.iconstyle.scale = 1
stopStyleI10.labelstyle.color = simplekml.Color.white  # Make the text red
stopStyleI10.labelstyle.scale = 0.8  # Make the text twice as big
stopStyleI10.iconstyle.icon.href = 'http://maps.google.com/mapfiles/kml/paddle/wht-stars.png' #5

# stopStyle10.iconstyle.icon.href = 'http://maps.google.com/mapfiles/kml/pal3/icon17.png' #10
def drawStops(kml,stops):
    stopStyle = simplekml.Style()
    stopStyle.color = simplekml.Color.red
    stopStyle.labelstyle.color = simplekml.Color.red  # Make the text red
    stopStyle.labelstyle.scale = 0.5  # Make the text twice as big
    stopStyle.iconstyle.icon.href = 'http://www.earthpoint.us/Dots/GoogleEarth/shapes/shaded_dot.png'

    fol = kml.newfolder(name='Stops',visibility=0)

    for stop in stops:
        pnt = fol.newpoint()
        pnt.name = stop.code
        pnt.style = stopStyle
        pnt.coords = [(stop.longitude, stop.latitude)]


def drawShapes(kml,shapes):

    for key in shapes:
        fol = kml.newfolder(name='Shape ' + key,visibility=0)
        coords = []
        for node in shapes[key].nodes:
            u = utm.to_latlon(node.x, node.y, 19, 'H')
            coords.append((u[1], u[0], 0))
        ls = fol.newlinestring(name='Polilinea')
        ls.style.linestyle.color = simplekml.Color.blue  # Red
        ls.style.linestyle.width = 5  # 10 pixels
        ls.coords = coords
        ls.extrude = 1
        ls.altitudemode = simplekml.AltitudeMode.relativetoground

def drawStopSequence(kml,stopsequences):
    stopStyle = simplekml.Style()
    stopStyle.color = simplekml.Color.red
    stopStyle.iconstyle.color = simplekml.Color.green
    stopStyle.iconstyle.scale = 1
    stopStyle.labelstyle.color = simplekml.Color.green  # Make the text red
    stopStyle.labelstyle.scale = 1  # Make the text twice as big
    stopStyle.iconstyle.icon.href = 'http://www.earthpoint.us/Dots/GoogleEarth/shapes/shaded_dot.png'

    for key in stopsequences:
        fol = kml.newfolder(name='StopSquence' + key)

        for d,stop in stopsequences[key].items():
            pnt = fol.newpoint()
            pnt.name = stop[0].code
            pnt.style = stopStyle
            pnt.style.balloonstyle.text = stop[0].code
            pnt.style.balloonstyle.bgcolor = simplekml.Color.lightgreen
            pnt.style.balloonstyle.textcolor = simplekml.Color.rgb(0, 0, 255)
            pnt.coords = [(stop[0].longitude, stop[0].latitude)]

def drawStopSequence(kml,stopsequences, shapes):


    for key in stopsequences:
        folder = kml.newfolder(name='Route ' + key)

        fol_shape = folder.newfolder(name='Shape ' + key)
        coords = []
        for node in shapes[key].nodes:
            u = utm.to_latlon(node.x, node.y, 19, 'H')
            coords.append((u[1], u[0], 0))
        ls = fol_shape.newlinestring(name='Polilinea')

        if key[-1] == "I":
            ls.style.linestyle.color = simplekml.Color.red
        else:
            ls.style.linestyle.color = simplekml.Color.blue

        ls.style.linestyle.width = 5  # 10 pixels
        ls.coords = coords
        ls.extrude = 1
        ls.altitudemode = simplekml.AltitudeMode.relativetoground

        fol_stopseq = folder.newfolder(name='StopSquence' + key)
        for d,stop in stopsequences[key].items():
            pnt = fol_stopseq.newpoint()

            if key[-1] == "I":
                if int(stop[1]) > 5 :
                    pnt.style = stopStyleI10
                else:
                    pnt.style = stopStyleI5
            else:
                if int(stop[1]) > 5 :
                    pnt.style = stopStyleR10
                else:
                    pnt.style = stopStyleR5

            pnt.name = str(int(stop[1]))
            pnt.style.balloonstyle.text = stop[0].code + " dist : " + str(stop[1])
            pnt.style.balloonstyle.textcolor = simplekml.Color.rgb(0, 0, 255)
            pnt.coords = [(stop[0].longitude, stop[0].latitude)]
