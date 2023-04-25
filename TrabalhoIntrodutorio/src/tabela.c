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
    int64_t byteOffset = cabecalhoObterProxByteOffset(cabecalho);
    int32_t nroRegArq = cabecalhoObterNroRegArq(cabecalho);
    int32_t nroRegRem = cabecalhoObterNroRegRem(cabecalho);

    fwrite(&status, sizeof(char), 1, arquivo);
    fwrite(&byteOffset, sizeof(int64_t), 1, arquivo);
    fwrite(&nroRegArq, sizeof(int32_t), 1, arquivo);
    fwrite(&nroRegRem, sizeof(int32_t), 1, arquivo);

    return true;
}

bool tabelaAtualizarDados(TABELA* tabela, DADOS* dados, METADADOS* metadados, 
                          char delimitadorCampos) {
    if (!delimitadorValido(delimitadorCampos)) return false;
    if (!tabelaExiste(tabela) || !arquivoExiste(tabela->arquivoBinario) || 
        !dadosExiste(dados)) return false;

    FILE* arquivo = tabela->arquivoBinario;

    char removido = dadosObterRemovido(dados);
    int32_t idCrime = dadosObterIdCrime(dados);
    char* dataCrime = dadosObterDataCrime(dados);
    int32_t numeroArtigo = dadosObterNumeroArtigo(dados);
    char* marcaCelular = dadosObterMarcaCelular(dados);
    char delimitador = dadosObterDelimitadorRegistro(dados);

    int64_t tamanhoLugarCrime = dadosMetadadosObterTamanhoLugarCrime(metadados);
    char lugarCrime[tamanhoLugarCrime];
    for (int64_t i = 0; i < tamanhoLugarCrime; i++) lugarCrime[i] = dadosObterLugarCrime(dados)[i];

    int64_t tamanhoDescricaoCrime = dadosMetadadosObterTamanhoDescricaoCrime(metadados);
    char descricaoCrime[tamanhoDescricaoCrime];
    for (int64_t i = 0; i < tamanhoDescricaoCrime; i++) descricaoCrime[i] = dadosObterDescricaoCrime(dados)[i];

    fwrite(&removido, sizeof(char), 1, arquivo);
    
    fwrite(&idCrime, sizeof(int32_t), 1, arquivo);

    fwrite(dataCrime, sizeof(char), TAMANHO_DATA_CRIME, arquivo);

    fwrite(&numeroArtigo, sizeof(int32_t), 1, arquivo);

    fwrite(marcaCelular, sizeof(char), TAMANHO_MARCA_CELULAR, arquivo);

    fwrite(lugarCrime, sizeof(lugarCrime), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);

    fwrite(descricaoCrime, sizeof(descricaoCrime), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);    
    
    fwrite(&delimitador, sizeof(char), 1, arquivo);
    return true;

}

char* tabelaObterNomeArquivo(TABELA* tabela) {
    if (!tabelaExiste(tabela)) return NULL;
    return tabela->nome;
}

bool tabelaFecharArquivo(TABELA* tabela) {
    if (!tabelaExiste(tabela) || !arquivoExiste(tabela->arquivoBinario)) return false;
    fclose(tabela->arquivoBinario);
    tabela->arquivoBinario = NULL;
    return true;
}

bool tabelaDeletar(TABELA** tabela, bool manterArquivo) {
    if (tabela == NULL || !tabelaExiste(*tabela)) return false;
    
    if (arquivoExiste((*tabela)->arquivoBinario)) fclose((*tabela)->arquivoBinario);
    
    if (!manterArquivo) remove((*tabela)->nome);
    free(*tabela);
    *tabela = NULL;
    tabela = NULL;
    return true;
}

