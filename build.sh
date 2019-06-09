make lib
make Cftps
make a.cgi
echo "start to search servers..."
./Cftps search
echo "end to search servers..."
mv data.db3 ./web/


