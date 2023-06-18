#include "indice.h"


struct chave_ {
  int32_t C;
  int64_t Pr;
};

struct registro_indice_ {
  char* tipoDado;
  void* chaveBusca;
  int64_t byteOffset;
  int32_t tamanhoRegistro;
    
  int contadorDeOcupacao;
  int32_t* P;
  CHAVE** chaves;
  int32_t nivel;
  int32_t n;
};

struct cabecalho_indice_ {
  char status;
  int32_t qtdReg;
  int64_t tamanhoRegistro;

  int32_t noRaiz;
  int32_t RNNproxNo;
  int32_t nroNiveis;
  int32_t nroChaves;
  int64_t tamanhoRegistroArvoreB;
};

void indiceArvoreBVarreduraDeBusca(ARGS* args);

bool indiceCabecalhoApagar(CABECALHO_INDICE** cabecalho_indice) {
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

  cabecalhoIndice->noRaiz = -1;
  cabecalhoIndice->RNNproxNo = 0;
  cabecalhoIndice->nroNiveis = 0;
  cabecalhoIndice->nroChaves = 0;
  cabecalhoIndice->tamanhoRegistroArvoreB = TAM_MAX_CABECALHO_INDICEB;

  return cabecalhoIndice;
}

CHAVE* arvoreBchaveInit() {
  CHAVE* chave = (CHAVE*) malloc(sizeof(CHAVE));
  chave->C = -1;
  chave->Pr = -1;
  return chave;
}

REGISTRO_INDICE* indiceRegistroInit(char* tipoDado) {
  REGISTRO_INDICE* registroIndice = (REGISTRO_INDICE*) malloc(sizeof(REGISTRO_INDICE));
  if (registroIndice == NULL) return NULL;

  registroIndice->byteOffset = 0;
  registroIndice->tipoDado = tipoDado;

  if (strcmp(tipoDado, "string") == 0) {
    registroIndice->chaveBusca = (char*) calloc(TAM_MAX_STR, sizeof(char));
    registroIndice->tamanhoRegistro = 
      sizeof(registroIndice->byteOffset) + 
      sizeof(char) * TAMANHO_CHAVE_BUSCA;
    return registroIndice;
  }

  int32_t* chaveBusca;
  registroIndice->chaveBusca = chaveBusca;
  registroIndice->tamanhoRegistro = 
    sizeof(registroIndice->byteOffset) + sizeof(int32_t);

  registroIndice->contadorDeOcupacao = 0;
  registroIndice->n = 0;
  registroIndice->nivel = 1;
  registroIndice->P = (int32_t*) malloc(sizeof(int32_t)*(ORDEM_ARVOREB-1));

  int numChaves = ORDEM_ARVOREB-2;
  registroIndice->chaves = (CHAVE**) malloc(sizeof(CHAVE*)*numChaves);
  for (int i = 0; i < numChaves; i++) {
    (registroIndice->chaves)[i] = (CHAVE*) malloc(sizeof(CHAVE));
  }
  return registroIndice;
}

bool indiceArvoreBLerCabecalhoDoArquivoBinario(FILE* arquivoBinarioIndice, CABECALHO_INDICE* cabecalho) {
  if (cabecalho == NULL || arquivoBinarioIndice == NULL) return false;

  resetLeituraDeArquivo(arquivoBinarioIndice, 0);
  fread(&cabecalho->status, sizeof(char), 1, arquivoBinarioIndice);

  // char statusEmProgresso = '0';
  // resetLeituraDeArquivo(arquivoBinarioIndice, 0);
  // fwrite(&statusEmProgresso, sizeof(char), 1, arquivoBinarioIndice);
  fread(&cabecalho->noRaiz, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&cabecalho->RNNproxNo, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&cabecalho->nroNiveis, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&cabecalho->nroChaves, sizeof(int32_t), 1, arquivoBinarioIndice);
  return true;
}

bool indiceArvoreBLerPaginaDoArquivoBinario(FILE* arquivoBinarioIndice, REGISTRO_INDICE* registro) {
  if (arquivoBinarioIndice == NULL || registro == NULL) return false;

  fread(&registro->nivel, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&registro->n, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&(registro->P)[0], sizeof(int64_t), 1, arquivoBinarioIndice);
  for (int i = 0; i < ORDEM_ARVOREB-1; i++) {
    fread(&(registro->chaves)[i]->C, sizeof(int32_t), 1, arquivoBinarioIndice);
    fread(&(registro->chaves)[i]->Pr, sizeof(int64_t), 1, arquivoBinarioIndice);
    fread(&(registro->P)[i+1], sizeof(int64_t), 1, arquivoBinarioIndice);
  }
  return true;
}


