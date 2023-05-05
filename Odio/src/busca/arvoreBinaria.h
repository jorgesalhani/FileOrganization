#ifndef ARVORE_BINARIA_H
    #define ARVORE_BINARIA_H

    #include "item.h"
    #include "../indice/indice.h"

    typedef struct arvore_binaria_ ARVORE_BINARIA;

    ARVORE_BINARIA* arvoreBinariaCriar(char* campoIndexado);
    bool arvoreBinariaExiste(ARVORE_BINARIA* arvoreBinaria);
    bool arvoreBinariaVazia(ARVORE_BINARIA* arvoreBinaria);
    ITEM* arvoreBinariaObterItem(ARVORE_BINARIA* arvoreBinaria, char* campo, void* valor);
    bool arvoreBinariaImprimirBusca(ARVORE_BINARIA* arvoreBinaria, char* campo, void* valor);
    ITEM* arvoreBinariaRemoverItem(ARVORE_BINARIA* arvoreBinaria, int32_t chave);
    bool arvoreBinariaDeletar(ARVORE_BINARIA** arvoreBinaria);
    bool arvoreBinariaArmazenarRegistrosOrdenados(ARVORE_BINARIA* arvoreBinaria, TABELA* tabela, CABECALHO* cabecalho);
    char* arvoreBinariaObterCampoIndexado(ARVORE_BINARIA* arvoreBinaria);
    bool arvoreBinariaArmazenarOrdemCrescente(ARVORE_BINARIA* arvoreBinaria, INDICE* indice, char* tipoDado);
    bool arvoreBinariaAdicionar(ARVORE_BINARIA* arvoreBinaria, DADOS* dados, METADADOS* metadados, int32_t chave, int64_t byteOffset, char* campoIndexado);

#endif