#ifndef ENTRADA_H
    #define ENTRADA_H

    #define TAMANHO_CHAVE_BUSCA 12

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>
    #include <stdint.h>
    #include "funcoesFornecidas.h"

    typedef struct entrada_ ENTRADA;

    enum camposIndexados { 
        idCrime, dataCrime, numeroArtigo, marcaCelular, 
        lugarCrime, descricaoCrime, removido, delimitadorCampos,
        delimitador, tamanhoRegistro,
        ENUM_FIM
    };

    static char* camposIndexadosTipos[ENUM_FIM] = {
        "inteiro", "string", "inteiro", "string", 
        "string", "string", "char", "char", 
        "char", "inteiro"
    };

    static char* camposIndexadosNomes[ENUM_FIM] = {
        "idCrime", "dataCrime", "numeroArtigo", "marcaCelular", 
        "lugarCrime", "descricaoCrime", "removido", "delimitadorCampos", 
        "delimitador", "tamanhoRegistro"
    };

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
    char* entradaProximaLinhaDeBusca(ENTRADA* entrada);
    char* entradaObterLinhaDeBusca(ENTRADA* entrada);
    char* entradaObterLinhaDeAtualizacao(ENTRADA* entrada);

    char* dadosObterTipoCampo(char* campoIndexado);
    
#endif