bool indiceRegistroApagar(REGISTRO_INDICE** registroIndice, char* tipoDado) {
  if (registroIndice == NULL || *registroIndice == NULL) return false;
  if (strcmp(tipoDado, "string") == 0) {
    free((*registroIndice)->chaveBusca);
    (*registroIndice)->chaveBusca = NULL;
  }

  if ((*registroIndice)->chaves != NULL) {
    int numChaves = ORDEM_ARVOREB-2;
    for (int i = 0; i < numChaves; i++) {
      free(&((*registroIndice)->chaves)[i]);
      (*registroIndice)->chaves[i] = NULL;
    }
  }
  
  free((*registroIndice));
  *registroIndice = NULL;
  registroIndice = NULL;
  return true;
}

void indiceLinearArmazenarCabecalho(FILE* arquivoBinarioIndice, CABECALHO_INDICE* cabecalhoIndice) {
  if (arquivoBinarioIndice == NULL || cabecalhoIndice == NULL) return;
  resetLeituraDeArquivo(arquivoBinarioIndice, 0);
  fwrite(&cabecalhoIndice->status, sizeof(char), 1, arquivoBinarioIndice);
  fwrite(&cabecalhoIndice->qtdReg, sizeof(int32_t), 1, arquivoBinarioIndice);
  return;
}

bool indiceArmazenarCabecalho(
  FILE* arquivoBinarioIndice,
  CABECALHO_INDICE* cabecalhoIndice,
  void (ftnArmazenarCabecalho(FILE*, CABECALHO_INDICE*))
) {
  ftnArmazenarCabecalho(arquivoBinarioIndice, cabecalhoIndice);
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
    preencherString(registroIndice->chaveBusca, chaveBusca, TAMANHO_CHAVE_BUSCA, false);
    fwrite(registroIndice->chaveBusca, sizeof(char), TAMANHO_CHAVE_BUSCA, args->arquivoIndiceBin);
  } else {
    int32_t* chaveBusca = (int32_t*) valorCampoDeBusca;
    registroIndice->chaveBusca = chaveBusca;
    fwrite(chaveBusca, sizeof(int32_t), 1, args->arquivoIndiceBin);
  }

  fwrite(&registroIndice->byteOffset, sizeof(int64_t), 1, args->arquivoIndiceBin);
  args->registro = raiz->registro;
  indiceRegistroApagar(&registroIndice, tipoDado);
  return;
}

void indiceArvoreBArmazenarCabecalho(FILE* arquivoBinarioIndice, CABECALHO_INDICE* cabecalho) {
  if (cabecalho == NULL || arquivoBinarioIndice == NULL) return;
  resetLeituraDeArquivo(arquivoBinarioIndice, 0);
  fwrite(&cabecalho->status, sizeof(char), 1, arquivoBinarioIndice);
  fwrite(&cabecalho->noRaiz, sizeof(int32_t), 1, arquivoBinarioIndice);
  fwrite(&cabecalho->RNNproxNo, sizeof(int32_t), 1, arquivoBinarioIndice);
  fwrite(&cabecalho->nroNiveis, sizeof(int32_t), 1, arquivoBinarioIndice);
  fwrite(&cabecalho->nroChaves, sizeof(int32_t), 1, arquivoBinarioIndice);

  int32_t tamCab = sizeof(cabecalho->status) + sizeof(cabecalho->noRaiz) +
    sizeof(cabecalho->RNNproxNo) + sizeof(cabecalho->nroNiveis) + sizeof(cabecalho->nroChaves);
  
  int32_t tamLixo = TAM_MAX_CABECALHO_INDICEB - tamCab;
  char lixo = '$';
  for (int i = 0; i < tamLixo; i++) {
    fwrite(&lixo, sizeof(char), 1, arquivoBinarioIndice);
  }
  fflush(arquivoBinarioIndice);
  return;
}

