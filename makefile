

Cftps : source/main.o source/ftps.o source/sqlite.o
	gcc -o Cftps source/main.c source/ftps.c source/sqlite.c -Llib/  -llib -lpthread -ldl -L./source

a.cgi : web/cgi.o source/sqlite.o
	gcc -o a.cgi web/cgi.c source/sqlite.c -Llib/ -llib -lpthread -ldl -L./
	mv a.cgi web/
lib : lib/sqlite3/sqlite3.o lib/Clog/Clog.o
	ar rcs liblib.a lib/sqlite3/sqlite3.o lib/Clog/Clog.o
	mv liblib.a lib/

clean :
	rm -f lib/sqlite3/*.o
	rm -f lib/Clog/*.o
	rm -f lib/*.a
	rm -f source/*.o
	rm -f web/*.o
	rm -f *.o Cftps web/cgi-bin/* source/Cftps 
