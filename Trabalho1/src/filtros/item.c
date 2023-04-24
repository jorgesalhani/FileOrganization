#include <stdlib.h>
#include <stdio.h>
#include "item.h"

struct item_ {
    int32_t chave;
    DADOS* dados;
    METADADOS* metadados;
};

bool itemExiste(ITEM* item) {
    return item != NULL ? true : false;
}

ITEM* itemCriar(int32_t chave, DADOS* dados, METADADOS* metadados) {
    if (!dadosExiste(dados) || !metadadosExiste(metadados)) return NULL;
    
    ITEM* item = (ITEM*) malloc(sizeof(ITEM));
    if (!itemExiste(item)) return NULL;

    item->chave = chave;
    item->dados = dados;
    item->metadados = metadados;

    return item;
}

int32_t itemObterChave(ITEM* item) {
    if (!itemExiste(item)) return -1;
    return item->chave;
}

DADOS* itemObterDados(ITEM* item) {
    if (!itemExiste(item)) return NULL;
    return item->dados;
}

METADADOS* itemObterMetadados(ITEM* item) {
    if (!itemExiste(item)) return NULL;
    return item->metadados;
}

bool itemAtualizarChave(ITEM* item, int32_t novaChave) {
    if (!itemExiste(item)) return false;
    item->chave = novaChave;
    return true;
}

bool itemDeletar(ITEM** item) {
    if (item == NULL || !itemExiste(*item)) return false;
    if (dadosExiste((*item)->dados)) dadosDeletar(&((*item)->dados));
    if (metadadosExiste((*item)->metadados)) dadosMetadadosDeletar(&((*item)->metadados));
    
    free(*item);
    *item = NULL;
    item = NULL;
    return true;
}
