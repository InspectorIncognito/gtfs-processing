sed -e 's///g' agency.txt > tmp; mv tmp agency.txt;
sed -e 's///g' calendar.txt > tmp; mv tmp calendar.txt; 
sed -e 's///g' calendar_dates.txt > tmp; mv tmp calendar_dates.txt;
sed -e 's///g' feed_info.txt > tmp; mv tmp feed_info.txt;
sed -e 's///g' frequencies.txt > tmp; mv tmp frequencies.txt;
sed -e 's///g' routes.txt > tmp; mv tmp routes.txt;
sed -e 's///g' shapes.txt > tmp; mv tmp shapes.txt;
sed -e 's///g' stops.txt > tmp; mv tmp stops.txt;
sed -e 's///g' stop_times.txt > tmp; mv tmp stop_times.txt;
sed -e 's///g' trips.txt > tmp; mv tmp trips.txt;