void indiceArvoreBArmazenarRegistro(ARGS* args) {
  if (args == NULL) return;
  FILE* arquivoBinarioIndice = args->arquivoIndiceBin;
  REGISTRO_INDICE* registro = args->registroIndice;
  
  fwrite(&registro->nivel, sizeof(int32_t), 1, arquivoBinarioIndice);
  fwrite(&registro->n, sizeof(int32_t), 1, arquivoBinarioIndice);
  fwrite(&(registro->P)[0], sizeof(int64_t), 1, arquivoBinarioIndice);
  for (int i = 0; i < ORDEM_ARVOREB-1; i++) {
    fwrite(&(registro->chaves)[i]->C, sizeof(int32_t), 1, arquivoBinarioIndice);
    fwrite(&(registro->chaves)[i]->Pr, sizeof(int64_t), 1, arquivoBinarioIndice);
    fwrite(&(registro->P)[i+1], sizeof(int64_t), 1, arquivoBinarioIndice);
  }
  return;
}

bool indiceCriarArquivoBinario(ENTRADA* entrada) {
  if (entrada == NULL) return false;
  ARGS* args = NULL;
  
  bool indiceEmArvoreB = entradaObterIndiceEmArvoreB(entrada);
  if (!indiceEmArvoreB) {
    args = dadosRegistrosEmArvoreBinaria(entrada);
  } else {
    args = argsInit();
  }
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

  cabecalhoIndice->status = '1';
  
  if (!indiceEmArvoreB) {
    cabecalhoIndice->qtdReg = args->arvoreBinaria->totalRegistros;
    indiceArmazenarCabecalho(arquivoBinarioIndice, cabecalhoIndice, indiceLinearArmazenarCabecalho);
    args->arvoreBinaria->ordem = emOrdem;
    dadosVarreduraCompletaArvoreBinaria(args, args->arvoreBinaria->raiz, indiceArmazenarRegistro);
    indiceArmazenarCabecalho(arquivoBinarioIndice, cabecalhoIndice, indiceLinearArmazenarCabecalho);
  } else {
    indiceArmazenarCabecalho(arquivoBinarioIndice, cabecalhoIndice, indiceArvoreBArmazenarCabecalho);
    ARGS* argsSeq = dadosVarreduraSequencialArquivoBinario(entrada, indiceArvoreBVarreduraDeBusca);
    indiceArmazenarCabecalho(arquivoBinarioIndice, cabecalhoIndice, indiceArvoreBArmazenarCabecalho);
    argsApagar(&argsSeq);
  }

  fclose(arquivoBinarioIndice);

  indiceCabecalhoApagar(&cabecalhoIndice);
  argsApagar(&args);
  
  binarioNaTela(arquivoIndice);

  return true;
}

void indiceLinearLerCabecalho(FILE* arquivoBinarioIndice, CABECALHO_INDICE* cabecalhoIndice) {
  if (arquivoBinarioIndice == NULL || cabecalhoIndice == NULL) return;
  resetLeituraDeArquivo(arquivoBinarioIndice, 0);
  fread(&cabecalhoIndice->status, sizeof(char), 1, arquivoBinarioIndice);

  // char statusEmProgresso = '0';
  // resetLeituraDeArquivo(arquivoBinarioIndice, 0);
  // fwrite(&statusEmProgresso, sizeof(char), 1, arquivoBinarioIndice);
  fread(&cabecalhoIndice->qtdReg, sizeof(int32_t), 1, arquivoBinarioIndice);
}

bool indiceLerRegistroDoArquivoBinario(FILE* arquivoBinarioIndice, REGISTRO_INDICE* registroIndice) {
  if (arquivoBinarioIndice == NULL || registroIndice == NULL) return false;
  
  if (strcmp(registroIndice->tipoDado, "string") == 0) {
    fread(registroIndice->chaveBusca, sizeof(char), TAMANHO_CHAVE_BUSCA, arquivoBinarioIndice);  
  } else {
    fread(&registroIndice->chaveBusca, sizeof(int32_t), 1, arquivoBinarioIndice);
  }
  fread(&registroIndice->byteOffset, sizeof(int64_t), 1, arquivoBinarioIndice);
  
  return true;
}

bool indiceLerCabecalhoDoArquivoBinario(
  FILE* arquivoBinarioIndice, 
  CABECALHO_INDICE* cabecalhoIndice,
  void (ftnLerCabecalho())
) {
  if (arquivoBinarioIndice == NULL || cabecalhoIndice == NULL) return false;
  ftnLerCabecalho(arquivoBinarioIndice, cabecalhoIndice);
  return true;
}

