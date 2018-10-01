

test : source/main.o 
	gcc -o test source/main.o 
main.o : source/main.c
	gcc -c source/main.c
clean :
	rm -f *.o test source/*o source/test
