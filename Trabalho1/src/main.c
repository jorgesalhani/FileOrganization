/**
 * **********************************************************************
 * @public Codigo desenvolvido para Disciplina de Organizacao de Arquivos
 * 
 * @author Henrique Garcia Gomes do Ó
 *         no USP: 10734331
 *         Funcionalidades: 5, 6        - 60%
 * 
 * @author Jorge Augusto Salgado Salhani
 *         no USP: 8927418
 *         Funcionalidades: 3, 4, 7     - 70%
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
#include "busca/arvoreBinaria.h"

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

void lerEntradasModo4(char* nomeArquivoEntrada, char* campoIndexado, 
  char* tipoDado, char* nomeArquivoIndice, int* numeroCamposBuscados
) {
  scanf("%s", nomeArquivoEntrada);
  scanf("%s", campoIndexado);
  scanf("%s", tipoDado);
  scanf("%s", nomeArquivoIndice);
  scanf("%d", numeroCamposBuscados);
}

void lerEntradasModo7(char* nomeArquivoEntrada, char* campoIndexado, 
  char* tipoDado, char* nomeArquivoIndice, int* numeroCamposBuscados
) {
  scanf("%s", nomeArquivoEntrada);
  scanf("%s", campoIndexado);
  scanf("%s", tipoDado);
  scanf("%s", nomeArquivoIndice);
  scanf("%d", numeroCamposBuscados);
}

bool inserirRegistro(char* nomeArquivoEntrada, char* campoIndexado, char* tipoDado, char* nomeArquivoIndice, int n) {
  TABELA* tabela = tabelaCriar(nomeArquivoEntrada, "rb+");
  if (!tabelaExiste(tabela)) {
    erroGenerico();
    return false;
  }

  CABECALHO* cabecalho = tabelaLerArmazenarCabecalho(tabela);
    if (!cabecalhoExiste(cabecalho)) return false;
  int nRegistros = cabecalhoObterNroRegArq(cabecalho);
  int64_t proxByteOffset = cabecalhoObterProxByteOffset(cabecalho);

  if (!tabelaArquivoIndisponivel(tabela, cabecalho)) return false;

  tabelaResetLeituraArquivoBinario(tabela, proxByteOffset);
  for(int i = 0; i < n; i++) {
    int32_t idCrimeAux;
    char dataCrimeAux[TAMANHO_DATA_CRIME+3];
    char numeroArtigoStr[10];
    int32_t numeroArtigoAux;
    char lugarCrimeAux[256]; 
    char descricaoCrimeAux[256];
    char marcaCelularAux[TAMANHO_MARCA_CELULAR+3];
    
    scanf("%d", &idCrimeAux);
    scan_quote_string(dataCrimeAux);
    scanf(" %s", numeroArtigoStr);
    scan_quote_string(lugarCrimeAux);
    scan_quote_string(descricaoCrimeAux);
    scan_quote_string(marcaCelularAux);
    
    if(strcmp(numeroArtigoStr, "NULO") == 0) {
      numeroArtigoAux = -1;
    } else {
      numeroArtigoAux = atoi(numeroArtigoStr);
    }
    if(strcmp(lugarCrimeAux, "NULO") == 0) {
      strcpy(lugarCrimeAux, "");
    }
    if(strcmp(descricaoCrimeAux, "NULO") == 0) {
      strcpy(descricaoCrimeAux, "");
    }
    
    DADOS* dados = dadosCriar(idCrimeAux, "$$$$$$$$$$", numeroArtigoAux, "$$$$$$$$$$$$", lugarCrimeAux, descricaoCrimeAux, '0');
    
    if(strcmp(marcaCelularAux, "NULO") != 0) {
      dadosAtualizarMarcaCelular(dados, marcaCelularAux);
    }
    if(strcmp(dataCrimeAux, "NULO") != 0) {
      dadosAtualizarDataCrime(dados, dataCrimeAux);
    }
    
    METADADOS* metadados = dadosCriarMetadados(strlen(descricaoCrimeAux), strlen(lugarCrimeAux));
    
    tabelaAtualizarDados(tabela, dados, metadados, '|');
    nRegistros++;
    proxByteOffset += dadosMetadadosObterTamanhoRegistro(dados, metadados);
    

    dadosDeletar(&dados);
    dadosMetadadosDeletar(&metadados);
  }

  cabecalhoAtualizarProxByteOffset(cabecalho, proxByteOffset);
  cabecalhoAtualizarNroRegArq(cabecalho, nRegistros);
  cabecalhoAtualizarStatus(cabecalho, '1');
  
  tabelaAtualizarCabecalho(tabela, cabecalho);

  tabelaFecharArquivo(tabela);
  tabelaDeletar(&tabela, true);
  cabecalhoDeletar(&cabecalho);

}

int main(void) {
  int modo, numeroCamposBuscados;
  char nomeArquivoEntrada[100], nomeArquivoSaida[100], nomeArquivoIndice[100];
  char campoIndexado[50], tipoDado[20];

  scanf("%d", &modo);

  TABELA* tabela = NULL;
  INDICE* indice = NULL;

  switch (modo)
  {
  case 1:;
    lerEntradasModo1(nomeArquivoEntrada, nomeArquivoSaida);
    tabela = tabelaCriarBinario(nomeArquivoEntrada, nomeArquivoSaida);
    if (!tabelaExiste(tabela)) return 0;

    tabelaFecharArquivo(tabela);
    binarioNaTela(tabelaObterNomeArquivo(tabela)); 
    tabelaDeletar(&tabela, true);
    break;

  case 2:;
    lerEntradasModo2(nomeArquivoEntrada);
    tabela = tabelaLerImprimirBinario(nomeArquivoEntrada);
    if (!tabelaExiste(tabela)) return 0;
    
    tabelaFecharArquivo(tabela);
    tabelaDeletar(&tabela, true);
    break;

  case 3:;
    lerEntradasModo3(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice);
    indice = indiceCriarBinario(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice);
    if (!indiceExiste(indice)) return 0;

    indiceFecharArquivo(indice);
    binarioNaTela(indiceObterNomeArquivo(indice)); 
    indiceDeletar(&indice, true);
    break;
  
  case 4:;
    lerEntradasModo4(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice, &numeroCamposBuscados);
    tabela = tabelaBusca(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice, numeroCamposBuscados, 4, "rb");
    if (!tabelaExiste(tabela)) return 0;

    tabelaFecharArquivo(tabela);
    tabelaDeletar(&tabela, true);
    break;

  case 5:;
    lerEntradasModo4(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice, &numeroCamposBuscados);
    tabela = tabelaBusca(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice, numeroCamposBuscados, 5, "rb+");
    if (!tabelaExiste(tabela)) return 0;

    tabelaFecharArquivo(tabela);
    binarioNaTela(tabelaObterNomeArquivo(tabela));
    tabelaDeletar(&tabela, true);

    INDICE* indiceNovo = indiceCriarBinario(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice);
    if (!indiceExiste(indiceNovo)) return 0;

    indiceFecharArquivo(indiceNovo);
    binarioNaTela(indiceObterNomeArquivo(indiceNovo)); 
    indiceDeletar(&indiceNovo, true);
    break;
  
  case 6:;
      lerEntradasModo4(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice, &numeroCamposBuscados);
      inserirRegistro(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice, numeroCamposBuscados);
      binarioNaTela(nomeArquivoEntrada);

      INDICE* indiceAtualizado = indiceCriarBinario(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice);
      if (!indiceExiste(indiceAtualizado)) return 0;
      indiceFecharArquivo(indiceAtualizado);
      binarioNaTela(indiceObterNomeArquivo(indiceAtualizado)); 
      indiceDeletar(&indiceAtualizado, true);
      
      break;

  case 7:;
    lerEntradasModo7(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice, &numeroCamposBuscados);
    tabela = tabelaLerAtualizar(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice, numeroCamposBuscados);
    if (!tabelaExiste(tabela)) return 0;

    indice = indiceCriarBinario(nomeArquivoEntrada, campoIndexado, tipoDado, nomeArquivoIndice);
    if (!indiceExiste(indice)) return 0;

    tabelaFecharArquivo(tabela);
    binarioNaTela(tabelaObterNomeArquivo(tabela)); 

    indiceFecharArquivo(indice);
    binarioNaTela(indiceObterNomeArquivo(indice)); 
    
    indiceDeletar(&indice, true);
    tabelaDeletar(&tabela, true);
    break;

  default: ;
    erroModo();
    break;
  }

  return 0;
}