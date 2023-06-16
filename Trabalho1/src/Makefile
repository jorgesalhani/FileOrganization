ARQS_O = \
	cabecalho.o \
	dados.o \
	tabela.o \
	cabecalhoIndice.o \
	dadosIndice.o \
	item.o \
	arvoreBinaria.o \
	indice.o \
	funcoesFornecidas.o \
	erros.o \
	main.o

BSH_COMPILAR_ALL = \
	gcc -g $(ARQS_O) -o exec -std=c99 -Wall; make clean;

all: $(ARQS_O)
	$(BSH_COMPILAR_ALL)

cabecalho.o:
	gcc -g -c tabela/cabecalho.c -o cabecalho.o -lmd

dados.o:
	gcc -g -c tabela/dados.c -o dados.o -lmd

tabela.o:
	gcc -g -c tabela/tabela.c -o tabela.o -lmd

cabecalhoIndice.o:
	gcc -g -c indice/cabecalhoIndice.c -o cabecalhoIndice.o -lmd

dadosIndice.o:
	gcc -g -c indice/dadosIndice.c -o dadosIndice.o -lmd

item.o:
	gcc -g -c busca/item.c -o item.o -lmd

arvoreBinaria.o:
	gcc -g -c busca/arvoreBinaria.c -o arvoreBinaria.o -lmd

indice.o:
	gcc -g -c indice/indice.c -o indice.o -lmd

funcoesFornecidas.o:
	gcc -g -c utilitarios/funcoesFornecidas.c -o funcoesFornecidas.o -lmd

erros.o:
	gcc -g -c utilitarios/erros.c -o erros.o -lmd

main.o:
	gcc -g -c main.c -o main.o -lmd

clean:
	rm *.o

run:
	valgrind ./exec < ../casosTeste/in/6.in