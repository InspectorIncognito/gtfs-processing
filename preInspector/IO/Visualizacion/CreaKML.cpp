/**
* Project: "proceGTFS".
* Package:
* Class:
* Created by: Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* Terms: These coded instructions, statements, and computer programs are
* produced as part of a software project. They contain unpublished
* propietary information and are protected by copyright law. They may
* not be disclosed to third parties or copied or duplicated in any form,
* in whole or in part, without the prior written consent of Pragxis SpA.
* Copyright:  Pragxis (c) 2016
* Last modified : Mauricio Zuñiga 18-06-2016
*/
#include "CreaKML.h"

CreaKML::CreaKML()
{
}

CreaKML::CreaKML(const char *nombreArchivo,const char *nombre)
{
	this->archivo_kml.open (nombreArchivo);

	this->archivo_kml << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	this->archivo_kml << "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n";
	this->archivo_kml << "<Document>\n";
	this->archivo_kml << "<name>" << nombre << "</name>\n";

	this->DefaultStyles();
}

void CreaKML::DefaultStyles()
{
	InsertaEstiloColor("#FFFF0000","Azul");
	InsertaEstiloColor("#FF0000FF","Rojo");
	InsertaEstiloColor("#FF00FFFF","Amarillo");
	InsertaEstiloColor("#FF0080FF","Naranja");
	InsertaEstiloColor("#FF969696","Gris");
	InsertaEstiloColor("#FF0B610B","VerdeIntenso");
	InsertaEstiloColor("#FF58FA58","VerdePalido");
	InsertaEstiloColor("#FFFBF98F","Magenta");
	InsertaEstiloColor("#FFF21CFD","Rosado");
	
	
}

void CreaKML::InsertaEstilos(const char *nombreEstilo,
			     const char *nombreNormalEstilo,
			     const char *nombreHighlightEstilo,
			     const char *icon1,
		 	     const char *iconEscala1,
			     const char *iconColor,
			     const char *colorLabelStyle1,
			     const char *escalaLabelStyle1,
			     const char *colorLineStyle1,
			     const char *anchoLineStyle1,
			     const char *icon2,
			     const char *iconEscala2,
			     const char *colorLabelStyle2,
			     const char *escalaLabelStyle2,
			     const char *colorLineStyle2,
			     const char *anchoLineStyle2)
{
	this->archivo_kml << "<StyleMap id=\""<< nombreEstilo <<"\">\n"; //Sheet1Map1
	this->archivo_kml << "	<Pair>\n";
	this->archivo_kml << "		<key>normal</key>\n";
	this->archivo_kml << "		<styleUrl>#"<< nombreNormalEstilo <<"</styleUrl>\n";
	this->archivo_kml << "	</Pair>\n";
	this->archivo_kml << "	<Pair>\n";
	this->archivo_kml << "		<key>highlight</key>\n";
	this->archivo_kml << "		<styleUrl>#" << nombreHighlightEstilo << "</styleUrl>\n";
	this->archivo_kml << "	</Pair>\n";
	this->archivo_kml << "	</StyleMap>\n";
	
	this->InsertaEstilo(nombreNormalEstilo,
			    icon1,
			    iconEscala1,
			    iconColor,
			    colorLabelStyle1,
			    escalaLabelStyle1,
			    colorLineStyle1,
			    anchoLineStyle1);

	this->InsertaEstilo(nombreHighlightEstilo,
			    icon2,
			    iconEscala2,
			    iconColor,
			    colorLabelStyle2,
			    escalaLabelStyle2,
			    colorLineStyle2,
			    anchoLineStyle2);		
}

