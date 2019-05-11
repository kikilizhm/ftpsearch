

Cftps : source/main.o source/ftps.o source/sqlite.o
	gcc -o Cftps source/main.c source/ftps.c source/sqlite.c -Llib/  -lClog -lsqlite3 -lpthread -ldl -L./source

clean :
	rm -f *.o Cftps source/*o source/Cftps
