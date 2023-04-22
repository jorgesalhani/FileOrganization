#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "indice.h"

/********************
 * ESTRUTURA DE DADOS
 * ******************
*/

struct indice_ {
    char* nome;
    FILE* arquivoBinario;
};

/*********************
 * FUNCOES UTILITARIAS
 * *******************
*/

bool nomeValido(char* nome) {
    return strlen(nome) > 0 ? true : false;
}

bool arquivoExiste(FILE* arquivo) {
    return arquivo != NULL ? true : false;
}

bool indiceExiste(INDICE* indice) {
    return indice != NULL ? true : false;
}


/********************
 * FUNCOES PRINCIPAIS
 * ******************
*/

INDICE* indiceCriar(char* nomeArquivoIndice) {
    if (!nomeValido(nomeArquivoIndice)) return NULL;
    
    INDICE* indice = (INDICE*) malloc(sizeof(INDICE));
    FILE* arquivoBinario = fopen(nomeArquivoIndice, "wb+");

    if (!indiceExiste(indice) || !arquivoExiste(arquivoBinario)) return NULL;

    indice->nome = nomeArquivoIndice;
    indice->arquivoBinario = arquivoBinario;
    return indice;
}

bool indiceExiste(INDICE* indice) {
    return indice != NULL ? true : false;
}

bool indiceAtualizarCabecalho(INDICE* indice, CABECALHO_INDICE* cabecalhoIndice) {
    if (!indiceExiste(indice) || !cabecalhoIndiceExiste(cabecalhoIndice) || !arquivoExiste(indice->arquivoBinario)) return false;
    
    FILE* arquivo = indice->arquivoBinario;

    char status = cabecalhoIndiceObterStatus(cabecalhoIndice);
    fwrite(&status, sizeof(char), 1, arquivo);
    return true;
}

bool indiceAtualizarDados(INDICE* indice, DADOS_INDICE* dadosIndice) {
    if (!indiceExiste(indice) || !dadosIndiceExiste(dadosIndice)) return false;
    if (!arquivoExiste(indice->arquivoBinario)) return false;

    FILE* arquivo = indice->arquivoBinario;

    uint64_t byteOffset = dadosIndiceObterByteOffset(dadosIndice);
    char chaveBuscaString[TAMANHO_CHAVE_BUSCA] = dadosIndiceObterChaveBuscaString(dadosIndice);
    uint32_t chaveBusca = dadosIndiceObterChaveBuscaInteiro(dadosIndice);

    if (chaveBuscaInteiroValida(chaveBusca)) {
        fwrite(&chaveBusca, sizeof(uint32_t), 1, arquivo);
    } else {
        fwrite(chaveBuscaString, sizeof(char), TAMANHO_CHAVE_BUSCA, arquivo);
    }
    fwrite(&byteOffset, sizeof(uint64_t), 1, arquivo);
}

char* indiceObterNomeArquivo(INDICE* indice) {
    if (!indiceExiste(indice) || !arquivoExiste(indice->arquivoBinario)) return "-1";
    return indice->nome;
}

bool indiceFecharArquivo(INDICE* indice) {
    if (!indiceExiste(indice) || !arquivoExiste(indice->arquivoBinario)) return false;
    fclose(indice->arquivoBinario);
    indice->arquivoBinario = NULL;
    return true;
}

bool indiceDeletar(INDICE** indice, bool manterArquivo) {
    if (indice == NULL || !indiceExiste(*indice)) return false;
    
    if (arquivoExiste((*indice)->arquivoBinario)) fclose((*indice)->arquivoBinario);
    
    if (!manterArquivo) remove((*indice)->nome);
    free(*indice);
    *indice = NULL;
    indice = NULL;
    return true;
}


