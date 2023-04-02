all: cabecalho.o dados.o funcoesFornecidas.o main.o
	gcc -g cabecalho.o dados.o funcoesFornecidas.o main.o -o exec -std=c99 -Wall; make clean

cabecalho.o:
	gcc -g -c cabecalho.c -o cabecalho.o

dados.o:
	gcc -g -c dados.c -o dados.o

funcoesFornecidas.o:
	gcc -g -c funcoesFornecidas.c -o funcoesFornecidas.o

main.o:
	gcc -g -c main.c -o main.o

clean:
	rm *.o

run:
	valgrind ./exec < ../casosTeste/in/1.in