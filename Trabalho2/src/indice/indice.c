#include "indice.h"

struct registro_indice_ {
  void* chaveBusca;
  int64_t byteOffset;
};

struct cabecalho_indice_ {
  char status;
  int32_t qtdReg;
  int32_t tamanhoRegistro;
};

bool indiceCabacelhoApagar(CABECALHO_INDICE** cabecalho_indice) {
  if (cabecalho_indice == NULL || *cabecalho_indice == NULL) return false;
  free(*cabecalho_indice);
  *cabecalho_indice = NULL;
  cabecalho_indice = NULL;
  return true;
}

CABECALHO_INDICE* indiceCabecalhoInit() {
  CABECALHO_INDICE* cabecalhoIndice = (CABECALHO_INDICE*) malloc(sizeof(CABECALHO_INDICE));
  if (cabecalhoIndice == NULL) return NULL;

  cabecalhoIndice->status = '0';
  cabecalhoIndice->qtdReg = 0;
  cabecalhoIndice->tamanhoRegistro = 
    sizeof(cabecalhoIndice->status) + 
    sizeof(cabecalhoIndice->qtdReg);

  return cabecalhoIndice;
}

REGISTRO_INDICE* indiceRegistroInit(char* tipoDado) {
  REGISTRO_INDICE* registroIndice = (REGISTRO_INDICE*) malloc(sizeof(REGISTRO_INDICE));
  if (registroIndice == NULL) return NULL;

  registroIndice->byteOffset = 0;
  if (strcmp(tipoDado, "string") == 0) {
    registroIndice->chaveBusca = (char*) malloc(sizeof(char)*TAMANHO_CHAVE_BUSCA+1);
    return registroIndice;
  }

  registroIndice->chaveBusca = (int32_t) 0;
  return registroIndice;
}

bool indiceRegistroApagar(REGISTRO_INDICE** registroIndice, char* tipoDado) {
  if (registroIndice == NULL || *registroIndice == NULL) return false;
  if (strcmp(tipoDado, "string") == 0) {
    free((*registroIndice)->chaveBusca);
    (*registroIndice)->chaveBusca = NULL;
  }
  free((*registroIndice));
  *registroIndice = NULL;
  registroIndice = NULL;
  return true;
}

bool indiceArmazenarCabecalho(CABECALHO_INDICE* cabecalhoIndice, FILE* arquivoBinarioIndice) {
  if (arquivoBinarioIndice == NULL || cabecalhoIndice == NULL) return false;
  resetLeituraDeArquivo(arquivoBinarioIndice, 0);
  fwrite(&cabecalhoIndice->status, sizeof(char), 1, arquivoBinarioIndice);
  fwrite(&cabecalhoIndice->qtdReg, sizeof(int32_t), 1, arquivoBinarioIndice);
  return true;
}

void indiceArmazenarRegistro(ARGS* args, NO* raiz) {
  if (args == NULL || raiz == NULL) return;

  char* tipoDado = entradaObterTipoDado(args->entrada);
  void* valorCampoDeBusca = dadosObterCampoIndexado(
    raiz->registro, entradaObterCampoIndexado(args->entrada)
  );
  REGISTRO_INDICE* registroIndice = indiceRegistroInit(tipoDado);
  registroIndice->byteOffset = dadosObterByteoffset(raiz->registro);
  if (strcmp(tipoDado, "string") == 0) {
    char* chaveBusca = (char*) valorCampoDeBusca;
    registroIndice->chaveBusca = chaveBusca;
    fwrite(chaveBusca, sizeof(char), TAMANHO_CHAVE_BUSCA, args->arquivoIndiceBin);
  } else {
    int32_t* chaveBusca = (int32_t*) valorCampoDeBusca;
    registroIndice->chaveBusca = chaveBusca;
    fwrite(chaveBusca, sizeof(int32_t), 1, args->arquivoIndiceBin);
  }

  fwrite(&registroIndice->byteOffset, sizeof(int64_t), 1, args->arquivoIndiceBin);

  indiceRegistroApagar(&registroIndice, tipoDado);
  return;
}

bool indiceCriarArquivoBinario(ENTRADA* entrada) {
  if (entrada == NULL) return false;

  ARGS* args = dadosRegistrosEmArvoreBinaria(entrada);
  if (args == NULL) return false;

  CABECALHO_INDICE* cabecalhoIndice = indiceCabecalhoInit();
  if (cabecalhoIndice == NULL) {
    argsApagar(&args);
    return false;
  }

  char* arquivoIndice = entradaObterArquivoIndice(entrada);
  FILE* arquivoBinarioIndice = fopen(arquivoIndice, MODO_ESCRITA_ARQUIVO);
  if (arquivoBinarioIndice == NULL) {
    erroGenerico();
    return false;
  };
  args->arquivoIndiceBin = arquivoBinarioIndice;

  cabecalhoIndice->qtdReg = args->arvoreBinaria->totalRegistros;
  indiceArmazenarCabecalho(cabecalhoIndice, arquivoBinarioIndice);

  dadosVarreduraCompletaArvoreBinaria(args, args->arvoreBinaria->raiz, indiceArmazenarRegistro);


  cabecalhoIndice->status = '1';
  indiceArmazenarCabecalho(cabecalhoIndice, arquivoBinarioIndice);

  fclose(arquivoBinarioIndice);

  indiceCabacelhoApagar(&cabecalhoIndice);
  argsApagar(&args);
  
  binarioNaTela(arquivoIndice);

  return true;
}
