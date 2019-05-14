

Cftps : source/main.o source/ftps.o source/sqlite.o
	gcc -o Cftps source/main.c source/ftps.c source/sqlite.c -Llib/  -lClog -lsqlite3 -lpthread -ldl -L./source

a.cgi : web/cgi.o source/sqlite.o
	gcc -o a.cgi web/cgi.c source/sqlite.c -Llib/ -lClog -lsqlite3 -lpthread -ldl -L./
	mv a.cgi web/

clean :
	rm -f *.o Cftps source/*o web/*.o web/cgi-bin/* source/Cftps
