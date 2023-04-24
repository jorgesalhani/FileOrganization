#ifndef ITEM_H
    #define ITEM_H

    #include "../indice/indice.h"
    #include "../tabela/tabela.h"

    typedef struct item_ ITEM;

    ITEM* itemCriar(int32_t chave, DADOS* dados, METADADOS* metadados);
    bool itemExiste(ITEM* item);
    int32_t itemObterChave(ITEM* item);
    DADOS* itemObterDados(ITEM* item);
    METADADOS* itemObterMetadados(ITEM* item);
    bool itemAtualizarChave(ITEM* item, int32_t novaChave);
    bool itemDeletar(ITEM** item);

#endif