#ifndef ARVORE_BINARIA_H
    #define ARVORE_BINARIA_H

    #include "item.h"

    typedef struct arvore_binaria_ ARVORE_BINARIA;

    ARVORE_BINARIA* arvoreBinariaCriar(char* campoIndexado);
    bool arvoreBinariaExiste(ARVORE_BINARIA* arvoreBinaria);
    ITEM* arvoreBinariaObterItem(ARVORE_BINARIA* arvoreBinaria, int32_t chave);
    ITEM* arvoreBinariaRemoverItem(ARVORE_BINARIA* arvoreBinaria, int32_t chave);
    bool arvoreBinariaDeletar(ARVORE_BINARIA** arvoreBinaria);
    bool arvoreBinariaArmazenarRegistrosOrdenados(ARVORE_BINARIA* arvoreBinaria, TABELA* tabela, CABECALHO* cabecalho);
    char* arvoreBinariaObterCampoIndexado(ARVORE_BINARIA* arvoreBinaria);
    bool indiceArmazenarRegistrosOrdemCrescente(INDICE* indice, ARVORE_BINARIA* arvoreBinaria, char* tipoDado);
    bool arvoreBinariaAdicionar(ARVORE_BINARIA* arvoreBinaria, DADOS* dados, METADADOS* metadados, int32_t chave, int64_t byteOffset, char* campoIndexado);
    int arvoreBinariaImprimirBusca(ARVORE_BINARIA* arvoreBinaria, char* campoIndexado, char** listaCamposDeBusca, void** listaValoresDeBusca, int numeroParesCampoValor);

#endif