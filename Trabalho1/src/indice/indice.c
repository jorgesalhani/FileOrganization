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

bool indiceInteiroAtualizarDados(INDICE* indice, DADOS_INDICE_INTEIRO* dadosIndiceInteiro) {
    if (!indiceExiste(indice) || !dadosIndiceInteiroExiste(dadosIndiceInteiro)) return false;
    if (!indiceArquivoExiste(indice->arquivoBinario)) return false;

    FILE* arquivo = indice->arquivoBinario;

    uint64_t byteOffset = dadosIndiceInteiroObterByteOffset(dadosIndiceInteiro);
    uint32_t chaveBusca = dadosIndiceObterChaveBuscaInteiro(dadosIndiceInteiro);

    fwrite(&chaveBusca, sizeof(uint32_t), 1, arquivo);

    fwrite(&byteOffset, sizeof(uint64_t), 1, arquivo);
}

bool indiceStringAtualizarDados(INDICE* indice, DADOS_INDICE_STRING* dadosIndiceString) {
    if (!indiceExiste(indice) || !dadosIndiceStringExiste(dadosIndiceString)) return false;
    if (!indiceArquivoExiste(indice->arquivoBinario)) return false;

    FILE* arquivo = indice->arquivoBinario;

    uint64_t byteOffset = dadosIndiceStringObterByteOffset(dadosIndiceString);

    char chaveBuscaString[TAMANHO_CHAVE_BUSCA]; 
    strcpy(chaveBuscaString, dadosIndiceObterChaveBuscaString(dadosIndiceString));

    fwrite(chaveBuscaString, sizeof(char), TAMANHO_CHAVE_BUSCA, arquivo);
    fwrite(&byteOffset, sizeof(uint64_t), 1, arquivo);
}

char* indiceObterNomeArquivo(INDICE* indice) {
    if (!indiceExiste(indice)) return "-1";
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

bool liberarMemoriaCasoErro(TABELA** tabela, CABECALHO** cabecalho) {
    tabelaFecharArquivo(*tabela);
    tabelaDeletar(tabela, true);
    cabecalhoDeletar(cabecalho);
    return true;
}

INDICE* indiceCriarBinario(char* nomeArquivoEntrada, char* campoIndexado, char* tipoDado, char* nomeArquivoIndice) {

    TABELA* tabela = tabelaCriar(nomeArquivoEntrada, "rb");
    if(!tabelaExiste(tabela)) {
        liberarMemoriaCasoErro(&tabela, NULL);
        erroGenerico();
        return NULL;
    }

    CABECALHO* cabecalho = tabelaLerArmazenarCabecalho(tabela);
    if (!cabecalhoExiste(cabecalho)) {
        liberarMemoriaCasoErro(&tabela, NULL);
        return NULL;
    }

    DADOS_INDICE_INTEIRO* dadosIndiceInteiro = NULL;
    DADOS_INDICE_STRING* dadosIndiceString = NULL;
    
    if (tipoDadoStringValido(tipoDado)) {
        dadosIndiceString = dadosIndiceStringCriar(tipoDado, "$$$$$$$$$$$", 0);
    } else if (tipoDadoInteiroValido(tipoDado)) {
        dadosIndiceInteiro = dadosIndiceInteiroCriar(tipoDado, 0, 0);
    } else {
        liberarMemoriaCasoErro(&tabela, &cabecalho);
        erroGenerico();
        return NULL;
    }

    INDICE* indice = indiceCriar(nomeArquivoIndice, "wb+");
    if (!indiceExiste(indice)) {
        liberarMemoriaCasoErro(&tabela, &cabecalho);
        erroGenerico();
        return indice;
    }

    uint32_t nroRegArq = cabecalhoObterNroRegArq(cabecalho);

    CABECALHO_INDICE* cabecalhoIndice = cabecalhoIndiceCriar('0');
    indiceAtualizarCabecalho(indice, cabecalhoIndice);
    
    while(nroRegArq--) {

        DADOS* dados = tabelaLerArmazenarDado(tabela);
        if (!dadosExiste(dados)) continue;

        METADADOS* metadados = tabelaLerArmazenarMetadado(dados);
        if (tipoDadoStringValido(tipoDado)) {
            indiceStringAtualizarDados(indice, dadosIndiceString);
        } else {
            indiceInteiroAtualizarDados(indice, dadosIndiceInteiro);
        }

        dadosDeletar(&dados);
        dadosMetadadosDeletar(&metadados);
    }

    if (dadosIndiceStringExiste(dadosIndiceString)) dadosIndiceStringDeletar(&dadosIndiceString);
    else dadosIndiceInteiroDeletar(&dadosIndiceInteiro);

    cabecalhoIndiceDeletar(&cabecalhoIndice);

    cabecalhoDeletar(&cabecalho);
    tabelaFecharArquivo(tabela);
    tabelaDeletar(&tabela, true);
    return indice;
}