ARGS* indiceVarreduraSequencialArquivoBinario(ENTRADA* entrada, void (*ftnPorRegistro)()) {

  char* arquivoIndice = entradaObterArquivoIndice(entrada);
  FILE* arquivoBinarioIndice = fopen(arquivoIndice, MODO_LEITURA_ARQUIVO);
  if (arquivoBinarioIndice == NULL) {
    erroGenerico();
    return false;
  };

  char* arquivoEntrada = entradaObterArquivoEntrada(entrada);
  FILE* arquivoBinarioDados = fopen(arquivoEntrada, MODO_LEITURA_ARQUIVO);
  if (arquivoBinarioDados == NULL) {
    erroGenerico();
    return false;
  };

  CABECALHO_INDICE* cabecalhoIndice = indiceCabecalhoInit();
  indiceLerCabecalhoDoArquivoBinario(arquivoBinarioIndice, cabecalhoIndice, indiceLinearLerCabecalho);

  if (cabecalhoIndice->status == '0') {
    erroGenerico();
    fclose(arquivoBinarioIndice);
    indiceCabecalhoApagar(&cabecalhoIndice);
    return NULL;
  }

  if (cabecalhoIndice->qtdReg == 0) {
    erroSemRegistros();
    fclose(arquivoBinarioIndice);
    indiceCabecalhoApagar(&cabecalhoIndice);
    return NULL;
  }

  ARGS* args = argsInit();
  args->entrada = entrada;
  args->arquivoDadosBin = arquivoBinarioDados;
  args->arquivoIndiceBin = arquivoBinarioIndice;

  int32_t qtdReg = cabecalhoIndice->qtdReg;

  resetLeituraDeArquivo(arquivoBinarioIndice, cabecalhoIndice->tamanhoRegistro);

  char* tipoDado = entradaObterTipoDado(entrada);
  while (qtdReg--) {
    REGISTRO_INDICE* registroIndice = indiceRegistroInit(tipoDado);
    if (registroIndice == NULL) continue;
    
    indiceLerRegistroDoArquivoBinario(arquivoBinarioIndice, registroIndice);

    resetLeituraDeArquivo(arquivoBinarioDados, registroIndice->byteOffset);
    
    REGISTRO* registro = dadosRegistroInit();
    if (registro == NULL) continue;
    dadosLerRegistroDoArquivoBinario(arquivoBinarioDados, registro);

    args->registro = registro;

    ftnPorRegistro(args);

    indiceRegistroApagar(&registroIndice, tipoDado);
    dadosRegistroApagar(&registro);
  }

  indiceCabecalhoApagar(&cabecalhoIndice);
  fclose(arquivoBinarioIndice);

  return args;
}

void indiceMapDadosAPartirDaPrimeiraOcorrencia(ARGS* args, void (*ftnPorRegistro)()) {
  if (args == NULL) return;
  int regComp = dadosCompararRegistroComChaveBusca(args);
  int64_t bOffAnterior = 1;
  if (regComp == 0) {
    REGISTRO_INDICE* registroIndice = indiceRegistroInit(entradaObterTipoDado(args->entrada));
    REGISTRO* registro = dadosRegistroInit();
    
    bOffAnterior = args->bOffPrimOcorArqIndice - (int64_t) registroIndice->tamanhoRegistro;
    if (bOffAnterior > 0) {
      resetLeituraDeArquivo(args->arquivoIndiceBin, bOffAnterior);
      
      indiceLerRegistroDoArquivoBinario(args->arquivoIndiceBin, registroIndice);

      if (registro == NULL) return;

      resetLeituraDeArquivo(args->arquivoDadosBin, registroIndice->byteOffset);
      dadosLerRegistroDoArquivoBinario(args->arquivoDadosBin, registro);

      copiarRegistro(registro, args->registro);
      args->bOffPrimOcorArqIndice = bOffAnterior;

      dadosRegistroApagar(&registro);
      indiceRegistroApagar(&registroIndice, registroIndice->tipoDado);

      return indiceMapDadosAPartirDaPrimeiraOcorrencia(args, ftnPorRegistro);
    }
    dadosRegistroApagar(&registro);
    indiceRegistroApagar(&registroIndice, registroIndice->tipoDado);
  }

  REGISTRO_INDICE* registroIndice = indiceRegistroInit(entradaObterTipoDado(args->entrada));
  if (bOffAnterior > 0) {
    int64_t bOffProx = args->bOffPrimOcorArqIndice + (int64_t) registroIndice->tamanhoRegistro;
    indiceLerRegistroDoArquivoBinario(args->arquivoIndiceBin, registroIndice);
    resetLeituraDeArquivo(args->arquivoDadosBin, registroIndice->byteOffset);
    dadosLerRegistroDoArquivoBinario(args->arquivoDadosBin, args->registro);
    regComp = dadosCompararRegistroComChaveBusca(args);
  }
  while (regComp == 0) {
    dadosAtualizarByteoffset(args->registro, registroIndice->byteOffset);
    dadosEncontrar(args, ftnPorRegistro);
    indiceLerRegistroDoArquivoBinario(args->arquivoIndiceBin, registroIndice);
    resetLeituraDeArquivo(args->arquivoDadosBin, registroIndice->byteOffset);
    dadosLerRegistroDoArquivoBinario(args->arquivoDadosBin, args->registro);
    regComp = dadosCompararRegistroComChaveBusca(args);
  }

  indiceRegistroApagar(&registroIndice, registroIndice->tipoDado);
}

