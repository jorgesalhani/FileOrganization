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
 *          TI: Trabalho Introdutorio
 *          =========================
 *          DADOS: leitura e armazenamento de dados presentes no arquivo .csv
 *          CABECALHO: leitura e armazenamento de metadados do arquivo .csv
 *          TABELA: escrita e atualizacao do arquivo de saida .bin
 *          FUNCOESFORNECIDAS: manipulacao e tratamento de entrada e saida
 * 
 *          T1: Trabalho 1
 *          ==============
 *          DADOSINDICE: leitura e armazenamento de dados de indices sobre campos
 *          CABECALHOINDICE: Leitura e armazenamento de metadados dos indices
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tabela/tabela.h"
#include "utilitarios/funcoesFornecidas.h"
#include "indice/indice.h"
#include "estrutura/arvoreBinaria.h"

void lerEntradasModo1(char* nomeArquivoEntrada, char* nomeArquivoSaida) {
  scanf("%s", nomeArquivoEntrada);
  scanf("%s", nomeArquivoSaida);
}

void lerEntradasModo2(char* nomeArquivoEntrada) {
  scanf("%s", nomeArquivoEntrada);
}

void lerEntradasModo3(char* nomeArquivoEntrada, char* campoIndexado, char* tipoDado, char* nomeArquivoIndice) {
  scanf("%s", nomeArquivoEntrada);
  scanf("%s", campoIndexado);
  scanf("%s", tipoDado);
  scanf("%s", nomeArquivoIndice);
}

int main(void) {
  int modo;
  char nomeArquivoEntrada[100], nomeArquivoSaida[100], nomeArquivoIndice[100];
  char campoIndexado[50], tipoDado[20];

  scanf("%d", &modo);

  TABELA* tabela = NULL;

  switch (modo)
  {
  case 1: ;
    lerEntradasModo1(nomeArquivoEntrada, nomeArquivoSaida);
    tabela = tabelaCriarBinario(nomeArquivoEntrada, nomeArquivoSaida);
    if (!tabelaExiste(tabela)) return 0;

    tabelaFecharArquivo(tabela);
    binarioNaTela(tabelaObterNomeArquivo(tabela)); 
    tabelaDeletar(&tabela, true);
    break;

  case 2:
    lerEntradasModo2(nomeArquivoEntrada);
    tabela = tabelaLerImprimirBinario(nomeArquivoEntrada);
    if (!tabelaExiste(tabela)) return 0;
    
    tabelaFecharArquivo(tabela);
    tabelaDeletar(&tabela, true);
    break;

  case 3:
    lerEntradasModo3(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice);
    INDICE* indice = indiceCriarBinario(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice);
    if (!indiceExiste(indice)) return 0;

    indiceFecharArquivo(indice);
    binarioNaTela(indiceObterNomeArquivo(indice)); 
    indiceDeletar(&indice, true);
    // lerBinario(nomeArquivoEntrada);
    // TABELA* tabela = tabelaCriarBinarioIndice(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice);
    // if (!tabelaExiste(tabela)) return 0;
    
    // tabelaFecharArquivo(tabela);
    // binarioNaTela(tabelaObterNomeArquivo(tabela));
    // tabelaDeletar(&tabela, true);
    break;
  
  default: ;
    erroModo();
    break;
  }

  return 0;
}