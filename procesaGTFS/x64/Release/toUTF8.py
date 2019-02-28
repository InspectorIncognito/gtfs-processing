
sourceEncoding = "iso-8859-1"
targetEncoding = "utf-8"
source = open("Android_busstops_sequencesv2.3.csv")
target = open("Android_busstops_sequencesv2.3.utf.csv", "w")

target.write(unicode(source.read(), sourceEncoding).encode(targetEncoding))