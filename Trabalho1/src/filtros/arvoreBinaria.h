#ifndef ARVORE_BINARIA_H
    #define ARVORE_BINARIA_H

    #include "item.h"

    typedef struct arvore_binaria_ ARVORE_BINARIA;

    ARVORE_BINARIA* arvoreBinariaCriar(char* campoIndexado);
    bool arvoreBinariaExiste(ARVORE_BINARIA* arvoreBinaria);
    ITEM* arvoreBinariaObterItem(ARVORE_BINARIA* arvoreBinaria, int32_t chave);
    bool arvoreBinariaOrdenarPorCampo(ARVORE_BINARIA* arvoreBinaria, char* campoIndexado);
    bool arvoreBinariaAdicionar(ARVORE_BINARIA* arvoreBinaria, DADOS* dados, METADADOS* metadados);
    ITEM* arvoreBinariaRemoverItem(ARVORE_BINARIA* arvoreBinaria, int32_t chave);
    bool arvoreBinariaDeletar(ARVORE_BINARIA** arvoreBinaria);

#endif