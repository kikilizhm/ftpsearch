

Cftps : source/main.o source/ftps.o 
	gcc -o Cftps source/main.c source/ftps.c  -Llib/  -lClog

clean :
	rm -f *.o Cftps source/*o source/Cftps
