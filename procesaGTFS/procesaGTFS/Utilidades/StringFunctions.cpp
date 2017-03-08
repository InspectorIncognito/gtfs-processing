/**
* Project: "pvm".
* Package: Utilidades
* Class: StringFunctions
* Created by: Mauricio Zuñiga G. <mzuniga@pragxis.cl>
* Terms: These coded instructions, statements, and computer programs are
* produced as part of a software project. They contain unpublished
* propietary information and are protected by copyright law. They may
* not be disclosed to third parties or copied or duplicated in any form,
* in whole or in part, without the prior written consent of Pragxis SpA.
* Copyright:  Pragxis (c) 2013
* Last modified : Mauricio Zuñiga 15-04-2013
*/
#include "StringFunctions.h"

StringFunctions::StringFunctions()
{
}


StringFunctions::~StringFunctions()
{
}

string StringFunctions::ReplaceChar(string str, char ch1, char ch2)
{
  for (int i = 0; i < str.length(); ++i) {
    if (str[i] == ch1)
      str[i] = ch2;
  }

  return str;
}

string StringFunctions::toLatex(string str)
{
  string output=string("");

  for (int i = 0; i < str.length(); ++i) 
  {
    if (str[i] == '_')
		output.append(Char2String('\\')+Char2String(str[i]));
	else
		output.append(Char2String(str[i]));
  }

  return output;
}


vector<string> StringFunctions::ExplodeF(char separator , ifstream *f)
{
	char line[MAXLINE];
	f->getline(line,MAXLINE);

	string str = string(line);
	vector<string>  result;
	size_t pos1 = 0;
	size_t pos2 = 0;
	string aux = "";
	bool openQuot = false;
	if (separator == ',') {
		for (int i = 0; i < str.size(); i++) {
			if (str[i] != '"' && !openQuot)
				aux += str[i];
			else if (str[i] == '"') {
				if (!openQuot)
					openQuot = true;
				else
					openQuot = false;
			}
			else if (openQuot) {
				if (str[i] != ',')
					aux += str[i];
			}
		}
	}
	else
		aux = str;
	/*if (separator == ',') {
		size_t posQuotOpen = -1;
		size_t posQuotClose = -1;
		size_t auxPos = 0;
		bool quotes = false;
		posQuotOpen = str.find('"', 0);
		posQuotClose = str.find('"', posQuotOpen+1);
		if (posQuotOpen != str.npos && posQuotClose != str.npos)
			quotes = true;
		while (pos2 != str.npos)
		{
			pos2 = str.find(separator, pos1);
			if (quotes) {
				if (posQuotOpen < pos2 && pos2 < posQuotClose) {
					string aux="";
					for (int i = posQuotOpen; i <= posQuotClose; i++) {
						if (str[i] != ',')
							aux += str[i];
					}
					result.push_back(aux);
					//result.push_back(str.substr(posQuotOpen + 1, posQuotClose - posQuotOpen - 1));
					pos1 = posQuotClose + 2;
					quotes = false;
					posQuotOpen = str.find('"', pos1);
					posQuotClose = str.find('"', posQuotOpen + 1);
					if (posQuotOpen != str.npos && posQuotClose != str.npos) {
						quotes = true;
					}
					continue;
				}
			}
			if (pos2 != str.npos)
			{
				auxPos = str.find(separator, pos2 + 1);
				if (auxPos > posQuotClose && quotes) {
					if (pos2 > pos1)
					{
						result.push_back(str.substr(pos1, pos2 - pos1));
					}
					else
					{
						result.push_back(string("-"));
					}
					if((posQuotClose - posQuotOpen - 1)>0)
						result.push_back(str.substr((posQuotOpen + 1), posQuotClose - posQuotOpen - 1));
					else
						result.push_back(string("-"));
					pos2 = posQuotClose + 1;
					quotes = false;
					posQuotOpen = str.find('"', posQuotClose + 1);
					posQuotClose = str.find('"', posQuotOpen + 1);
					if (posQuotOpen != str.npos && posQuotClose != str.npos) {
						quotes = true;
					}
				}
				else {
					if (pos2 > pos1)
					{
						result.push_back(str.substr(pos1, pos2 - pos1));
					}
					else
					{
						result.push_back(string("-"));
						//cout << str.substr(pos1, pos2-pos1) << "|" << pos1 << "|" << pos2 << endl;
					}
				}
				pos1 = pos2 + 1;
			}
		}
	}
	else {
		while (pos2 != str.npos)
		{
			pos2 = str.find(separator, pos1);
			if (pos2 != str.npos)
			{
				if (pos2 > pos1)
				{
					result.push_back(str.substr(pos1, pos2 - pos1));
				}
				else
				{
					result.push_back(string("-"));
					//cout << str.substr(pos1, pos2-pos1) << "|" << pos1 << "|" << pos2 << endl;
				}
				pos1 = pos2 + 1;
			}
		}
	}
	if(pos1 > str.size())
		result.push_back("");
	else
		result.push_back( str.substr(pos1, str.size()-pos1) );*/
	while (pos2 != aux.npos)
	{
		pos2 = aux.find(separator, pos1);
		if (pos2 != aux.npos)
		{
			if (pos2 > pos1)
			{
				result.push_back(aux.substr(pos1, pos2 - pos1));
			}
			else
			{
				result.push_back(string("-"));
			}
			pos1 = pos2 + 1;
		}
	}
	result.push_back(aux.substr(pos1, aux.size() - pos1));
	return result;
}





string StringFunctions::Double2String(double d,int p)
{
	ostringstream str;
	str.precision(p);
	str << d;
	return str.str();
}

string StringFunctions::Int2String(int d)
{
	ostringstream str;
	str << d;
	return str.str();
}

string StringFunctions::Char2String(char c)
{
	stringstream str;
	str << c;
	return str.str();
}

string StringFunctions::Long2String(long d)
{
	ostringstream str;
	str << d;
	return str.str();
}