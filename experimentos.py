import json
import urllib2
import time

#f = open('horarios.csv', 'w')

link = "http://dev.adderou.cl/transanpbl/busdata.php?paradero="
link2 = "http://54.94.231.101/dtpm/busStopInfo/og2mgzby9crdm14wq6g7nxkb458gkny/"

webPC666 = urllib2.urlopen(link+"PC666")
PC666 =json.load(webPC666)
for servicio in PC666['servicios']:
	if(servicio['servicio'] == '518'):
		print  "1 PC666 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']
		
webPC666 = urllib2.urlopen(link2+"PC666")
PC666 =json.load(webPC666)
for servicio in PC666['servicios']:
	if(servicio['servicio'] == '518'):
		print  "2 PC666 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']
print "-------------------------------------------------------------"

webPC667 = urllib2.urlopen(link+"PC667")
PC667 =json.load(webPC667)
for servicio in PC667['servicios']:
	if(servicio['servicio'] == '518'):
		print  "1 PC667 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']

webPC667 = urllib2.urlopen(link2+"PC667")
PC667 =json.load(webPC667)
for servicio in PC667['servicios']:
	if(servicio['servicio'] == '518'):
		print  "2 PC667 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']
print "-------------------------------------------------------------"
		
webPC668 = urllib2.urlopen(link+"PC668")
PC668 =json.load(webPC668)
print webPC668.read()
"""print webPC668.read()
#for servicio in PC668['servicios']:
#	if(servicio['servicio'] == '518'):
#		if 'patente' not in servicio:
#			print  "1 PC668 : " + servicio['servicio']
#		else:
#			if servicio['patente'] is not None:
#				print  "1 PC668 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']
"""	
print "-------------------------------------------------------------"
webPC668 = urllib2.urlopen(link2+"PC668")
print webPC668.read()
"""PC668 =json.load(webPC668)
for servicio in PC668['servicios']:
	if(servicio['servicio'] == '518'):
		if 'patente' not in servicio:
			print  "2 PC668 : " + servicio['servicio']
		else:
			if servicio['patente'] is not None:
				print  "2 PC668 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']
"""
print "-------------------------------------------------------------"
		
webPC669 = urllib2.urlopen(link+"PC669")
PC669 =json.load(webPC669)
for servicio in PC669['servicios']:
	if(servicio['servicio'] == '518'):
		if 'patente' not in servicio:
			print  "1 PC669 : " + servicio['servicio']
		else:
			if servicio['patente'] is not None:
				print  "1 PC669 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']

webPC669 = urllib2.urlopen(link2+"PC669")
PC669 =json.load(webPC669)
for servicio in PC669['servicios']:
	if(servicio['servicio'] == '518'):
		if 'patente' not in servicio:
			print  "2 PC669 : " + servicio['servicio']
		else:
			if servicio['patente'] is not None:
				print  "2 PC669 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']
print "-------------------------------------------------------------"		

webPC670 = urllib2.urlopen(link+"PC670")
PC670 =json.load(webPC670)
for servicio in PC670['servicios']:
	if(servicio['servicio'] == '518'):
		if 'patente' not in servicio:
			print  "1 PC670 : " + servicio['servicio']
		else:
			if servicio['patente'] is not None:
				print  "1 PC670 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']

webPC670 = urllib2.urlopen(link2+"PC670")
PC670 =json.load(webPC670)
for servicio in PC670['servicios']:
	if(servicio['servicio'] == '518'):
		if 'patente' not in servicio:
			print  "2 PC670 : " + servicio['servicio']
		else:
			if servicio['patente'] is not None:
				print  "2 PC670 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']
print "-------------------------------------------------------------"
			
webPC975 = urllib2.urlopen(link+"PC975")
print webPC975.read()
PC975 =json.load(webPC975)
for servicio in PC975['servicios']:
	if(servicio['servicio'] == '518'):
		if 'patente' not in servicio:
			print  "1 PC975 : " + servicio['servicio']
		else:
			if servicio['patente'] is not None:
				print  "1 PC975 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']

