#include <stdlib.h>
#include "dadosIndice.h"
#include <string.h>

/*******************
 * ESTRUTURA DE DADO
 * *****************
*/

struct dados_indice_ {
    uint32_t chaveBuscaInteiro;
    char chaveBuscaString[TAMANHO_CHAVE_BUSCA];
    uint64_t byteOffset;
};

/*********************
 * FUNCOES UTILITARIAS
 * *******************
*/

bool tipoDadoValido(char* tipoDado) {
    return (tipoDado != "string" && tipoDado != "inteiro") ? false : true;
}

bool dadosIndiceEntradasValidas(
    char* tipoDado, char chaveBuscaString[TAMANHO_CHAVE_BUSCA], uint64_t byteOffset
) {
    if (!tipoDadoValido(tipoDado) || !chaveBuscaStringValida(chaveBuscaString)) return false;
    return true;
}

/********************
 * FUNCOES PRINCIPAIS
 * ******************
*/

bool chaveBuscaStringValida(char chaveBuscaString[TAMANHO_CHAVE_BUSCA]) {
    return strlen(chaveBuscaString) <= TAMANHO_CHAVE_BUSCA ? true : false;
}

bool chaveBuscaInteiroValida(uint32_t chaveBuscaInteiro) {
    return chaveBuscaInteiro >= 0 ? true : false;
}

DADOS_INDICE* dadosIndiceCriar(
    char* tipoDado, uint32_t chaveBuscaInteiro, char chaveBuscaString[TAMANHO_CHAVE_BUSCA], uint64_t byteOffset
) {
    if (!tipoDadoValido(tipoDado)) return NULL;
    if (strcmp(tipoDado, "string") == 0) {
        chaveBuscaInteiro = -1;
    } else {
        chaveBuscaString = "$$$$$$$$$$$$";
    }
    if (!dadosIndiceEntradasValidas(tipoDado, chaveBuscaString, byteOffset)) return NULL;

    DADOS_INDICE* dadosIndice = (DADOS_INDICE*) malloc(sizeof(DADOS_INDICE));
    if (!dadosIndiceExiste(dadosIndice)) return NULL;
    
    dadosIndice->chaveBuscaInteiro = chaveBuscaInteiro;
    strcpy(dadosIndice->chaveBuscaString, chaveBuscaString);
    dadosIndice->byteOffset = byteOffset;
    
    return dadosIndice;
}

bool dadosIndiceExiste(DADOS_INDICE* dadosIndice) {
    return dadosIndice != NULL ? true : false;
}

uint32_t dadosIndiceObterChaveBuscaInteiro(DADOS_INDICE* dadosIndice) {
    if (!dadosIndiceExiste(dadosIndice)) return -1;
    return dadosIndice->chaveBuscaInteiro;
}

char* dadosIndiceObterChaveBuscaString(DADOS_INDICE* dadosIndice) {
    if (!dadosIndiceExiste(dadosIndice)) return "$";
    return dadosIndice->chaveBuscaString;
}

uint64_t dadosIndiceObterByteOffset(DADOS_INDICE* dadosIndice) {
    if (!dadosIndiceExiste(dadosIndice)) return -1;
    return dadosIndice->byteOffset;
}
bool dadosIndiceAtualizarChaveBuscaString(DADOS_INDICE* dadosIndice, char novaChaveBuscaString[TAMANHO_CHAVE_BUSCA]) {
    if (!dadosIndiceExiste(dadosIndice) || !chaveBuscaStringValida(novaChaveBuscaString)) return false;
    strcpy(dadosIndice->chaveBuscaString, novaChaveBuscaString);
    return true;
}

bool dadosIndiceAtualizarChaveBuscaInteiro(DADOS_INDICE* dadosIndice, uint32_t novaChaveBuscaInteiro) {
    if (!dadosIndiceExiste(dadosIndice)) return false;
    dadosIndice->chaveBuscaInteiro = novaChaveBuscaInteiro;
    return true;
}

bool dadosIndiceAtualizarByteOffset(DADOS_INDICE* dadosIndice, uint64_t novoByteOffset) {
    if (!dadosIndiceExiste(dadosIndice)) return false;
    dadosIndice->byteOffset = novoByteOffset;
    return true;
}

bool dadosIndiceDeletar(DADOS_INDICE** dadosIndice) {
    if (dadosIndice == NULL || !dadosIndiceExiste(*dadosIndice)) return false;
    free(*dadosIndice);
    *dadosIndice = NULL;
    dadosIndice = NULL;
    return true;
}
