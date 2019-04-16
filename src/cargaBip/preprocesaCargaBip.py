import csv

f_out = open('cargaBip.csv', 'w')

f_in = open("metro24-08-2016-oficio-47702013.csv", 'rt')
reader = csv.reader(f_in)
for row in reader:
	f_out.write(row[2]+";"+row[8]+";"+row[9]+"\n")
		
f_in = open("pcma24082016-oficio-47702013.csv", 'rt')
reader = csv.reader(f_in)
iterrow = iter(reader)
next(iterrow)
for row in iterrow:
	f_out.write(row[2]+";"+row[8]+";"+row[9]+"\n")
	
f_in = open("pcmav-alto-estandar24082016-oficio-47702013.csv", 'rt')
reader = csv.reader(f_in)
iterrow = iter(reader)
next(iterrow)
for row in iterrow:
	f_out.write(row[1]+";"+row[7]+";"+row[8]+"\n")

	
f_in = open("pcmav-estandar-normal24082016-oficio-47702013.csv", 'rt')
reader = csv.reader(f_in)
iterrow = iter(reader)
next(iterrow)
for row in iterrow:
	f_out.write(row[1]+";"+row[7]+";"+row[8]+"\n")
	
f_in = open("retail24-08-2016-oficio-47702013.csv", 'rt')
reader = csv.reader(f_in)
iterrow = iter(reader)
next(iterrow)
for row in iterrow:
	f_out.write(row[2]+";"+row[8]+";"+row[9]+"\n")