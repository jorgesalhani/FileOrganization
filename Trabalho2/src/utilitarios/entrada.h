#ifndef ENTRADA_H
    #define ENTRADA_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>

    typedef struct entrada_ ENTRADA;

    ENTRADA* lerEntradas();
    bool entradaApagar(ENTRADA** entrada);

    int entradaObterFuncionalidade(ENTRADA* entrada);
    char* entradaObterArquivoEntrada(ENTRADA* entrada);
    char* entradaObterArquivoSaida(ENTRADA* entrada);
    char* entradaObterArquivoIndice(ENTRADA* entrada);
    char* entradaObterCampoIndexado(ENTRADA* entrada);
    char* entradaObterTipoDado(ENTRADA* entrada);
    int entradaObterNumeroCamposBuscados(ENTRADA* entrada);
    char** entradaObterLinhasBusca(ENTRADA* entrada);
    
#endif