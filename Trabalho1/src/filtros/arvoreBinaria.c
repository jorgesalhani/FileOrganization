#include <stdio.h>
#include <stdlib.h>
#include "arvoreBinaria.h"
#include "item.h"

typedef struct no_ NO;

struct no_ {
    ITEM* item;
    NO* esquerda;
    NO* direita;
};

struct arvore_binaria_ {
    char* campoIndexado;
    NO* raiz;
};

bool arvoreBinariaExiste(ARVORE_BINARIA* arvoreBinaria) {
    return arvoreBinaria != NULL ? true : false;
}

bool arvoreBinariaNoExiste(NO* no) {
    return no != NULL ? true : false;
}

bool arvoreBinariaNoDeletar(NO** no) {
    if (no == NULL || !arvoreBinariaNoExiste(*no)) return false;
    itemDeletar(&((*no)->item));
    free(*no);
    *no = NULL;
    no = NULL;
    return true;
}

bool arvoreBinariaDeletarAux(NO* no) {
    if (!arvoreBinariaNoExiste(no)) return false;

    arvoreBinariaDeletarAux(no->direita);
    arvoreBinariaDeletarAux(no->esquerda);
    arvoreBinariaNoDeletar(&no);
    return true;
}

NO* arvoreBinariaNoCriar(ITEM* item) {
    if (!itemExiste(item)) return NULL;

    NO* no = (NO*) malloc(sizeof(NO));
    if (!arvoreBinariaNoExiste(no)) return NULL;

    no->direita = NULL;
    no->esquerda = NULL;
    no->item = item;
    return no;
}

bool arvoreBinariaAdicionarAux(NO* raiz, DADOS* dados, METADADOS* metadados, char* campoIndexado) {
    int indiceCampoEscolhido = dadosObterNumeroCampoIndexado(campoIndexado);
    void* campoEscolhido = dadosObterCampoIndexado(dados, campoIndexado);

    ITEM* item = itemCriar(0, dados, metadados);
    if (!itemExiste(item)) return false;
    
    NO* no = arvoreBinariaNoCriar(item);
    raiz->direita = no;
    if (!arvoreBinariaNoExiste(no)) return false;    
}

ARVORE_BINARIA* arvoreBinariaCriar(char* campoIndexado) {
    if (!dadosCampoIndexadoValido(campoIndexado)) return NULL;

    ARVORE_BINARIA* arvoreBinaria = (ARVORE_BINARIA*) malloc(sizeof(ARVORE_BINARIA));
    if (!arvoreBinariaExiste(arvoreBinaria)) return NULL;

    NO* raiz = (NO*) malloc(sizeof(NO));
    if (!arvoreBinariaNoExiste(raiz)) return NULL;

    arvoreBinaria->raiz = raiz;
    arvoreBinaria->campoIndexado = campoIndexado;
    raiz->direita = NULL;
    raiz->esquerda = NULL;
    raiz->item = NULL;

    return arvoreBinaria;
}

ITEM* arvoreBinariaObterItem(ARVORE_BINARIA* arvoreBinaria, int32_t chave) {

}

bool arvoreBinariaOrdenarPorCampo(ARVORE_BINARIA* arvoreBinaria, char* campoIndexado) {

}

bool arvoreBinariaAdicionar(ARVORE_BINARIA* arvoreBinaria, DADOS* dados, METADADOS* metadados) {
    if (!arvoreBinariaExiste(arvoreBinaria) || !dadosExiste(dados) || !metadadosExiste(metadados)) return false;
    arvoreBinariaAdicionarAux(arvoreBinaria->raiz, dados, metadados, arvoreBinaria->campoIndexado);

    return true;
}

ITEM* arvoreBinariaRemoverItem(ARVORE_BINARIA* arvoreBinaria, int32_t chave) {

}

bool arvoreBinariaDeletar(ARVORE_BINARIA** arvoreBinaria) {
    if (arvoreBinaria == NULL || !arvoreBinariaExiste(*arvoreBinaria)) return false;

    arvoreBinariaDeletarAux((*arvoreBinaria)->raiz);
    free(*arvoreBinaria);
    *arvoreBinaria = NULL;
    arvoreBinaria = NULL;
    return true;
}
