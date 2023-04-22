all: cabecalho.o dados.o tabela.o funcoesFornecidas.o erros.o main.o
	gcc -g cabecalho.o dados.o tabela.o funcoesFornecidas.o erros.o main.o -o exec -std=c99 -Wall; make clean

cabecalho.o:
	gcc -g -c tabela/cabecalho.c -o cabecalho.o -lmd

dados.o:
	gcc -g -c tabela/dados.c -o dados.o -lmd

tabela.o:
	gcc -g -c tabela/tabela.c -o tabela.o -lmd

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