TABELA* tabelaCriarBinario(char* nomeEntrada, char* nomeSaida) {
    FILE *crimesDados = fopen(nomeEntrada, "r");
    if (crimesDados == NULL) {
        erroGenerico();
        return NULL;
    }

    char str[256] = "";
    TABELA* tabela = tabelaCriar(nomeSaida);

    CABECALHO* cabecalho = cabecalhoCriar('0', 1, 2, 3);
    DADOS* dados = dadosCriar(0, "$$$$$$$$$$", 0, "$$$$$$$$$$$$", "", "", '0');
    METADADOS* metadados = dadosCriarMetadados();

    int32_t tamanhoRegistroDados = 0;
    int64_t tamanhoRegistroCabecalho = 0;
    int32_t novoNroRegArq = 0;
    int32_t novoNroRegRem = 0;
    

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
                if(strcmp(token, "") == 0) {
                    erroGenerico(); 
                    return NULL;
                }
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
        
        tamanhoRegistroDados += dadosMetadadosObterTamanhoRegistro(dados, metadados);

        if (dadosObterRemovido(dados) == '1') {
            novoNroRegRem = (int32_t)(novoNroRegRem + 1);
        }

        // dadosImprimir(dados);
        // printf("=====\n");
    }

    tamanhoRegistroCabecalho = cabecalhoObterTamanhoRegistro(cabecalho);
    int64_t novoProxByteOffset = (int64_t)(tamanhoRegistroDados + tamanhoRegistroCabecalho);

    fseek(tabela->arquivoBinario, 0, SEEK_SET);

    cabecalhoAtualizarStatus(cabecalho, '1');
    cabecalhoAtualizarProxByteOffset(cabecalho, novoProxByteOffset);
    cabecalhoAtualizarNroRegArq(cabecalho, novoNroRegArq);
    cabecalhoAtualizarNroRegRem(cabecalho, novoNroRegRem);

    tabelaAtualizarCabecalho(tabela, cabecalho);

    fclose(crimesDados);

    dadosMetadadosDeletar(&metadados);
    dadosDeletar(&dados);
    cabecalhoDeletar(&cabecalho);

    return tabela;
}

bool strVazia(char* str) {
  if(strcmp(str, "") == 0 || str[0] == '$') {
    strcpy(str, STR_VAZIA);
    return true;
  }
  return false;
}

void lerBinario(char* entrada) {
  FILE *binarioDados = fopen(entrada, "rb");
  if(binarioDados == NULL) {
    erroGenerico();
    return;
  }
  char lixo[256];
  char charAux = '0';
  char statusAux = '0';
  char removidoAux;
  int32_t nroRegArq; 
  int idAux, numArtAux, i = 0;
  char dataAux[TAMANHO_DATA_CRIME+1], marcaAux[TAMANHO_MARCA_CELULAR+1];
  char descricaoAux[64], lugarAux[64], numArtAuxPrint[5]; 
  
  fread(&statusAux, sizeof(char), 1, binarioDados);
  fread(lixo, sizeof(char), 8, binarioDados);
  fread(&nroRegArq, sizeof(int32_t), 1, binarioDados);
  fread(lixo, sizeof(char), 4, binarioDados);

  if (statusAux == '0') {
    fclose(binarioDados);
    erroGenerico();
    return;
  }
  
  if(nroRegArq == 0) {
    fclose(binarioDados);
    erroSemRegistros();
    return;
  }

  while(nroRegArq--) {

    fread(&removidoAux, sizeof(char), 1, binarioDados);
    if(removidoAux == '1') {
      fread(lixo, sizeof(char), 30, binarioDados);
      while(charAux != '#') {
        fread(&charAux, sizeof(char), 1, binarioDados);
      }
      charAux = '\0';
      continue;
    }

    
    fread(&idAux, sizeof(int), 1, binarioDados);
    
    fread(dataAux, sizeof(char), TAMANHO_DATA_CRIME , binarioDados);
    dataAux[TAMANHO_DATA_CRIME] = '\0';
    strVazia(dataAux); 
    
    fread(&numArtAux, sizeof(int), 1, binarioDados);
    
    fread(marcaAux, sizeof(char), TAMANHO_MARCA_CELULAR, binarioDados);
    marcaAux[TAMANHO_MARCA_CELULAR] = '\0';
    if(!strVazia(marcaAux)) {
      for(int j = 0; j < strlen(marcaAux); j++) {
        if(marcaAux[j] == '$') {
          marcaAux[j] = '\0';
          break;
        }
      }
    }
    i=0;
    
    while(1) {
      fread(&lugarAux[i], sizeof(char), 1, binarioDados);
      if(lugarAux[i] == '|') {
        lugarAux[i] = '\0';
        break;
      }
      i++;
    }
    strVazia(lugarAux);
    
    i=0;
    
    while(1) {
      fread(&descricaoAux[i], sizeof(char), 1, binarioDados);
      if(descricaoAux[i] == '|') {
        descricaoAux[i] = '\0';
        break;
      }
      i++;
    }
    strVazia(descricaoAux);

    fread(&charAux, sizeof(char), 1, binarioDados);
    
    if(numArtAux == -1) {
      printf("%d, %s, %s, %s, %s, %s\n", idAux, dataAux, STR_VAZIA, lugarAux, descricaoAux, marcaAux); 
    } else {
      printf("%d, %s, %d, %s, %s, %s\n", idAux, dataAux, numArtAux, lugarAux, descricaoAux, marcaAux); 
    }

    memset(dataAux,0,strlen(dataAux));
    memset(marcaAux,0,strlen(marcaAux));
    memset(lugarAux,0,strlen(lugarAux));
    memset(descricaoAux,0,strlen(descricaoAux));
    charAux = '\0';
  }
  fclose(binarioDados);
}