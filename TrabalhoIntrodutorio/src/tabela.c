#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tabela.h"

/********************
 * ESTRUTURA DE DADOS
 * ******************
*/

struct tabela_ {
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

bool tabelaExiste(TABELA* tabela) {
    return tabela != NULL ? true : false;
}

bool delimitadorValido(char delimitadorCampos) {
    return delimitadorCampos == '|' ? true : false;
}


/********************
 * FUNCOES PRINCIPAIS
 * ******************
*/
    
TABELA* tabelaCriar(char* nome) {
    if (!nomeValido(nome)) return NULL;
    FILE* arquivoBinario = fopen(nome, "wb+");
    if (!arquivoExiste(arquivoBinario)) return NULL;

    TABELA* tabela = (TABELA*) malloc(sizeof(TABELA));
    if (!tabelaExiste(tabela)) return NULL;

    tabela->arquivoBinario = arquivoBinario;
    tabela->nome = nome;
    return tabela;
}

bool tabelaAtualizarCabecalho(TABELA* tabela, CABECALHO* cabecalho) {
    if (!tabelaExiste(tabela) || !arquivoExiste(tabela->arquivoBinario) || 
        !cabecalhoExiste(cabecalho)) return false;

    FILE* arquivo = tabela->arquivoBinario;

    char status = cabecalhoObterStatus(cabecalho);
    uint64_t byteOffset = cabecalhoObterProxByteOffset(cabecalho);
    uint32_t nroRegArq = cabecalhoObterNroRegArq(cabecalho);
    uint32_t nroRegRem = cabecalhoObterNroRegRem(cabecalho);

    fwrite(&status, sizeof(char), 1, arquivo);
    fwrite(&byteOffset, sizeof(uint64_t), 1, arquivo);
    fwrite(&nroRegArq, sizeof(uint32_t), 1, arquivo);
    fwrite(&nroRegRem, sizeof(uint32_t), 1, arquivo);

    return true;
}

bool tabelaAtualizarDados(TABELA* tabela, DADOS* dados, char delimitadorCampos, char delimitadorRegistro) {
    if (!delimitadorValido(delimitadorCampos)) return false;
    if (!tabelaExiste(tabela) || !arquivoExiste(tabela->arquivoBinario) || 
        !dadosExiste(dados)) return false;

    FILE* arquivo = tabela->arquivoBinario;

    char removido = dadosObterRemovido(dados);
    uint32_t idCrime = dadosObterIdCrime(dados);
    char* dataCrime = dadosObterDataCrime(dados);
    uint32_t numeroArtigo = dadosObterNumeroArtigo(dados);
    char* marcaCelular = dadosObterMarcaCelular(dados);
    char* lugarCrime = dadosObterLugarCrime(dados);
    char* descricaoCrime = dadosObterDescricaoCrime(dados);
    char delimitador = delimitadorRegistro;

    fwrite(&removido, sizeof(char), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);

    fwrite(&idCrime, sizeof(uint32_t), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);

    fwrite(&dataCrime, sizeof(char), TAMANHO_DATA_CRIME, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);

    fwrite(&numeroArtigo, sizeof(uint32_t), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);

    fwrite(&marcaCelular, sizeof(char), TAMANHO_MARCA_CELULAR, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);

    fwrite(&lugarCrime, sizeof(lugarCrime), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);

    fwrite(&descricaoCrime, sizeof(descricaoCrime), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);
    
    fwrite(&delimitador, sizeof(char), 1, arquivo);

    return true;

}

char* tabelaObterNomeArquivo(TABELA* tabela) {
    if (!tabelaExiste(tabela) || !arquivoExiste(tabela->arquivoBinario)) return NULL;
    return tabela->nome;
}

bool tabelaFecharArquivo(TABELA* tabela) {
    if (!tabelaExiste(tabela) || !arquivoExiste(tabela->arquivoBinario)) return false;
    fclose(tabela->arquivoBinario);
    return true;
}

bool tabelaDeletar(TABELA** tabela, bool manterArquivo) {
    if (tabela == NULL || !tabelaExiste(*tabela) || 
        !arquivoExiste((*tabela)->arquivoBinario)) return false;
    
    fclose((*tabela)->arquivoBinario);
    
    if (!manterArquivo) remove((*tabela)->nome);
    free(*tabela);
    *tabela = NULL;
    tabela = NULL;
    return true;
}