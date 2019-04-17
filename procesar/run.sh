#!/bin/bash

version=v3.0

mkdir ../output/$version ;

#Procesamiento de datos de puntos de carga
#Falta colgar el procesamiento automatico via python
cp ../inputs/bip/PhoneFairPoints.csv ../output/$version ;

#Procesamiento de datos del GTFS
cp ../src/procesaGTFS/x64/Release/procesaGTFS.exe ./ ;

sed -e "s/VERSION/${version}/g" ../inputs/procesaGTFS.par > ./procesaGTFS.par ;
./procesaGTFS.exe procesaGTFS.par

#Procesamiento de datos de horarios del metro
cp ../inputs/metro/PhoneMetroSchedule.csv ../output/$version ;
cp ../inputs/metro/PhoneMetroSantiagoPredictor.csv ../output/$version ;
cp ../inputs/metro/PhoneStations.csv ../output/$version ;
