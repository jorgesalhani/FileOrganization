#ifndef INDICE_H
    #define INDICE_H

    #define TAMANHO_CHAVE_BUSCA 12

    #include "dadosIndice.h"
    #include "cabecalhoIndice.h"
    #include "../busca/arvoreBinaria.h"
    
    typedef struct indice_ INDICE;

    INDICE* indiceCriar(char* nomeArquivoIndice, char* modoAberturaArquivo);
    bool indiceExiste(INDICE* indice);
    bool indiceAtualizarCabecalho(INDICE* indice, CABECALHO_INDICE* cabecalhoIndice);
    bool indiceInteiroAtualizarDados(INDICE* indice, DADOS_INDICE_INTEIRO* dadosIndiceInteiro);
    bool indiceStringAtualizarDados(INDICE* indice, DADOS_INDICE_STRING* dadosIndiceString);
    char* indiceObterNomeArquivo(INDICE* indice);
    bool indiceFecharArquivo(INDICE* indice);
    bool indiceDeletar(INDICE** indice, bool manterArquivo);
    DADOS_INDICE_INTEIRO* indiceLerArmazenarDadosInteiro(INDICE* indice, char* tipoDado);
    DADOS_INDICE_STRING* indiceLerArmazenarDadosString(INDICE* indice, char* tipoDado);
    CABECALHO_INDICE* indiceLerArmazenarCabecalho(INDICE* indice);
    bool indiceArmazenarRegistrosOrdemCrescente(INDICE* indice, ARVORE_BINARIA* arvoreBinaria, char* tipoDado);
    INDICE* indiceCriarBinario(char* nomeArquivoEntrada, char* campoIndexado, char* tipoDado, char* nomeArquivoIndice);
    bool indiceImprimirBuscaBinaria(ARVORE_BINARIA* arvoreBinaria, char** listaCamposDeBusca, void** listaValoresDeBusca, int numeroParesCampoValor);
    
#endif