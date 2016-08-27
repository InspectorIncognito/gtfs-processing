import csv

f = open("metro24-08-2016-oficio-47702013.csv", 'rt')
try:
    reader = csv.reader(f)
    for row in reader:
        print row[2]
finally:
    f.close()