void CreaKML::InsertaEstilo(const char *nombreEstilo,
			    const char *icon,
			    const char *iconEscala,
 			    const char *iconColor,
			    const char *colorLabelStyle,
			    const char *escalaLabelStyle,
			    const char *colorLineStyle,
			    const char *anchoLineStyle)
{
	
	this->archivo_kml << "<Style id=\"" << nombreEstilo << "\">\n";
	this->archivo_kml << "	<IconStyle>\n";
	this->archivo_kml << "		<scale>"<< iconEscala <<"</scale>\n";
	this->archivo_kml << "		<Icon>\n";
	this->archivo_kml << "			<href>"<< icon <<"</href>\n";
	this->archivo_kml << "		</Icon>\n";
	this->archivo_kml << "		<heading>25</heading>\n";
	this->archivo_kml << "		<color>"<< iconColor << "</color>\n"; //FF00FFFF
	this->archivo_kml << "	</IconStyle>\n";
	this->archivo_kml << "	<LabelStyle>\n";
	this->archivo_kml << "		<color>"<< colorLabelStyle <<"</color>\n"; //FFFFFFFF
	this->archivo_kml << "		<scale>"<< escalaLabelStyle << "</scale>\n"; //1
	this->archivo_kml << "	</LabelStyle>\n";
	this->archivo_kml << "	<LineStyle>\n";
	this->archivo_kml << "		<color>"<< colorLineStyle <<"</color>\n"; //FFFF00FF
	this->archivo_kml << "		<width>"<< anchoLineStyle << "</width>\n"; //2
	this->archivo_kml << "	</LineStyle>\n";
	this->archivo_kml << "	<PolyStyle>\n";
	this->archivo_kml << "		<fill>0</fill>\n";
	this->archivo_kml << "		<outline>1</outline>\n";
	this->archivo_kml << "		<color>00FF00FF</color>\n";
	this->archivo_kml << "	</PolyStyle>\n";
	this->archivo_kml << "	<BalloonStyle>\n";
	this->archivo_kml << "		<text><![CDATA[$[description]]]></text>\n";
	this->archivo_kml << "	</BalloonStyle>\n";
	this->archivo_kml << "</Style>\n";	
}


void CreaKML::InsertaPunto(const char *nombre,
			   const char *descripcion,
			   const char *latitud,
                           const char *longitud,
			   const char *style,
			   int visibility)
{
	this->archivo_kml << "	<Placemark>\n";
	this->archivo_kml << "		<visibility>"<< visibility << "</visibility>\n";
	this->archivo_kml << "		<name>"<< nombre << "</name>\n";
	this->archivo_kml << "		<description>" << descripcion << "</description>\n";
	this->archivo_kml << "		<styleUrl>#" << style << "</styleUrl>\n";
	this->archivo_kml << "		<Point>\n";
	this->archivo_kml << "			<coordinates>" << longitud << "," << latitud << "</coordinates>\n";
	this->archivo_kml << "		</Point>\n";
	this->archivo_kml << "	</Placemark>\n";
}



void CreaKML::InsertaSegmento(const char *nombre,
			      const char *lat1,
                              const char *lon1,
			      const char *lat2,
                              const char *lon2,
			      const char *style,
			      int visibility)
{
	this->archivo_kml << "	<Placemark>\n";
	this->archivo_kml << "		<visibility>"<< visibility << "</visibility>\n";
	this->archivo_kml << "		<name>"<< nombre << "</name>\n";
	this->archivo_kml << "		<styleUrl>" << style << "</styleUrl>\n";
	this->archivo_kml << "		<LineString>\n";
	this->archivo_kml << "			<altitudeMode>clampToGround</altitudeMode>\n";
	this->archivo_kml << "			<tessellate>1</tessellate>\n";
	this->archivo_kml << "				<coordinates>" << lon1 << "," << lat1 << ",0 " << lon2 << "," << lat2 << ",0" << "</coordinates>\n";
	this->archivo_kml << "		</LineString>\n";
	this->archivo_kml << "	</Placemark>\n";
}


void CreaKML::CierraArchivo()
{
	this->archivo_kml << "</Document>\n";
	this->archivo_kml << "</kml>\n";
	this->archivo_kml.close();
}


void CreaKML::InsertaSetEstilosColor(int iniEscala,int finEscala,int nIniEstilos,int nFinEstilos)
{
	
	for(int j=nIniEstilos;j<=nFinEstilos;j++)
	{
		stringstream tmp;
		tmp << j;

		string txt1("SheetColor");
		txt1.append(tmp.str());
		
		string txt2("NormalSheetColor");
		txt2.append(tmp.str());
		
		string txt3("HighlightSheetColor");
		txt3.append(tmp.str());

		string Color("");
		Color.append("FF");
		Color.append(ColorScale::int2ColorHex(iniEscala,finEscala,1,j));

		InsertaEstilos(txt1.c_str(),
				txt2.c_str(),
				txt3.c_str(),
				"http://www.earthpoint.us/Dots/GoogleEarth/shapes/shaded_dot.png",
				"0.5",
				Color.c_str(),
				"00FF00FF",
				"1",
				Color.c_str(),
				"2",
				"http://www.earthpoint.us/Dots/GoogleEarth/shapes/shaded_dot.png",
				"0.5",
				Color.c_str(),
				"1.1",
				"FFFF00FF",
				"3");
	}
}

