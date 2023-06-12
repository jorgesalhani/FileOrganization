#include <stdlib.h>
#include <stdio.h>
#include "item.h"

struct item_ {
    int32_t chave;
    int64_t byteOffset;
    void* valorCampo;
    DADOS* dados;
    METADADOS* metadados;
};

bool itemExiste(ITEM* item) {
    return item != NULL ? true : false;
}

ITEM* itemCriar(int32_t chave, int64_t byteOffset, void* valorCampo, DADOS* dados, METADADOS* metadados) {
    if (!dadosExiste(dados) || !metadadosExiste(metadados) || valorCampo == NULL) return NULL;
    
    ITEM* item = (ITEM*) malloc(sizeof(ITEM));
    if (!itemExiste(item)) return NULL;

    item->chave = chave;
    item->valorCampo = valorCampo;
    item->dados = dados;
    item->metadados = metadados;
    item->byteOffset = byteOffset;

    return item;
}

bool itemAtualizarValorCampo(ITEM* item, void* valorCampo) {
    if (!itemExiste(item) || valorCampo == NULL) return false;
    item->valorCampo = valorCampo;
    return true;
}

int32_t itemObterChave(ITEM* item) {
    if (!itemExiste(item)) return -1;
    return item->chave;
}

int64_t itemObterByteOffset(ITEM* item) {
    if (!itemExiste(item)) return -1;
    return item->byteOffset;
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
