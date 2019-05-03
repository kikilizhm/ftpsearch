

ftps : source/main.o source/ftps.o 
	gcc -o test source/main.c source/ftps.c  -Llib/  -lClog

clean :
	rm -f *.o ftps source/*o source/ftps