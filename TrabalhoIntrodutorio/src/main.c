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
#include "cabecalho.h"
#include "dados.h"
#include "funcoesFornecidas.h"
#include "tabela.h"

void erroGenerico() {
  printf("Falha no processamento do arquivo.");
  exit(1);
}

void erroSemRegistros() {
  printf("Registro inexistente.");
  exit(1);
}

void binarioCria() {
  FILE *crimesDados = fopen("../arquivos/antes/dados1.csv", "r");
  if (crimesDados == NULL) erroGenerico();
  char lixo[100];
  char str[256];

  // DADOS* dados = dadosCriar(0, 0, "", 0, "", "", "");
  
  fgets(str, 256, crimesDados);
  while(fgets(str, 256, crimesDados)) {
    int j = 0;
    char strAux[256];
    for(int i = 0; i < strlen(str); i++) {
      if(str[i] == '\n' || str[i] == ',' || (i == strlen(str)-1 && feof(crimesDados))) {
        
        if(i == strlen(str)-1 && feof(crimesDados)) {
          strAux[j] = str[i];
        }
        
        printf("%s\n", strAux);
        memset(strAux,0,strlen(strAux));
        j = 0;
        continue;
      }
      strAux[j] = str[i];
      //printf("%c\n", strAux[j]);
      j++;
    }
     printf("=====\n");
  }

void funcionalidadeCriarTabela() {

}

void funcionalidadeSelect() {

}
  fclose(crimesDados);
}

void funcionalidadeCriarTabela() {

}

void funcionalidadeSelect() {

}

int main(int argc, char* argv[]) {

  // if (argc < 2) erroGenerico();
  // if (atoi(argv[1]) == 1) funcionalidadeCriarTabela();
  // if (atoi(argv[1]) == 2) funcionalidadeSelect();


  // // binarioCria();
  
  
  CABECALHO* cabecalho = cabecalhoCriar(
      '0', 1, 2, 3
  );

  DADOS* dados = dadosCriar(
      // 2, "01/02/2082", 1, "celularABBB", "Lugar Crime", "Descricao Crime", '0'
      445, "15/12/2018", 155, "SAMSUNG$$$$", "SAO PAULO", "FURTO (ART. 155) - OUTROS", '0'
  );


  TABELA* tabela = tabelaCriar("out.bin");
  tabelaAtualizarCabecalho(tabela, cabecalho);
  tabelaAtualizarDados(tabela, dados, '|', '#');
  tabelaDeletar(&tabela, true);

  cabecalhoDeletar(&cabecalho);
  dadosDeletar(&dados);

  binarioNaTela("out.bin");


    return 0;
}