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

void erroGenerico() {
  printf("Falha no processamento do arquivo.");
  exit(1);
}

void erroSemRegistros() {
  printf("Registro inexistente.");
  exit(1);
}

void erroModo() {
  printf("Modo inexistente");
  exit(1);
}

void binarioCria(char* entrada, char *saida) {
  FILE *crimesDados = fopen(entrada, "r");
  char str[256];

  CABECALHO* cabecalho = cabecalhoCriar('0', 1, 2, 3);
  DADOS* dados = dadosCriar('0', 0, "$$$$$$$$$$", 0, "$$$$$$$$$$$$", "", "");

  
  fgets(str, 256, crimesDados);
  while(fgets(str, 256, crimesDados)) {
    if(str[0] == '\n')
      continue;
    int j = 0, k = 0;
    char token[256];
    for(int i = 0; i < strlen(str); i++) {
      if(str[i] == '\n' || str[i] == ',' || (i == strlen(str)-1 && feof(crimesDados))) {
        
        if(i == strlen(str)-1 && feof(crimesDados)) {
          token[j] = str[i];
        }
        
        switch(k) {
          case 0:
            if(strcmp(token, "") == 0)
              erroGenerico(); 
            dadosAtualizarIdCrime(dados, atoi(token));
            break;
          case 1:
            dadosAtualizarDataCrime(dados, token);
            break;
          case 2:
            if(strcmp(token, "") == 0)
              dadosAtualizarNumeroArtigo(dados, -1);
            else
              dadosAtualizarNumeroArtigo(dados, atoi(token));
            break;
          case 3:
            dadosAtualizarLugarCrime(dados, token);
            break;
          case 4:
            dadosAtualizarDescricaoCrime(dados, token);
            break;
          case 5:
            dadosAtualizarMarcaCelular(dados, token);
        }
        k++;
        memset(token,0,strlen(token));
        j = 0;
        continue;
      }
      token[j] = str[i];
      //printf("%c\n", token[j]);
      j++;
    }
    
    dadosImprimir(dados);
    printf("=====\n");
  }
  fclose(crimesDados);
  dadosDeletar(&dados);
}

int main(void) {
  // char entrada[64], saida[64];
  // int modo;
  // scanf("%d %s %s", &modo, entrada, saida);

  // switch(modo) {
  //   case 1:
  //     binarioCria(entrada, saida);
  //     binarioNaTela(saida);
  //     break;
  //   default:
  //     erroModo();
  // }
  
  
  
  
  
  binarioCria("arquivos/antes/dados1.csv", "lol");
  
  
  // CABECALHO* cabecalho = cabecalhoCriar(
  //     '0', 1, 2, 3
  // );

  // DADOS* dados = dadosCriar(
  //     '0', 2, "01/02/2082", 1, "celularA", "Lugar Crime", "Descricao Crime"
  // );


  // TABELA* tabela = tabelaCriar("out.bin");
  // tabelaAtualizarCabecalho(tabela, cabecalho);
  // tabelaAtualizarDados(tabela, dados, '|');
  // tabelaDeletar(&tabela, true);

  // cabecalhoDeletar(&cabecalho);
  // dadosDeletar(&dados);

  //binarioNaTela("out.bin");


    return 0;
}