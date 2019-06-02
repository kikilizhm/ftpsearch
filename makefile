

Cftps : source/main.o source/ftps.o source/sqlite.o
	gcc -o Cftps source/main.c source/ftps.c source/sqlite.c -Llib/  -llib -lpthread -ldl -L./source

a.cgi : web/cgi.o source/sqlite.o
	gcc -o a.cgi web/cgi.c source/sqlite.c -Llib/ -llib -lpthread -ldl -L./
	mv a.cgi web/
lib : lib/sqlite3/sqlite3.o lib/Clog/Clog.o
	ar rcs liblib.a lib/sqlite3/sqlite3.o lib/Clog/Clog.o
	mv liblib.a lib/

clean :
	rm -f *.o Cftps source/*o web/*.o web/cgi-bin/* source/Cftps
