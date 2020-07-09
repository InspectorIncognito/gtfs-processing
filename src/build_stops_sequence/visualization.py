import utm
import simplekml

def drawStops(kml,stops):
    stopStyle = simplekml.Style()
    stopStyle.color = simplekml.Color.red
    stopStyle.labelstyle.color = simplekml.Color.red  # Make the text red
    stopStyle.labelstyle.scale = 0.5  # Make the text twice as big
    stopStyle.iconstyle.icon.href = 'http://www.earthpoint.us/Dots/GoogleEarth/shapes/shaded_dot.png'

    fol = kml.newfolder(name='Stops')

    for stop in stops:
        pnt = fol.newpoint()
        pnt.name = stop.code
        pnt.style = stopStyle
        pnt.coords = [(stop.longitude, stop.latitude)]


def drawShapes(kml,shapes):

    for key in shapes:
        fol = kml.newfolder(name='Shape ' + key)
        coords = []
        for node in shapes[key].nodes:
            u = utm.to_latlon(node.x, node.y, 19, 'K')
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
            pnt.name = stop.code
            pnt.style = stopStyle
            pnt.style.balloonstyle.text = stop.code
            pnt.style.balloonstyle.bgcolor = simplekml.Color.lightgreen
            pnt.style.balloonstyle.textcolor = simplekml.Color.rgb(0, 0, 255)
            pnt.coords = [(stop.longitude, stop.latitude)]