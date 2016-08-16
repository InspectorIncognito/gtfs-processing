import json
import urllib2
import time

#reload(sys)
#sys.setdefaultencoding("latin-1")

services =json.load(urllib2.urlopen("http://www.transantiago.cl/restservice/rest/getservicios/all"))

f = open('horarios.csv', 'w')

for service in services:
	webdata = ""
	try :
		webdata = urllib2.urlopen("http://www.transantiago.cl/restservice/rest/getrecorrido/"+service)
	except urllib2.HTTPError:
		print "Oops, no se pudo recuperar : " + service
		continue

	try:
		data =json.load(webdata)
	except ValueError:
		print "Oops, no se pudo recuperar : " + service
		continue

	print data[0]['cod']
	for horario in data[0]['horarios']:
		registro = data[0]['cod'] + ";" + data[0]['destino'] + ";" + horario["tipoDia"] + ";" + horario["inicio"] + ";" + horario['fin']
		f.write( registro.encode('latin-1') )
		f.write ("\n")
	for horario in data[1]['horarios']:
		registro = data[1]['cod'] + ";" + data[1]['destino'] + ";" + horario["tipoDia"] + ";" + horario["inicio"] + ";" + horario['fin']
		f.write( registro.encode('latin-1') )
		f.write ("\n")
	#break;
	#time.sleep( 1 )
	
#

#print decoded[0]['destino']#[0]['tipoDia']

#