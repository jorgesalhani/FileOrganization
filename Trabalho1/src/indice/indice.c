#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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

bool indiceNomeValido(char* nome) {
    return strlen(nome) > 0 ? true : false;
}

bool indiceArquivoExiste(FILE* arquivo) {
    return arquivo != NULL ? true : false;
}


/********************
 * FUNCOES PRINCIPAIS
 * ******************
*/

INDICE* indiceCriar(char* nomeArquivoIndice, char* modoAberturaArquivo) {
    if (!indiceNomeValido(nomeArquivoIndice)) return NULL;
    if (!modoAbrirArquivoValido(modoAberturaArquivo)) return NULL;
    INDICE* indice = (INDICE*) malloc(sizeof(INDICE));
    FILE* arquivoBinario = fopen(nomeArquivoIndice, modoAberturaArquivo);

    if (!indiceExiste(indice) || !indiceArquivoExiste(arquivoBinario)) return NULL;

    indice->nome = nomeArquivoIndice;
    indice->arquivoBinario = arquivoBinario;
    return indice;
}

bool indiceExiste(INDICE* indice) {
    return indice != NULL ? true : false;
}

bool indiceAtualizarCabecalho(INDICE* indice, CABECALHO_INDICE* cabecalhoIndice) {
    if (!indiceExiste(indice) || !cabecalhoIndiceExiste(cabecalhoIndice) || !indiceArquivoExiste(indice->arquivoBinario)) return false;
    
    FILE* arquivo = indice->arquivoBinario;

    char status = cabecalhoIndiceObterStatus(cabecalhoIndice);
    fwrite(&status, sizeof(char), 1, arquivo);
    return true;
}

bool indiceAtualizarDados(INDICE* indice, DADOS_INDICE* dadosIndice) {
    if (!indiceExiste(indice) || !dadosIndiceExiste(dadosIndice)) return false;
    if (!indiceArquivoExiste(indice->arquivoBinario)) return false;

    FILE* arquivo = indice->arquivoBinario;

    uint64_t byteOffset = dadosIndiceObterByteOffset(dadosIndice);
    char chaveBuscaString[TAMANHO_CHAVE_BUSCA]; 
    strcpy(chaveBuscaString, dadosIndiceObterChaveBuscaString(dadosIndice));
    uint32_t chaveBusca = dadosIndiceObterChaveBuscaInteiro(dadosIndice);

    if (chaveBuscaInteiroValida(chaveBusca)) {
        fwrite(&chaveBusca, sizeof(uint32_t), 1, arquivo);
    } else {
        fwrite(chaveBuscaString, sizeof(char), TAMANHO_CHAVE_BUSCA, arquivo);
    }
    fwrite(&byteOffset, sizeof(uint64_t), 1, arquivo);
}

char* indiceObterNomeArquivo(INDICE* indice) {
    if (!indiceExiste(indice) || !indiceArquivoExiste(indice->arquivoBinario)) return "-1";
    return indice->nome;
}

bool indiceFecharArquivo(INDICE* indice) {
    if (!indiceExiste(indice) || !indiceArquivoExiste(indice->arquivoBinario)) return false;
    fclose(indice->arquivoBinario);
    indice->arquivoBinario = NULL;
    return true;
}

bool indiceDeletar(INDICE** indice, bool manterArquivo) {
    if (indice == NULL || !indiceExiste(*indice)) return false;
    
    if (indiceArquivoExiste((*indice)->arquivoBinario)) fclose((*indice)->arquivoBinario);
    
    if (!manterArquivo) remove((*indice)->nome);
    free(*indice);
    *indice = NULL;
    indice = NULL;
    return true;
}

INDICE* indiceCriarBinario(char* nomeArquivoEntrada, char* campoIndexado, char* tipoDado, char* nomeArquivoIndice) {
    INDICE* indice = indiceCriar(nomeArquivoIndice, "wb+");
    if (!indiceExiste(indice)) {
        erroGenerico();
        return indice;
    }

    TABELA* tabela = tabelaCriar(nomeArquivoEntrada, "rb");
    if(!tabelaExiste(tabela)) {
        tabelaFecharArquivo(tabela);
        tabelaDeletar(&tabela, true);
        erroGenerico();
        return indice;
    }

    CABECALHO* cabecalho = tabelaLerArmazenarCabecalho(tabela);
    if (!cabecalhoExiste(cabecalho)) {
        tabelaFecharArquivo(tabela);
        tabelaDeletar(&tabela, true);
        return indice;
    }

    uint32_t nroRegArq = cabecalhoObterNroRegArq(cabecalho);

    while(nroRegArq--) {

        DADOS* dados = tabelaLerArmazenarDado(tabela);
        if (!dadosExiste(dados)) continue;

        METADADOS* metadados = tabelaLerArmazenarMetadado(dados);
        
        

        dadosDeletar(&dados);
        dadosMetadadosDeletar(&metadados);
    }

    cabecalhoDeletar(&cabecalho);
    tabelaFecharArquivo(tabela);
    tabelaDeletar(&tabela, true);
    return indice;
}


