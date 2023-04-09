/**
 * **********************************************************************
 * @public Codigo desenvolvido para Disciplina de Organizacao de Arquivos
 * 
 * @author Henrique Garcia Gomes do Ó
 *         no USP: 10734331
 * 
 * @author Jorge Augusto Salgado Salhani
 *         no USP: 8927418
 *  
 * @date Abril, 2023
 * **********************************************************************
 * 
 * @brief Obter dados de um arquivo .csv de entrada e gerar um
 *        arquivo binario com os mesmos
 * 
 * @details A aplicacao usa as TADs
 *          DADOS: leitura e armazenamento de dados presentes no arquivo .csv
 *          CABECALHO: leitura e armazenamento de metadados do arquivo .csv
 *          TABELA: escrita e atualizacao do arquivo de saida .bin
 *          FUNCOESFORNECIDAS: manipulacao e tratamento de entrada e saida
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "cabecalho.h"
#include "dados.h"
#include "funcoesFornecidas.h"
#include "tabela.h"
#include "erros.h"

#define STR_VAZIA "NULO"

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
  }
  char lixo[256];
  char charAux = '0';
  char removidoAux;
  uint32_t nroRegArq; 
  int idAux, numArtAux, i = 0;
  char dataAux[TAMANHO_DATA_CRIME+1], marcaAux[TAMANHO_MARCA_CELULAR+1];
  char descricaoAux[64], lugarAux[64], numArtAuxPrint[5]; 
  
  fread(lixo, sizeof(char), 9, binarioDados);
  fread(&nroRegArq, sizeof(uint32_t), 1, binarioDados);
  fread(lixo, sizeof(char), 4, binarioDados);

  if(nroRegArq == 0) {
    fclose(binarioDados);
    erroSemRegistros();
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

void lerEntradasModo1(char* nomeArquivoEntrada, char* nomeArquivoSaida) {
  scanf("%s", nomeArquivoEntrada);
  scanf("%s", nomeArquivoSaida);
}

void lerEntradasModo2(char* nomeArquivoEntrada) {
  scanf("%s", nomeArquivoEntrada);
}

int main(void) {
  int modo;
  char nomeArquivoEntrada[50], nomeArquivoSaida[50];
  scanf("%d", &modo);

  switch (modo)
  {
  case 1: ;
    lerEntradasModo1(nomeArquivoEntrada, nomeArquivoSaida);
    TABELA* tabela = tabelaCriarBinario(nomeArquivoEntrada, nomeArquivoSaida);
    if (!tabelaExiste(tabela)) erroGenerico();

    tabelaFecharArquivo(tabela);
    binarioNaTela(tabelaObterNomeArquivo(tabela)); 
    tabelaDeletar(&tabela, true);
    break;

  case 2:
    lerEntradasModo2(nomeArquivoEntrada);
    lerBinario(nomeArquivoEntrada);
    break;
  
  default: ;
    break;
  }

  return 0;
}