#include "dadosIndice.h"

/*******************
 * ESTRUTURA DE DADO
 * *****************
*/

struct dados_indice_inteiro_ {
    int32_t chaveBusca;
    int64_t byteOffset;
};

struct dados_indice_string_ {
    char chaveBusca[TAMANHO_CHAVE_BUSCA];
    int64_t byteOffset;
};

/*********************
 * FUNCOES UTILITARIAS
 * *******************
*/

/********************
 * FUNCOES PRINCIPAIS
 * ******************
*/

bool tipoDadoStringValido(char* tipoDado) {
    return strcmp(tipoDado, "string") == 0 ? true : false;
}

bool tipoDadoInteiroValido(char* tipoDado) {
    return strcmp(tipoDado, "inteiro") == 0 ? true : false;
}

char* dadosIndiceTruncarString(char* valor) {
    int tamanhoString = strlen(valor);
    if (tamanhoString == 0) return NULL;

    char* valorTruncado = (char*) malloc(sizeof(char) * (TAMANHO_CHAVE_BUSCA + 1));

    int valor_i = 0;
    if (tamanhoString <= TAMANHO_CHAVE_BUSCA) {
        while (valor_i < tamanhoString) {
            valorTruncado[valor_i] = valor[valor_i];
            valor_i++;
        }

        while (valor_i < TAMANHO_CHAVE_BUSCA) {
            valorTruncado[valor_i] = '$';
            valor_i++;
        }
    } else {
        while (valor_i < TAMANHO_CHAVE_BUSCA) {
            valorTruncado[valor_i] = valor[valor_i];
            valor_i++;
        }
    }

    valorTruncado[TAMANHO_CHAVE_BUSCA] = '\0';
    return valorTruncado;
}

DADOS_INDICE_INTEIRO* dadosIndiceInteiroCriar(char* tipoDado, int32_t chaveBuscaInteiro, int64_t byteOffset) {
    if (!tipoDadoInteiroValido(tipoDado)) return NULL;
    DADOS_INDICE_INTEIRO* dadosIndice = (DADOS_INDICE_INTEIRO*) malloc(sizeof(DADOS_INDICE_INTEIRO));
    if (!dadosIndiceInteiroExiste(dadosIndice)) return NULL;

    dadosIndice->chaveBusca = chaveBuscaInteiro;
    dadosIndice->byteOffset = byteOffset;
    return dadosIndice;
}

DADOS_INDICE_STRING* dadosIndiceStringCriar(char* tipoDado, char chaveBuscaString[TAMANHO_CHAVE_BUSCA], int64_t byteOffset) {
    if (!tipoDadoStringValido(tipoDado)) return NULL;
    DADOS_INDICE_STRING* dadosIndice = (DADOS_INDICE_STRING*) malloc(sizeof(DADOS_INDICE_STRING));
    if (!dadosIndiceStringExiste(dadosIndice)) return NULL;

    strcpy(dadosIndice->chaveBusca, chaveBuscaString);
    dadosIndice->byteOffset = byteOffset;
    return dadosIndice;
}

bool dadosIndiceInteiroExiste(DADOS_INDICE_INTEIRO* dadosIndiceInteiro) {
    return dadosIndiceInteiro != NULL ? true : false;
}

bool dadosIndiceStringExiste(DADOS_INDICE_STRING* dadosIndiceString) {
    return dadosIndiceString != NULL ? true : false;
}

int32_t dadosIndiceObterChaveBuscaInteiro(DADOS_INDICE_INTEIRO* dadosIndiceInteiro) {
    if (!dadosIndiceInteiroExiste(dadosIndiceInteiro)) return -1;
    return dadosIndiceInteiro->chaveBusca;
}
char* dadosIndiceObterChaveBuscaString(DADOS_INDICE_STRING* dadosIndiceString) {
    if (!dadosIndiceStringExiste(dadosIndiceString)) return "$";
    return dadosIndiceString->chaveBusca;
}

int64_t dadosIndiceInteiroObterByteOffset(DADOS_INDICE_INTEIRO* dadosIndiceInteiro) {
    if (!dadosIndiceInteiroExiste(dadosIndiceInteiro)) return -1;
    return dadosIndiceInteiro->byteOffset;
}
int64_t dadosIndiceStringObterByteOffset(DADOS_INDICE_STRING* dadosIndiceString) {
    if (!dadosIndiceStringExiste(dadosIndiceString)) return -1;
    return dadosIndiceString->byteOffset;
}

bool dadosIndiceInteiroAtualizarChaveBusca(DADOS_INDICE_INTEIRO* dadosIndice, int32_t novaChaveBuscaInteiro) {
    if (!dadosIndiceInteiroExiste(dadosIndice)) return false;
    dadosIndice->chaveBusca = novaChaveBuscaInteiro;
    return true;
}

bool dadosIndiceStringAtualizarChaveBusca(DADOS_INDICE_STRING* dadosIndice, char novaChaveBuscaString[TAMANHO_CHAVE_BUSCA]) {
    if (!dadosIndiceStringExiste(dadosIndice)) return false;
    strcpy(dadosIndice->chaveBusca, novaChaveBuscaString);
    return true;
}

bool dadosIndiceInteiroAtualizarByteOffset(DADOS_INDICE_INTEIRO* dadosIndice, int64_t novoByteOffset) {
    if (!dadosIndiceInteiroExiste(dadosIndice)) return false;
    dadosIndice->byteOffset = novoByteOffset;
    return true;
}

bool dadosIndiceStringAtualizarByteOffset(DADOS_INDICE_STRING* dadosIndice, int64_t novoByteOffset) {
    if (!dadosIndiceStringExiste(dadosIndice)) return false;
    dadosIndice->byteOffset = novoByteOffset;
    return true;
}

bool dadosIndiceInteiroDeletar(DADOS_INDICE_INTEIRO** dadosIndiceInteiro) {
    if (dadosIndiceInteiro == NULL || !dadosIndiceInteiroExiste(*dadosIndiceInteiro)) return false;
    free(*dadosIndiceInteiro);
    *dadosIndiceInteiro = NULL;
    dadosIndiceInteiro = NULL;
    return true;
}

bool dadosIndiceStringDeletar(DADOS_INDICE_STRING** dadosIndiceString) {
    if (dadosIndiceString == NULL || !dadosIndiceStringExiste(*dadosIndiceString)) return false;
    free(*dadosIndiceString);
    *dadosIndiceString = NULL;
    dadosIndiceString = NULL;
    return true;
}