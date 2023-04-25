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

    arvoreBinaria->raiz = NULL;
    arvoreBinaria->campoIndexado = campoIndexado;

    return arvoreBinaria;
}

ITEM* arvoreBinariaObterItem(ARVORE_BINARIA* arvoreBinaria, int32_t chave) {

}

char* arvoreBinariaObterCampoIndexado(ARVORE_BINARIA* arvoreBinaria) {
    if (!arvoreBinariaExiste(arvoreBinaria)) return NULL;
    return arvoreBinaria->campoIndexado;
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

bool arvoreBinariaAdicionar(int32_t chave, ARVORE_BINARIA* arvoreBinaria, DADOS* dados, METADADOS* metadados, char* campoIndexado) {
    if (!arvoreBinariaExiste(arvoreBinaria) || !dadosExiste(dados) || !metadadosExiste(metadados)) return false;

    int indiceCampoEscolhido = dadosObterNumeroCampoIndexado(campoIndexado);
    void* valorCampoEscolhido = dadosObterCampoIndexado(dados, campoIndexado);

    ITEM* item = itemCriar(chave, valorCampoEscolhido, dados, metadados);
    if (!itemExiste(item)) return false;
    
    NO* novoNo = arvoreBinariaNoCriar(item);
    if (!arvoreBinariaNoExiste(novoNo)) return false;

    if (!arvoreBinariaNoExiste(arvoreBinaria->raiz)) {
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

bool arvoreBinariaArmazenarRegistrosOrdenados(ARVORE_BINARIA* arvoreBinaria, TABELA* tabela, CABECALHO* cabecalho) {
    if (!arvoreBinariaExiste(arvoreBinaria) || !tabelaExiste(tabela) || !cabecalhoExiste(cabecalho)) return false;

    int32_t nroRegArq = cabecalhoObterNroRegArq(cabecalho);
    int64_t byteOffset = 0;
    int32_t chave = 0;
    while(nroRegArq--) {
        DADOS* dados = tabelaLerArmazenarDado(tabela);
        METADADOS* metadados = tabelaLerArmazenarMetadado(dados);
        byteOffset += dadosMetadadosObterTamanhoRegistro(dados, metadados);
        if (!dadosExiste(dados)) continue;        
        arvoreBinariaAdicionar(chave, arvoreBinaria, dados, metadados, arvoreBinaria->campoIndexado);
        chave++;
    }

    return true;
}


bool armazenarRegistroOrdemCrescente(
    ITEM* item, INDICE* indice, ARVORE_BINARIA* arvoreBinaria, 
    char* tipoDado, uint64_t* byteOffset
) {
    if (!indiceExiste(indice) || !arvoreBinariaExiste(arvoreBinaria)) return false;

    DADOS_INDICE_INTEIRO* dadosIndiceInteiro = NULL;
    DADOS_INDICE_STRING* dadosIndiceString = NULL;

    char* campoIndexado = arvoreBinariaObterCampoIndexado(arvoreBinaria);
    int indiceCampoEscolhido = dadosObterNumeroCampoIndexado(campoIndexado);

    DADOS* dados = itemObterDados(item);
    METADADOS* metadados = itemObterMetadados(item);

    void* valorCampoEscolhido = dadosObterCampoIndexado(dados, campoIndexado);

    uint64_t valorByteOffset = *byteOffset;
    valorByteOffset += dadosMetadadosObterTamanhoRegistro(dados, metadados);
    *byteOffset = valorByteOffset;
    switch (indiceCampoEscolhido) {
        case 0:
            int32_t* campoInt = (int32_t*) valorCampoEscolhido;
            dadosIndiceInteiro = dadosIndiceInteiroCriar(tipoDado, *campoInt, valorByteOffset);
            
            if (*campoInt != -1) indiceInteiroAtualizarDados(indice, dadosIndiceInteiro);
            
            dadosIndiceInteiroDeletar(&dadosIndiceInteiro);
            break;
        case 1:
            char* campoStr = (char*) valorCampoEscolhido;
            if (strlen(campoStr) == 0) break;
            char* campoTruncado = dadosIndiceTruncarString(campoStr);
            dadosIndiceString = dadosIndiceStringCriar(tipoDado, campoTruncado, valorByteOffset);

            if (campoTruncado[0] != '$') indiceStringAtualizarDados(indice, dadosIndiceString);

            dadosIndiceStringDeletar(&dadosIndiceString);
            free(campoTruncado);
            break;
        default:
            break;
    }

    return true;
}

ITEM* arvoreBinariaObterItemOrdenadoAux(
    NO* no, INDICE* indice, ARVORE_BINARIA* arvoreBinaria, 
    char* tipoDado, uint64_t* byteOffset
) {
    if (!arvoreBinariaNoExiste(no)) return NULL;
    arvoreBinariaObterItemOrdenadoAux(no->esquerda, indice, arvoreBinaria, tipoDado, byteOffset);
    arvoreBinariaObterItemOrdenadoAux(no->direita, indice, arvoreBinaria, tipoDado, byteOffset);
    armazenarRegistroOrdemCrescente(no->item, indice, arvoreBinaria, tipoDado, byteOffset);
    return no->item;
}

bool indiceArmazenarRegistrosOrdemCrescente(INDICE* indice, ARVORE_BINARIA* arvoreBinaria, char* tipoDado) {
    if (!arvoreBinariaExiste(arvoreBinaria) || !indiceExiste(indice)) return false;
    uint64_t byteOffset = 0;
    arvoreBinariaObterItemOrdenadoAux(arvoreBinaria->raiz, indice, arvoreBinaria, tipoDado, &byteOffset);
    return true;
}

