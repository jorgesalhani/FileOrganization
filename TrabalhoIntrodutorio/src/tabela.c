#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tabela.h"
#include "erros.h"

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
    fflush(arquivo);

    return true;
}

bool tabelaAtualizarDados(TABELA* tabela, DADOS* dados, METADADOS* metadados, 
                          char delimitadorCampos) {
    if (!delimitadorValido(delimitadorCampos)) return false;
    if (!tabelaExiste(tabela) || !arquivoExiste(tabela->arquivoBinario) || 
        !dadosExiste(dados)) return false;

    FILE* arquivo = tabela->arquivoBinario;

    char removido = dadosObterRemovido(dados);
    uint32_t idCrime = dadosObterIdCrime(dados);
    char* dataCrime = dadosObterDataCrime(dados);
    uint32_t numeroArtigo = dadosObterNumeroArtigo(dados);
    char* marcaCelular = dadosObterMarcaCelular(dados);
    char delimitador = dadosObterDelimitadorRegistro(dados);

    int tamanhoLugarCrime = dadosMetadadosObterTamanhoLugarCrime(metadados);
    char lugarCrime[tamanhoLugarCrime];
    for (int i = 0; i < tamanhoLugarCrime; i++) lugarCrime[i] = dadosObterLugarCrime(dados)[i];

    int tamanhoDescricaoCrime = dadosMetadadosObterTamanhoDescricaoCrime(metadados);
    char descricaoCrime[tamanhoDescricaoCrime];
    for (int i = 0; i < tamanhoDescricaoCrime; i++) descricaoCrime[i] = dadosObterDescricaoCrime(dados)[i];

    fwrite(&removido, sizeof(char), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);
    fflush(arquivo);
    
    fwrite(&idCrime, sizeof(uint32_t), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);
    fflush(arquivo);

    fwrite(dataCrime, sizeof(char), TAMANHO_DATA_CRIME, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);
    fflush(arquivo);

    fwrite(&numeroArtigo, sizeof(uint32_t), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);
    fflush(arquivo);

    fwrite(marcaCelular, sizeof(char), TAMANHO_MARCA_CELULAR, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);
    fflush(arquivo);

    fwrite(lugarCrime, sizeof(lugarCrime), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);
    fflush(arquivo);

    fwrite(descricaoCrime, sizeof(descricaoCrime), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);    
    fflush(arquivo);
    
    fwrite(&delimitador, sizeof(char), 1, arquivo);
    fflush(arquivo);
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

bool tabelaCriarBinario(char* nomeEntrada, char* nomeSaida) {
    FILE *crimesDados = fopen(nomeEntrada, "r");
    if (crimesDados == NULL) erroGenerico();

    char str[256] = "";
    TABELA* tabela = tabelaCriar(nomeSaida);

    CABECALHO* cabecalho = cabecalhoCriar('0', 1, 2, 3);
    DADOS* dados = dadosCriar(0, "$$$$$$$$$$", 0, "$$$$$$$$$$$$", "", "", '0');
    METADADOS* metadados = dadosCriarMetadados();

    int tamanhoRegistroDados = 0;
    uint64_t tamanhoRegistroCabecalho = 0;
    uint32_t novoNroRegArq = 0;
    uint32_t novoNroRegRem = 0;
    

    tabelaAtualizarCabecalho(tabela, cabecalho);
    
    fgets(str, 256, crimesDados);
    while(fgets(str, 256, crimesDados)) {
        if(str[0] == '\n')
        continue;

        int j = 0, k = 0;
        char token[256] = "";
        for(int i = 0; i < strlen(str); i++) {
        if(str[i] == '\n' || str[i] == ',' || (i == strlen(str)-1 && feof(crimesDados))) {
            // printf("%s\n", str);
            
            if(i == strlen(str)-1 && feof(crimesDados)) {
            token[j] = str[i];
            }
            
            switch(k) {
            case 0:
                if(strcmp(token, "") == 0)
                erroGenerico(); 
                // printf("%d\n", atol(token));
                dadosAtualizarIdCrime(dados, atoi(token));
                break;
            case 1:
                dadosAtualizarDataCrime(dados, token);
                break;
            case 2:
                if(strlen(token) == 0)
                dadosAtualizarNumeroArtigo(dados, -1);
                else
                dadosAtualizarNumeroArtigo(dados, atoi(token));
                break;
            case 3:
                dadosAtualizarLugarCrime(dados, token, metadados);
                break;
            case 4:
                dadosAtualizarDescricaoCrime(dados, token, metadados);
                break;
            case 5:
                dadosAtualizarMarcaCelular(dados, token);
            }
            k++;
            memset(token, 0, strlen(token));
            j = 0;
            continue;
        }
        token[j] = str[i];
        //printf("%c\n", token[j]);
        j++;
        }
    
        tabelaAtualizarDados(tabela, dados, metadados, '|');

        novoNroRegArq++;
        
        if (tamanhoRegistroDados == 0) {
            tamanhoRegistroDados = dadosMetadadosObterTamanhoRegistro(dados, metadados);
        }

        if (dadosObterRemovido(dados) == '1') {
            novoNroRegRem = (int32_t)(novoNroRegRem + 1);
        }

        // dadosImprimir(dados);
        // printf("=====\n");
    }

    fseek(tabela->arquivoBinario, 0, SEEK_SET);

    cabecalhoAtualizarNroRegArq(cabecalho, novoNroRegArq);
    cabecalhoAtualizarNroRegRem(cabecalho, novoNroRegRem);
    cabecalhoAtualizarStatus(cabecalho, '1');

    tamanhoRegistroCabecalho = cabecalhoObterTamanhoRegistro(cabecalho);
    cabecalhoAtualizarProxByteOffset(cabecalho, (uint64_t)(tamanhoRegistroCabecalho + tamanhoRegistroDados));

    tabelaAtualizarCabecalho(tabela, cabecalho);

    fclose(crimesDados);

    dadosMetadadosDeletar(&metadados);
    dadosDeletar(&dados);
    cabecalhoDeletar(&cabecalho);
    tabelaDeletar(&tabela, true);

    return true;
}