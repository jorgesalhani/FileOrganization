ARQS_O = \
	entrada.o \
	dados.o \
	erros.o \
	indice.o \
	funcoesFornecidas.o \
	main.o

BSH_COMPILAR_ALL = \
	gcc -g $(ARQS_O) -o exec -std=c99 -Wall; make clean;

all: $(ARQS_O)
	$(BSH_COMPILAR_ALL)

entrada.o:
	gcc -g -c utilitarios/entrada.c -o entrada.o -lmd

funcoesFornecidas.o:
	gcc -g -c utilitarios/funcoesFornecidas.c -o funcoesFornecidas.o -lmd

erros.o:
	gcc -g -c utilitarios/erros.c -o erros.o -lmd

indice.o:
	gcc -g -c indice/indice.c -o indice.o -lmd

dados.o:
	gcc -g -c dados/dados.c -o dados.o -lmd

main.o:
	gcc -g -c main.c -o main.o -lmd

clean:
	rm *.o

run:
	valgrind ./exec 