webPC975 = urllib2.urlopen(link2+"PC975")
PC975 =json.load(webPC975)
for servicio in PC975['servicios']:
	if(servicio['servicio'] == '518'):
		if 'patente' not in servicio:
			print  "2 PC975 : " + servicio['servicio']
		else:
			if servicio['patente'] is not None:
				print  "2 PC975 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']
print "-------------------------------------------------------------"
				
webPC976 = urllib2.urlopen(link+"PC976")
PC976 =json.load(webPC976)
for servicio in PC976['servicios']:
	if(servicio['servicio'] == '518'):
		if 'patente' not in servicio:
			print  "1 PC976 : " + servicio['servicio']
		else:
			if servicio['patente'] is not None:
				print  "1 PC976 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']
	
webPC976 = urllib2.urlopen(link2+"PC976")
PC976 =json.load(webPC976)
for servicio in PC976['servicios']:
	if(servicio['servicio'] == '518'):
		if 'patente' not in servicio:
			print  "2 PC976 : " + servicio['servicio']
		else:
			if servicio['patente'] is not None:
				print  "2 PC976 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']
print "-------------------------------------------------------------"


webPC977 = urllib2.urlopen(link+"PC977")
PC977 =json.load(webPC977)
for servicio in PC977['servicios']:
	if(servicio['servicio'] == '518'):
		if 'patente' not in servicio:
			print  "1 PC977 : " + servicio['servicio']
		else:
			if servicio['patente'] is not None:
				print  "1 PC977 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']
		
webPC977 = urllib2.urlopen(link2+"PC977")
PC977 =json.load(webPC977)
for servicio in PC977['servicios']:
	if(servicio['servicio'] == '518'):
		if 'patente' not in servicio:
			print  "2 PC977 : " + servicio['servicio']
		else:
			if servicio['patente'] is not None:
				print  "2 PC977 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']
print "-------------------------------------------------------------"

webPC482 = urllib2.urlopen(link+"PC482")
PC482 =json.load(webPC482)
for servicio in PC482['servicios']:
	if(servicio['servicio'] == '518'):
		if 'patente' not in servicio:
			print  "1 PC482 : " + servicio['servicio']
		else:
			if servicio['patente'] is not None:
				print  "1 PC482 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']
			
webPC482 = urllib2.urlopen(link2+"PC482")
PC482 =json.load(webPC482)
for servicio in PC482['servicios']:
	if(servicio['servicio'] == '518'):
		if 'patente' not in servicio:
			print  "2 PC482 : " + servicio['servicio']
		else:
			if servicio['patente'] is not None:
				print  "2 PC482 : " + servicio['servicio'] + "	" + servicio['patente'] + "	" + servicio['tiempo'] + "			" + servicio['distancia']
print "-------------------------------------------------------------"
		
#http://dev.adderou.cl/transanpbl/busdata.php?paradero=PI62

#for service in services:
#	webdata = ""
#	try :
#		webdata = urllib2.urlopen("http://www.transantiago.cl/restservice/rest/getrecorrido/"+service)
#	except urllib2.HTTPError:
#		print "Oops, no se pudo recuperar : " + service
#		continue
#
#	try:
#		data =json.load(webdata)
#	except ValueError:
#		print "Oops, no se pudo recuperar : " + service
#		continue
#
#	print data[0]['cod']
#	for horario in data[0]['horarios']:
#		registro = data[0]['cod'] + ";" + data[0]['destino'] + ";" + horario["tipoDia"] + ";" + horario["inicio"] + ";" + horario['fin']
#		f.write( registro.encode('latin-1') )
#		f.write ("\n")
#	for horario in data[1]['horarios']:
#		registro = data[1]['cod'] + ";" + data[1]['destino'] + ";" + horario["tipoDia"] + ";" + horario["inicio"] + ";" + horario['fin']
#		f.write( registro.encode('latin-1') )
#		f.write ("\n")
	#break;
	#time.sleep( 1 )
	
#

#print decoded[0]['destino']#[0]['tipoDia']

#