void CreaKML::InsertaEstiloColor(const char* color,const char* codigo)
{
	string txt1("SheetColor");
	txt1.append(codigo);
		
	string txt2("NormalSheetColor");
	txt2.append(codigo);
		
	string txt3("HighlightSheetColor");
	txt3.append(codigo);

	InsertaEstilos(txt1.c_str(),
			txt2.c_str(),
			txt3.c_str(),
			"http://www.earthpoint.us/Dots/GoogleEarth/shapes/shaded_dot.png",
			"0.5",
			color,
			"00FF00FF",
			"1",
			color,
			"2",
			"http://www.earthpoint.us/Dots/GoogleEarth/shapes/shaded_dot.png",
			"0.5",
			color,
			"1.1",
			"FFFF00FF",
			"3");
}
void CreaKML::InsertaExtrude(double x, double y,double ancho,double alto)
{
	double lat0,lon0;
	double lat1,lon1;
	double lat2,lon2;
	double lat3,lon3;
	char *UTMZone="19H";
	ConvertCoordinate::UTMtoLL(23, y-(ancho/2), x-(ancho/2), UTMZone, lat0, lon0);
	ConvertCoordinate::UTMtoLL(23, y-(ancho/2), x+(ancho/2), UTMZone, lat1, lon1);
	ConvertCoordinate::UTMtoLL(23, y+(ancho/2), x+(ancho/2), UTMZone, lat2, lon2);
	ConvertCoordinate::UTMtoLL(23, y+(ancho/2), x-(ancho/2), UTMZone, lat3, lon3);

	this->archivo_kml << "  <Placemark> " << endl;
    this->archivo_kml << "<name>Extruded Cube</name> " << endl;
    this->archivo_kml << "<visibility>0</visibility> " << endl;
    this->archivo_kml << "<styleUrl>#radio-folder</styleUrl> " << endl;
    this->archivo_kml << "<Style> " << endl;
	this->archivo_kml << "<PolyStyle> " << endl;
	this->archivo_kml << "<color>ff0000ff</color> " << endl;
	this->archivo_kml << "</PolyStyle> " << endl;
    this->archivo_kml << "</Style> " << endl;
    this->archivo_kml << "<Polygon> " << endl;
    this->archivo_kml << "<extrude>1</extrude> " << endl;
    this->archivo_kml << "<altitudeMode>relativeToGround</altitudeMode> " << endl;
    this->archivo_kml << "<outerBoundaryIs> " << endl;
    this->archivo_kml << "<LinearRing> " << endl;
    this->archivo_kml << "<coordinates> " << endl;
	this->archivo_kml.precision(20);
	this->archivo_kml << lon0 << "," << lat0 << "," << alto  << endl;
	this->archivo_kml << lon1 << "," << lat1 << "," << alto  << endl;
	this->archivo_kml << lon2 << "," << lat2 << "," << alto  << endl;
	this->archivo_kml << lon3 << "," << lat3 << "," << alto  << endl;
	this->archivo_kml << lon0 << "," << lat0 << "," << alto  << endl;
    this->archivo_kml << "</coordinates> " << endl;
    this->archivo_kml << "</LinearRing> " << endl;
    this->archivo_kml << "</outerBoundaryIs> " << endl;
    this->archivo_kml << "</Polygon> " << endl;
    this->archivo_kml << "</Placemark> " << endl;
}


void CreaKML::InsertaEstilo(const char* color,const char* codigo, const char *porte)
{
	string txt1("SheetColor");
	txt1.append(codigo);
		
	string txt2("NormalSheetColor");
	txt2.append(codigo);
		
	string txt3("HighlightSheetColor");
	txt3.append(codigo);
	
	InsertaEstilos(txt1.c_str(),
			txt2.c_str(),
			txt3.c_str(),
			"http://www.earthpoint.us/Dots/GoogleEarth/shapes/shaded_dot.png",
			porte,
			color,
			"00FF00FF",
			"1",
			color,
			"2",
			"http://www.earthpoint.us/Dots/GoogleEarth/shapes/shaded_dot.png",
			porte,
			color,
			"1.1",
			"FFFF00FF",
			"3");	
}

void CreaKML::AbreFolder(const char* nombre)
{
	this->archivo_kml << "<Folder>\n";
	this->archivo_kml << "		<name>" << nombre << "</name>\n";
}

void CreaKML::CierraFolder()
{
	this->archivo_kml << "</Folder>\n";
}

CreaKML::~CreaKML()
{
}
