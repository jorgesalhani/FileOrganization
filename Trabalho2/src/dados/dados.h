#ifndef DADOS_H
  #define DADOS_H

  #define TAMANHO_DATA_CRIME 10
  #define TAMANHO_MARCA_CELULAR 12
  #define NUMERO_CAMPOS_VARIAVEIS 2
  #define STR_VAZIA "NULO"
  #define TAM_MAX_STR 250
  #define MODO_LEITURA_ARQUIVO "rb+"
  #define MODO_ESCRITA_ARQUIVO "wb+"

  #include <stdlib.h>
  #include <stdbool.h>
  #include <stdio.h>
  #include <stdint.h>
  #include "../utilitarios/entrada.h"
  #include "../utilitarios/erros.h"
  #include "../utilitarios/funcoesFornecidas.h"

  typedef struct registro_ REGISTRO;
  typedef struct cabecalho_ CABECALHO;
  typedef struct arvore_binaria_ ARVORE_BINARIA;
  typedef struct args_ ARGS;
  typedef struct no_ NO;

  typedef struct registro_indice_ REGISTRO_INDICE;

  typedef struct chave_ CHAVE;
  typedef struct cabecalho_indice_ CABECALHO_INDICE;

  enum ordemBuscaBinaria_ {
    emOrdem, preOrdem, posOrdem
  };

  struct no_ {
    REGISTRO* registro;
    NO* esquerda;
    NO* direita;
  };

  struct arvore_binaria_ {
    int32_t totalRegistros;
    enum ordemBuscaBinaria_ ordem;
    NO* raiz;
  };

  struct args_ {
    ENTRADA* entrada;
    REGISTRO* registro;
    CABECALHO* cabecalho;
    ARVORE_BINARIA* arvoreBinaria;
    FILE* arquivoDadosBin;
    FILE* arquivoIndiceBin;
    bool regEncontradoEmBusca;
    int idProxRamo;
    NO* proxRamoArvore;
    int64_t bOffPrimOcorArqIndice;
    CABECALHO_INDICE* cabecalhoIndice;
    REGISTRO_INDICE* registroIndice;
    int64_t byteOffPaiRegSplitado;
    int32_t idCrimeBuscado;
  };

  bool dadosCriarArquivoBinario(ENTRADA* entrada);
  bool dadosImprimirArquivoBinario(ENTRADA* entrada);
  ARGS* dadosRegistrosEmArvoreBinaria(ENTRADA* entrada);
  bool resetLeituraDeArquivo(FILE* arquivoBin, int64_t byteOffset);
  void dadosVarreduraCompletaArvoreBinaria(ARGS* args, NO* raiz, void (*ftnPorRegistro)()); 
  bool dadosLerRegistroDoArquivoBinario(FILE* arquivoBinarioDados, REGISTRO* registro);
  void* dadosObterCampoIndexado(REGISTRO* registro, char* campoIndexado);
  int64_t dadosObterByteoffset(REGISTRO* registro);
  bool argsApagar(ARGS** args);
  bool dadosRegistroApagar(REGISTRO** registro);
  void preencherString(char* dest, char* orig, int tamanho, bool comDelimitador);
  bool dadosBuscarPorCampos(ENTRADA* entrada, void (*ftnPorBusca));
  int dadosCompararRegistroComChaveBusca(ARGS* args);
  void copiarRegistro(REGISTRO* regOrig, REGISTRO* regDest);
  ARGS* argsInit();
  bool dadosRemoverRegistroLogicamente(ARGS* args);
  int dadosObterIdCrime(REGISTRO* registro);
  void dadosEncontrar(ARGS* args, void (*ftnPorRegistro)());
  void dadosImprimirRegistro(ARGS* args);
  void dadosAtualizarByteoffset(REGISTRO* registro, int64_t byteOffset);
  int64_t dadosObterRemovido(REGISTRO* registro);
  bool dadosInserirNovosRegistros(ENTRADA* entrada);
  bool dadosAtualizarRegistros(ARGS* args);
  ARGS* dadosVarreduraSequencialArquivoBinario(ENTRADA* entrada, void (*ftnPorRegistro)());

  CABECALHO* dadosCabecalhoInit();
  REGISTRO* dadosRegistroInit();

#endif