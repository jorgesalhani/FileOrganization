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
    if (itemExiste((*no)->item)) itemDeletar(&((*no)->item));
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

bool arvoreBinariaNoAdicionar() {

}

bool arvoreBinariaOrdenarPorCampo(NO* raiz, NO* novoNo, char* campoIndexado, int indiceCampoEscolhido) {
    if (!arvoreBinariaNoExiste(raiz) || !arvoreBinariaNoExiste(novoNo)) return false;

    DADOS* dadosNoRaiz = itemObterDados(raiz->item);
    DADOS* dadosNo = itemObterDados(novoNo->item);


    void* valorCampoEscolhidoRaiz = dadosObterCampoIndexado(dadosNoRaiz, campoIndexado);
    void* valorCampoEscolhido = dadosObterCampoIndexado(dadosNo, campoIndexado);

    switch (indiceCampoEscolhido) {
        case 0:
            int32_t* campoIntRaiz = (int32_t*) valorCampoEscolhidoRaiz;
            int32_t* campoInt = (int32_t*) valorCampoEscolhido;
            if (*campoInt > *campoIntRaiz) {
                if (!arvoreBinariaNoExiste(raiz->direita)) {
                    raiz->direita = novoNo;
                    return true;
                } else {
                    return arvoreBinariaOrdenarPorCampo(raiz->direita, novoNo, campoIndexado, indiceCampoEscolhido);
                }
            } else {
                if (!arvoreBinariaNoExiste(raiz->esquerda)) {
                        raiz->esquerda = novoNo;
                        return true;
                } else {
                    return arvoreBinariaOrdenarPorCampo(raiz->esquerda, novoNo, campoIndexado, indiceCampoEscolhido);
                }
            }
            break;

        case 1:
            char* campoStrRaiz = (char*) valorCampoEscolhidoRaiz;
            char* campoStr = (char*) valorCampoEscolhido;
            if (strcmp(campoStr, campoStrRaiz) > 0) {
                if (!arvoreBinariaNoExiste(raiz->direita)) {
                    raiz->direita = novoNo;
                    return true;
                } else {
                    return arvoreBinariaOrdenarPorCampo(raiz->direita, novoNo, campoIndexado, indiceCampoEscolhido);
                }
            } else {
                if (!arvoreBinariaNoExiste(raiz->esquerda)) {
                        raiz->esquerda = novoNo;
                        return true;
                } else {
                    return arvoreBinariaOrdenarPorCampo(raiz->esquerda, novoNo, campoIndexado, indiceCampoEscolhido);
                }
            }
            break;

        default:
            break;
    }
}

bool arvoreBinariaAdicionar(ARVORE_BINARIA* arvoreBinaria, DADOS* dados, METADADOS* metadados, char* campoIndexado) {
    if (!arvoreBinariaExiste(arvoreBinaria) || !dadosExiste(dados) || !metadadosExiste(metadados)) return false;

    int indiceCampoEscolhido = dadosObterNumeroCampoIndexado(campoIndexado);
    void* valorCampoEscolhido = dadosObterCampoIndexado(dados, campoIndexado);

    ITEM* item = itemCriar(0, valorCampoEscolhido, dados, metadados);
    if (!itemExiste(item)) return false;
    
    NO* novoNo = arvoreBinariaNoCriar(item);
    if (!arvoreBinariaNoExiste(novoNo)) return false;

    if (!itemExiste(arvoreBinaria->raiz->item)) {
        arvoreBinaria->raiz = novoNo;
        return true;
    }

    arvoreBinariaOrdenarPorCampo(arvoreBinaria->raiz, novoNo, campoIndexado, indiceCampoEscolhido);

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
