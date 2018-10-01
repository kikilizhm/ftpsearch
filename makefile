

test : source/main.o 
	gcc -o test source/main.o 
main.o : source/main.c
	gcc -c source/main.c
clean : 
	rm *.o source/*o source/test
