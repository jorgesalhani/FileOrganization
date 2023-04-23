all: cabecalho.o dados.o tabela.o cabecalhoIndice.o dadosIndice.o indice.o funcoesFornecidas.o erros.o main.o
	gcc -g cabecalho.o dados.o tabela.o cabecalhoIndice.o dadosIndice.o indice.o funcoesFornecidas.o erros.o main.o -o exec -std=c99 -Wall; make clean

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
	valgrind ./exec

