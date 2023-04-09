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
    if (!tabelaExiste(tabela)) {
      return 0;
    }

    tabelaFecharArquivo(tabela);
    binarioNaTela(tabelaObterNomeArquivo(tabela)); 
    tabelaDeletar(&tabela, true);
    break;

  case 2:
    lerEntradasModo2(nomeArquivoEntrada);
    lerBinario(nomeArquivoEntrada);
    break;
  
  default: ;
    erroModo();
    break;
  }

  return 0;
}