bool indiceBuscaBinariaArquivoBinario(ENTRADA* entrada, ARGS* args, void (*ftnPorRegistro)()) {

  CABECALHO_INDICE* cabecalhoIndice = indiceCabecalhoInit();
  indiceLerCabecalhoDoArquivoBinario(args->arquivoIndiceBin, cabecalhoIndice, indiceLinearLerCabecalho);

  if (cabecalhoIndice->status == '0') {
    indiceCabecalhoApagar(&cabecalhoIndice);
    erroGenerico();
    return false;
  }

  if (cabecalhoIndice->qtdReg == 0) {
    indiceCabecalhoApagar(&cabecalhoIndice);
    erroSemRegistros();
    return false;
  }

  int32_t qtdReg = cabecalhoIndice->qtdReg;

  resetLeituraDeArquivo(args->arquivoIndiceBin, cabecalhoIndice->tamanhoRegistro);

  char* tipoDado = entradaObterTipoDado(entrada);

  REGISTRO_INDICE* registroBase = indiceRegistroInit(tipoDado);
  int32_t tamRegIndice = registroBase->tamanhoRegistro;
  indiceRegistroApagar(&registroBase, tipoDado);

  int32_t pivotSuperior = 
    tamRegIndice * (int32_t) (qtdReg - 1) + 
    cabecalhoIndice->tamanhoRegistro;

  int32_t pivotCentral = (int32_t) (pivotSuperior / 2);
  int32_t sobra = pivotCentral%tamRegIndice;
  pivotCentral -= (tamRegIndice - sobra);
  pivotCentral += 1;

  int32_t pivotInferior = cabecalhoIndice->tamanhoRegistro;

  while ( pivotSuperior >= pivotInferior && 
          pivotCentral >= (pivotInferior) && 
          pivotCentral <= pivotSuperior) {
    resetLeituraDeArquivo(args->arquivoIndiceBin, pivotCentral);

    REGISTRO_INDICE* registroIndice = indiceRegistroInit(tipoDado);
    if (registroIndice == NULL) continue;
    
    indiceLerRegistroDoArquivoBinario(args->arquivoIndiceBin, registroIndice);
    
    REGISTRO* registro = dadosRegistroInit();
    if (registro == NULL) continue;
    resetLeituraDeArquivo(args->arquivoDadosBin, registroIndice->byteOffset);
    dadosLerRegistroDoArquivoBinario(args->arquivoDadosBin, registro);

    args->registro = registro;

    int regComp = dadosCompararRegistroComChaveBusca(args);
    // if (registroIndice->byteOffset >= 42592) {
    //   printf("AA\n");
    // }

    if (regComp < 0) {
      pivotSuperior = pivotCentral - 1;
    } else 
    if (regComp > 0) {
      pivotInferior = pivotCentral + 1;
    } else {
      args->bOffPrimOcorArqIndice = pivotCentral;
      indiceMapDadosAPartirDaPrimeiraOcorrencia(args, ftnPorRegistro);
      dadosRegistroApagar(&registro);
      indiceRegistroApagar(&registroIndice, tipoDado);
      break;
    }
    pivotCentral = (int32_t) (pivotInferior + (pivotSuperior - pivotInferior) / 2);
    sobra = pivotCentral%tamRegIndice - cabecalhoIndice->tamanhoRegistro;
    if (sobra < 0) {
      pivotCentral -= sobra;
      pivotCentral -= registroIndice->tamanhoRegistro;
    } else {
      pivotCentral -= sobra;
    }

    dadosRegistroApagar(&registro);
    indiceRegistroApagar(&registroIndice, tipoDado);
  }

  indiceCabecalhoApagar(&cabecalhoIndice);
  return true;
}

void indiceArvoreBVarreduraDeBusca(ARGS* args) {
  if (args == NULL) return;

  

}