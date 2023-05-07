#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "indice.h"
#include "../busca/arvoreBinaria.h"

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
    int32_t qtdReg = cabecalhoIndiceObterQtdReg(cabecalhoIndice);
    
    fwrite(&status, sizeof(char), 1, arquivo);
    fwrite(&qtdReg, sizeof(int32_t), 1, arquivo);

    return true;
}

bool indiceInteiroAtualizarDados(INDICE* indice, DADOS_INDICE_INTEIRO* dadosIndiceInteiro) {
    if (!indiceExiste(indice) || !dadosIndiceInteiroExiste(dadosIndiceInteiro)) return false;
    if (!indiceArquivoExiste(indice->arquivoBinario)) return false;

    FILE* arquivo = indice->arquivoBinario;

    int64_t byteOffset = dadosIndiceInteiroObterByteOffset(dadosIndiceInteiro);
    int32_t chaveBusca = dadosIndiceObterChaveBuscaInteiro(dadosIndiceInteiro);

    fwrite(&chaveBusca, sizeof(int32_t), 1, arquivo);
    fwrite(&byteOffset, sizeof(int64_t), 1, arquivo);
}

bool indiceStringAtualizarDados(INDICE* indice, DADOS_INDICE_STRING* dadosIndiceString) {
    if (!indiceExiste(indice) || !dadosIndiceStringExiste(dadosIndiceString)) return false;
    if (!indiceArquivoExiste(indice->arquivoBinario)) return false;

    FILE* arquivo = indice->arquivoBinario;

    int64_t byteOffset = dadosIndiceStringObterByteOffset(dadosIndiceString);

    char* chaveBuscaString = dadosIndiceObterChaveBuscaString(dadosIndiceString);

    fwrite(chaveBuscaString, sizeof(char), TAMANHO_CHAVE_BUSCA, arquivo);
    fwrite(&byteOffset, sizeof(int64_t), 1, arquivo);
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

bool liberarMemoriaCasoErro(TABELA** tabela, CABECALHO** cabecalho, ARVORE_BINARIA** arvoreBinaria) {
    tabelaFecharArquivo(*tabela);
    tabelaDeletar(tabela, true);
    cabecalhoDeletar(cabecalho);
    arvoreBinariaDeletar(arvoreBinaria);
    return true;
}

bool indiceResetLeituraArquivoBinario(INDICE* indice) {
  if (!indiceExiste(indice) || !indiceArquivoExiste(indice->arquivoBinario)) return false;
  fseek(indice->arquivoBinario, 0, SEEK_SET);
  return true;
}

INDICE* indiceCriarBinario(char* nomeArquivoEntrada, char* campoIndexado, char* tipoDado, char* nomeArquivoIndice) {

    if (!dadosCampoIndexadoValido(campoIndexado)) {
        erroGenerico();
        return NULL;
    }

    TABELA* tabela = tabelaCriar(nomeArquivoEntrada, "rb");
    if(!tabelaExiste(tabela)) {
        erroGenerico();
        return NULL;
    }

    CABECALHO* cabecalho = tabelaLerArmazenarCabecalho(tabela);
    if (!cabecalhoExiste(cabecalho)) {
        liberarMemoriaCasoErro(&tabela, NULL, NULL);
        return NULL;
    }

    ARVORE_BINARIA* arvoreBinaria = arvoreBinariaCriar(campoIndexado);
    if (!arvoreBinariaExiste(arvoreBinaria)) {
        liberarMemoriaCasoErro(&tabela, &cabecalho, NULL);
        erroGenerico();
        return NULL;
    }

    arvoreBinariaArmazenarRegistrosOrdenados(arvoreBinaria, tabela, cabecalho, tipoDado);

    INDICE* indice = indiceCriar(nomeArquivoIndice, "wb+");
    if (!indiceExiste(indice)) {
        liberarMemoriaCasoErro(&tabela, &cabecalho, &arvoreBinaria);
        erroGenerico();
        return indice;
    }

    CABECALHO_INDICE* cabecalhoIndice = cabecalhoIndiceCriar('0');
    if (!cabecalhoIndiceExiste(cabecalhoIndice)) {
        liberarMemoriaCasoErro(&tabela, &cabecalho, &arvoreBinaria);
        erroGenerico();
        return indice;
    }

    indiceAtualizarCabecalho(indice, cabecalhoIndice);

    indiceArmazenarRegistrosOrdemCrescente(indice, arvoreBinaria, tipoDado);


    int32_t qtdRegExistentes = arvoreBinariaObterQtdReg(arvoreBinaria);
    arvoreBinariaDeletar(&arvoreBinaria);

    // if (dadosIndiceStringExiste(dadosIndiceString)) dadosIndiceStringDeletar(&dadosIndiceString);
    // else dadosIndiceInteiroDeletar(&dadosIndiceInteiro);

    fseek(indice->arquivoBinario, 0, SEEK_SET);

    cabecalhoIndiceAtualizarStatus(cabecalhoIndice, '1');
    cabecalhoIndiceAtualizarQtdReg(cabecalhoIndice, qtdRegExistentes);
    
    indiceAtualizarCabecalho(indice, cabecalhoIndice);

    cabecalhoIndiceDeletar(&cabecalhoIndice);

    cabecalhoDeletar(&cabecalho);
    tabelaFecharArquivo(tabela);
    tabelaDeletar(&tabela, true);
    return indice;
}

DADOS_INDICE_INTEIRO* indiceLerArmazenarDadosInteiro(INDICE* indice, char* tipoDado) {
    if (!indiceExiste(indice) || !tipoDadoInteiroValido(tipoDado)) return NULL;

    FILE* arquivoIndice = indice->arquivoBinario;

    int32_t chaveBuscaInteiro = 0;
    int64_t byteOffset = 0;

    DADOS_INDICE_INTEIRO* dadosIndiceInteiro = dadosIndiceInteiroCriar(tipoDado, chaveBuscaInteiro, byteOffset);
    if (!dadosIndiceInteiroExiste(dadosIndiceInteiro)) return NULL;

    fread(&chaveBuscaInteiro, sizeof(int32_t), 1, arquivoIndice);
    fread(&byteOffset, sizeof(int64_t), 1, arquivoIndice);

    dadosIndiceInteiroAtualizarByteOffset(dadosIndiceInteiro, byteOffset);
    dadosIndiceInteiroAtualizarChaveBusca(dadosIndiceInteiro, chaveBuscaInteiro);

    return dadosIndiceInteiro;
}

DADOS_INDICE_STRING* indiceLerArmazenarDadosString(INDICE* indice, char* tipoDado) {
    if (!indiceExiste(indice) || !tipoDadoStringValido(tipoDado)) return NULL;

    FILE* arquivoIndice = indice->arquivoBinario;

    char chaveBuscaString[TAMANHO_CHAVE_BUSCA] = "";
    int64_t byteOffset = 0;

    DADOS_INDICE_STRING* dadosIndiceString = dadosIndiceStringCriar(tipoDado, chaveBuscaString, byteOffset);
    if (!dadosIndiceStringExiste(dadosIndiceString)) return NULL;

    fread(chaveBuscaString, sizeof(char), TAMANHO_CHAVE_BUSCA, arquivoIndice);
    fread(&byteOffset, sizeof(int64_t), 1, arquivoIndice);

    dadosIndiceStringAtualizarByteOffset(dadosIndiceString, byteOffset);
    dadosIndiceStringAtualizarChaveBusca(dadosIndiceString, chaveBuscaString);

    return dadosIndiceString;
}

CABECALHO_INDICE* indiceLerArmazenarCabecalho(INDICE* indice) {
    if (!indiceExiste(indice)) return NULL;

    FILE* arquivoIndice = indice->arquivoBinario;

    CABECALHO_INDICE* cabecalhoIndice = cabecalhoIndiceCriar('0');
    if (!cabecalhoIndiceExiste(cabecalhoIndice)) return NULL;

    char status = '0';
    int32_t qtdReg = 0;

    fread(&status, sizeof(char), 1, arquivoIndice);
    fread(&qtdReg, sizeof(int32_t), 1, arquivoIndice);

    cabecalhoIndiceAtualizarStatus(cabecalhoIndice, status);
    cabecalhoIndiceAtualizarQtdReg(cabecalhoIndice, qtdReg);

    return cabecalhoIndice;
}

