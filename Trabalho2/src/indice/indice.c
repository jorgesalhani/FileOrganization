#include "indice.h"


struct chave_ {
  int32_t C;
  int64_t Pr;
};

struct registro_indice_ {
  char* tipoDado;
  void* chaveBusca;
  int64_t byteOffset;
  int64_t tamanhoRegistroLinear;
    
  int32_t P[ORDEM_ARVOREB];
  CHAVE* chaves[ORDEM_ARVOREB-1];
  int32_t nivel;
  int32_t n;
  int64_t tamanhoRegistroArvoreB;
};

struct cabecalho_indice_ {
  char status;
  int32_t qtdReg;
  int64_t tamanhoRegistroLinear;

  int32_t noRaiz;
  int64_t proxByteOffsetNoRaiz;
  int32_t RNNproxNo;
  int64_t proxByteOffsetProxNo;
  int32_t nroNiveis;
  int32_t nroChaves;
  int64_t tamanhoRegistroArvoreB;
};

struct chave_arvore_B_estrela_ {
  int32_t C;
  int64_t Pr;
  int32_t Pp; //Ponteirp pagina atrelado
};

struct cabecalho_arvore_B_estrela_ {
  char status;
  int noRaiz;
  int proxRRN;
  int nroNiveis;
  int nroChaves;
};

struct pagina_arvore_B_estrela_ {
  int nivel;
  int contadorDeOcupacao;
  int P[5];
  CHAVE chaves[4];
};


void indiceArvoreBInserirRegistro(ARGS* args);

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
  cabecalhoIndice->tamanhoRegistroLinear = 
    sizeof(cabecalhoIndice->status) + 
    sizeof(cabecalhoIndice->qtdReg);

  cabecalhoIndice->noRaiz = -1;
  cabecalhoIndice->RNNproxNo = 0;
  cabecalhoIndice->nroNiveis = 0;
  cabecalhoIndice->nroChaves = 0;
  cabecalhoIndice->tamanhoRegistroArvoreB = TAM_MAX_CABECALHO_INDICEB;
  cabecalhoIndice->proxByteOffsetNoRaiz = -1;
  cabecalhoIndice->proxByteOffsetProxNo = -1;

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
    registroIndice->tamanhoRegistroLinear = 
      sizeof(registroIndice->byteOffset) + 
      sizeof(char) * TAMANHO_CHAVE_BUSCA;
    return registroIndice;
  }

  int32_t* chaveBusca;
  registroIndice->chaveBusca = chaveBusca;
  registroIndice->tamanhoRegistroLinear = 
    sizeof(registroIndice->byteOffset) + sizeof(int32_t);

  registroIndice->n = 0;
  registroIndice->nivel = 1;
  registroIndice->P[0] = -1;

  int numChaves = ORDEM_ARVOREB-1;
  for (int i = 0; i < numChaves; i++) {
    (registroIndice->chaves)[i] = (CHAVE*) malloc(sizeof(CHAVE));
    (registroIndice->chaves)[i]->C = -1;
    (registroIndice->chaves)[i]->Pr = -1;
    (registroIndice->P)[i+1] = -1;
  }
  registroIndice->tamanhoRegistroArvoreB = 
    sizeof(registroIndice->nivel) + 
    sizeof(registroIndice->n) + 
    sizeof(registroIndice->chaves[0]->C)*numChaves + 
    sizeof(registroIndice->chaves[0]->Pr)*numChaves +
    sizeof(registroIndice->P[0])*(numChaves+1);

  return registroIndice;
}

void indiceArvoreBLerCabecalhoDoArquivoBinario(FILE* arquivoBinarioIndice, CABECALHO_INDICE* cabecalho) {
  if (cabecalho == NULL || arquivoBinarioIndice == NULL) return;

  resetLeituraDeArquivo(arquivoBinarioIndice, 0);
  fread(&cabecalho->status, sizeof(char), 1, arquivoBinarioIndice);

  // char statusEmProgresso = '0';
  // resetLeituraDeArquivo(arquivoBinarioIndice, 0);
  // fwrite(&statusEmProgresso, sizeof(char), 1, arquivoBinarioIndice);
  fread(&cabecalho->noRaiz, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&cabecalho->RNNproxNo, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&cabecalho->nroNiveis, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&cabecalho->nroChaves, sizeof(int32_t), 1, arquivoBinarioIndice);
  return;
}


bool indiceRegistroApagar(REGISTRO_INDICE** registroIndice, char* tipoDado) {
  if (registroIndice == NULL || *registroIndice == NULL) return false;
  if (strcmp(tipoDado, "string") == 0) {
    free((*registroIndice)->chaveBusca);
    (*registroIndice)->chaveBusca = NULL;
  }

  if ((*registroIndice)->chaves != NULL) {
    int numChaves = ORDEM_ARVOREB-1;
    // for (int i = 0; i < numChaves; i++) {
    //   free(&((*registroIndice)->chaves)[i]);
    //   (*registroIndice)->chaves[i] = NULL;
    // }
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

void indiceLinearArmazenarRegistro(ARGS* args, NO* raiz) {
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
  fwrite(&(registro->P)[0], sizeof(int32_t), 1, arquivoBinarioIndice);
  int numChaves = ORDEM_ARVOREB-1;
  for (int i = 0; i < numChaves; i++) {
    fwrite(&(registro->chaves)[i]->C, sizeof(int32_t), 1, arquivoBinarioIndice);
    fwrite(&(registro->chaves)[i]->Pr, sizeof(int64_t), 1, arquivoBinarioIndice);
    fwrite(&(registro->P)[i+1], sizeof(int32_t), 1, arquivoBinarioIndice);
  }
  fflush(arquivoBinarioIndice);
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
    dadosVarreduraCompletaArvoreBinaria(args, args->arvoreBinaria->raiz, indiceLinearArmazenarRegistro);
    indiceArmazenarCabecalho(arquivoBinarioIndice, cabecalhoIndice, indiceLinearArmazenarCabecalho);
  } else {
    indiceArmazenarCabecalho(arquivoBinarioIndice, cabecalhoIndice, indiceArvoreBArmazenarCabecalho);
    ARGS* argsSeq = dadosVarreduraSequencialArquivoBinario(entrada, indiceArvoreBInserirRegistro);
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

void indiceLinearLerRegistro(FILE* arquivoBinarioIndice, REGISTRO_INDICE* registroIndice) {
  if (arquivoBinarioIndice == NULL || registroIndice == NULL) return; 
  
  if (strcmp(registroIndice->tipoDado, "string") == 0) {
    fread(registroIndice->chaveBusca, sizeof(char), TAMANHO_CHAVE_BUSCA, arquivoBinarioIndice);  
  } else {
    fread(&registroIndice->chaveBusca, sizeof(int32_t), 1, arquivoBinarioIndice);
  }
  fread(&registroIndice->byteOffset, sizeof(int64_t), 1, arquivoBinarioIndice);
}



void indiceArvoreBLerRegistro(FILE* arquivoBinarioIndice, REGISTRO_INDICE* registroIndice) {
  if (arquivoBinarioIndice == NULL || registroIndice == NULL) return; 
  fread(&registroIndice->nivel, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&registroIndice->n, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&(registroIndice->P)[0], sizeof(int32_t), 1, arquivoBinarioIndice);
  int numChaves = ORDEM_ARVOREB-1;
  for (int i = 0; i < numChaves; i++) {
    fread(&(registroIndice->chaves)[i]->C, sizeof(int32_t), 1, arquivoBinarioIndice);
    fread(&(registroIndice->chaves)[i]->Pr, sizeof(int64_t), 1, arquivoBinarioIndice);
    fread(&(registroIndice->P)[i+1], sizeof(int32_t), 1, arquivoBinarioIndice);
  }
}

void indiceLerRegistroDoArquivoBinario(
  FILE* arquivoBinarioIndice, 
  REGISTRO_INDICE* registroIndice,
  void (ftnLerRegistro(FILE*, REGISTRO_INDICE*))
) {
  if (arquivoBinarioIndice == NULL || registroIndice == NULL) return;
  ftnLerRegistro(arquivoBinarioIndice, registroIndice);
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

  resetLeituraDeArquivo(arquivoBinarioIndice, cabecalhoIndice->tamanhoRegistroLinear);

  char* tipoDado = entradaObterTipoDado(entrada);
  while (qtdReg--) {
    REGISTRO_INDICE* registroIndice = indiceRegistroInit(tipoDado);
    if (registroIndice == NULL) continue;
    
    indiceLerRegistroDoArquivoBinario(arquivoBinarioIndice, registroIndice, indiceLinearLerRegistro);

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
    
    bOffAnterior = args->bOffPrimOcorArqIndice - (int64_t) registroIndice->tamanhoRegistroLinear;
    if (bOffAnterior > 0) {
      resetLeituraDeArquivo(args->arquivoIndiceBin, bOffAnterior);
      
      indiceLerRegistroDoArquivoBinario(args->arquivoIndiceBin, registroIndice, indiceLinearLerRegistro);

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
    int64_t bOffProx = args->bOffPrimOcorArqIndice + (int64_t) registroIndice->tamanhoRegistroLinear;
    indiceLerRegistroDoArquivoBinario(args->arquivoIndiceBin, registroIndice, indiceLinearLerRegistro);
    resetLeituraDeArquivo(args->arquivoDadosBin, registroIndice->byteOffset);
    dadosLerRegistroDoArquivoBinario(args->arquivoDadosBin, args->registro);
    regComp = dadosCompararRegistroComChaveBusca(args);
  }
  while (regComp == 0) {
    dadosAtualizarByteoffset(args->registro, registroIndice->byteOffset);
    dadosEncontrar(args, ftnPorRegistro);
    indiceLerRegistroDoArquivoBinario(args->arquivoIndiceBin, registroIndice, indiceLinearLerRegistro);
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

  resetLeituraDeArquivo(args->arquivoIndiceBin, cabecalhoIndice->tamanhoRegistroLinear);

  char* tipoDado = entradaObterTipoDado(entrada);

  REGISTRO_INDICE* registroBase = indiceRegistroInit(tipoDado);
  int32_t tamRegIndice = registroBase->tamanhoRegistroLinear;
  indiceRegistroApagar(&registroBase, tipoDado);

  int32_t pivotSuperior = 
    tamRegIndice * (int32_t) (qtdReg - 1) + 
    cabecalhoIndice->tamanhoRegistroLinear;

  int32_t pivotCentral = (int32_t) (pivotSuperior / 2);
  int32_t sobra = pivotCentral%tamRegIndice;
  pivotCentral -= (tamRegIndice - sobra);
  pivotCentral += 1;

  int32_t pivotInferior = cabecalhoIndice->tamanhoRegistroLinear;

  while ( pivotSuperior >= pivotInferior && 
          pivotCentral >= (pivotInferior) && 
          pivotCentral <= pivotSuperior) {
    resetLeituraDeArquivo(args->arquivoIndiceBin, pivotCentral);

    REGISTRO_INDICE* registroIndice = indiceRegistroInit(tipoDado);
    if (registroIndice == NULL) continue;
    
    indiceLerRegistroDoArquivoBinario(args->arquivoIndiceBin, registroIndice, indiceLinearLerRegistro);
    
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
    sobra = pivotCentral%tamRegIndice - cabecalhoIndice->tamanhoRegistroLinear;
    if (sobra < 0) {
      pivotCentral -= sobra;
      pivotCentral -= registroIndice->tamanhoRegistroLinear;
    } else {
      pivotCentral -= sobra;
    }

    dadosRegistroApagar(&registro);
    indiceRegistroApagar(&registroIndice, tipoDado);
  }

  indiceCabecalhoApagar(&cabecalhoIndice);
  return true;
}

void ordenarChaves(int n, CHAVE* arr[n]) {
  bool swapped;
  for (int i = 0; i < n-1; i++) {
    CHAVE* arr_i = arr[i];
    swapped = false;
    for (int j = 0; j < n - i - 1; j++) {
      CHAVE* arr_j = arr[j];
      if (arr[j]->C > arr[j+1]->C) {
        CHAVE* temp = arr[j];
        arr[j] = arr[j+1];
        arr[j+1] = temp;
        swapped = true;
      }
    }
  }
}

bool indiceOrdenarChavesNoRegistro(REGISTRO_INDICE* registroIndice) {
  if (registroIndice == NULL) return false;
  if (registroIndice->n == 0) return false;
  if (registroIndice->n == 1) return true;

  bool swapped;
  int numChaves = ORDEM_ARVOREB-1;
  for (int i = 0; i < numChaves-1; i++) {
    CHAVE* chave_i = registroIndice->chaves[i];
    if (chave_i->C == -1) break;
    swapped = false;
    for (int j = 0; j < numChaves - i - 1; j++) {
      CHAVE* chave_j = registroIndice->chaves[j];
      if (registroIndice->chaves[j+1]->C == -1) continue;
      if (chave_j->C > registroIndice->chaves[j+1]->C) {
        CHAVE* temp = chave_j;
        registroIndice->chaves[j] = registroIndice->chaves[j+1];
        registroIndice->chaves[j+1] = temp;

        int32_t tempP = registroIndice->P[j];
        registroIndice->P[j] = registroIndice->P[j+1];
        registroIndice->P[j+1] = tempP;
        swapped = true;
      }
      if (swapped == false) break;
    }
  }
}

bool indiceDeslocarChavesParaDireita(REGISTRO_INDICE* registroIndice) {
  if (registroIndice == NULL) return false;
  if (registroIndice->n == ORDEM_ARVOREB-1) return false;
  if (registroIndice->n == 0) return true;

  if (registroIndice->chaves[0]->C == -1) return true;

  for (int i = registroIndice->n-1; i >= 0; i--) {
    CHAVE* chave_i = registroIndice->chaves[i];
    registroIndice->chaves[i+1]->C = chave_i->C;
    registroIndice->chaves[i+1]->Pr = chave_i->Pr;

  }
  return true;
}

REGISTRO_INDICE** split1para2(ARGS* args) {
  REGISTRO_INDICE** novosRegCriados = (REGISTRO_INDICE**) malloc(sizeof(REGISTRO_INDICE*)*2);
  char* tipoDado = entradaObterTipoDado(args->entrada);

  /**
   * 1. Criar no promovido [0]
   * 2. Criar no irmao direito [1] 
   * 3. Redistribuir chaves ordenadas
   * 4. Atualizar registro raiz
  */
  
  novosRegCriados[0] = indiceRegistroInit(tipoDado);
  novosRegCriados[1] = indiceRegistroInit(tipoDado);
  
  REGISTRO_INDICE* registroIndicePromovido = novosRegCriados[0];
  REGISTRO_INDICE* registroIndiceIrmaoDireito = novosRegCriados[1];

  int32_t numChaves = args->registroIndice->n;
  int32_t idParaInserir = dadosObterIdCrime(args->registro);
  CHAVE* chavesId[numChaves+1];
  
  for (int i = 0; i < numChaves; i++) {
    chavesId[i] = arvoreBchaveInit();
    chavesId[i]->C = args->registroIndice->chaves[i]->C;
    chavesId[i]->Pr = args->registroIndice->chaves[i]->Pr;
  }
  CHAVE* novaChave = arvoreBchaveInit();
  novaChave->C = idParaInserir;
  novaChave->Pr = dadosObterByteoffset(args->registro);
  chavesId[numChaves] = novaChave;

  ordenarChaves(numChaves+1, chavesId);

  int posPromovida = (int) (numChaves+1)/2;

  int i = 0;
  /**
   * Para o registro original: 
   * 1. Manter 2 chaves menores
   * 2. Limpar 3 chaves restantes
  */
  for (i = 0; i < posPromovida; i++) {
    args->registroIndice->chaves[i]->C = chavesId[i]->C;
    args->registroIndice->chaves[i]->Pr = chavesId[i]->Pr;
  }

  for (i = posPromovida; i < numChaves; i++) {
    args->registroIndice->chaves[i]->C = -1;
    args->registroIndice->chaves[i]->Pr = -1;
    args->registroIndice->n--;
  }

  /**
   * Para o novo registro irmao direito:
   * 1. Adicionar 2 ultimas chaves ao inicio
  */
  int j = 0;
  for (i = posPromovida+1; i < numChaves+1; i++) {
    registroIndiceIrmaoDireito->chaves[j]->C = chavesId[i]->C;
    registroIndiceIrmaoDireito->chaves[j]->Pr = chavesId[i]->Pr;
    j++;
    registroIndiceIrmaoDireito->n++;
  }

  /**
   * Para o registro promovido:
   * 1. Adicionar 1 chave central
   * 2. Atualizar 
   *    - P[0]: noRaiz e 
   *    - P[1]: noRaiz + sizeof(registroIrmao)
  */
  registroIndicePromovido->chaves[0]->C = chavesId[posPromovida]->C;
  registroIndicePromovido->chaves[0]->Pr = chavesId[posPromovida]->Pr;
  registroIndicePromovido->P[0] = args->cabecalhoIndice->noRaiz;
  registroIndicePromovido->P[1] = 
    args->cabecalhoIndice->noRaiz + 
    args->registroIndice->tamanhoRegistroArvoreB;
  registroIndicePromovido->n++;

  return novosRegCriados;
}

bool registroIndiceCheio(ARGS* args) {
  return args->registroIndice->n == ORDEM_ARVOREB-1 ? true : false;
}

void evitarPrint(ARGS* args) {
  return;
}

bool indiceArvoreBVarreduraDeBuscaArquivoBinario(
  ARGS* args, void (ftnPorRegistro(ARGS*))
) {
  if (args == NULL || ftnPorRegistro == NULL) return false;

  REGISTRO_INDICE* registroIndice = args->registroIndice;
  int32_t numChaves = ORDEM_ARVOREB - 1;

  FILE* arquivoIndice = args->arquivoIndiceBin;
  int32_t regIdCrime = args->idCrimeBuscado;
  int64_t proxRamoPR = -1;

  for (int i = 0; i <= numChaves; i++) {
    proxRamoPR = registroIndice->P[i];

    if (
      (i == numChaves) ||
      (regIdCrime < registroIndice->chaves[i]->C) || 
      (registroIndice->chaves[i]->C == -1)
    ) break;
  }

  for (int i = 0; i < numChaves; i++) {
    if (registroIndice->chaves[i]->C == regIdCrime) {
      FILE* arquivoDados = args->arquivoDadosBin;
      int64_t PrDado = registroIndice->chaves[i]->Pr;

      REGISTRO* registroDado = dadosRegistroInit();
      args->registro = registroDado;
      
      resetLeituraDeArquivo(arquivoDados, PrDado);
      dadosLerRegistroDoArquivoBinario(arquivoDados, registroDado);
      
      int funcionalidade = entradaObterFuncionalidade(args->entrada);
      bool encontrado = args->regEncontradoEmBusca;
      if (funcionalidade == 9) {
        dadosEncontrar(args, ftnPorRegistro);
      }
      if (funcionalidade == 8) {
        dadosEncontrar(args, evitarPrint);
        ftnPorRegistro(args);
      }

      if (encontrado) {
        dadosRegistroApagar(&registroDado);
        return false;
      }

      return true;
    }
  }

  /**
   * Se ramoPR != -1, entao existe no folha
   * 1. Comparar registro a ser inserido com
   *  - P[0] : no mais a esquerda
   *  - ...
   *  - P[2] : no mais a direita
   * 
   * 2. Reset ponteiro de leitura para ramoPR
   * 3. Ler registro
   * 4. Realizar varredura novamente
  */
  if (proxRamoPR != -1) {
    int64_t proxBoffRamoP = 
      (proxRamoPR * registroIndice->tamanhoRegistroArvoreB) +
      args->cabecalhoIndice->tamanhoRegistroArvoreB;
    resetLeituraDeArquivo(arquivoIndice, proxBoffRamoP);
    indiceLerRegistroDoArquivoBinario(arquivoIndice, registroIndice, indiceArvoreBLerRegistro);
    return indiceArvoreBVarreduraDeBuscaArquivoBinario(args, ftnPorRegistro);
  }

  ftnPorRegistro(args);
  
  return false;
}


// bool indiceArvoreBVarreduraDeBuscaArquivoBinario(
//   ARGS* args, int64_t ramoPR, void (ftnPorRegistro(ARGS*))
// ) {
//   if (args == NULL || ftnPorRegistro == NULL) return false;

//   REGISTRO_INDICE* registroIndice = args->registroIndice;
//   int32_t numChaves = ORDEM_ARVOREB - 1;

//   FILE* arquivoIndice = args->arquivoIndiceBin;
//   int32_t regIdCrime = dadosObterIdCrime(args->registro);
//   int64_t proxRamoPR = -1;

//   for (int i = 0; i < numChaves; i++) {
//     proxRamoPR = registroIndice->P[i];
//     if (
//       (regIdCrime < registroIndice->chaves[i]->C || 
//       registroIndice->chaves[i]->C == -1)
//     ) break;
//   }

//   /**
//    * Se ramoPR != -1, entao existe no folha
//    * 1. Comparar registro a ser inserido com
//    *  - P[0] : no mais a esquerda
//    *  - ...
//    *  - P[2] : no mais a direita
//    * 
//    * 2. Reset ponteiro de leitura para ramoPR
//    * 3. Ler registro
//    * 4. Realizar varredura novamente
//   */
//   if (proxRamoPR != -1) {
//     resetLeituraDeArquivo(arquivoIndice, proxRamoPR);
//     indiceLerRegistroDoArquivoBinario(arquivoIndice, registroIndice, indiceArvoreBLerRegistro);
//     return indiceArvoreBVarreduraDeBuscaArquivoBinario(args, proxRamoPR, ftnPorRegistro);
//   }

//   /**
//    * Se n < 3, entao registro tem espaco:
//    * 1. Inserir nova chave
//    * 2. Ordenar
//    * 3. Armazenar registro no indice
//   */
//   if (registroIndice->n < numChaves) {
//     bool deslocado = indiceDeslocarChavesParaDireita(registroIndice);
//     if (deslocado) {
//       registroIndice->chaves[0]->C = dadosObterIdCrime(args->registro);
//       registroIndice->chaves[0]->Pr = dadosObterByteoffset(args->registro);
//       registroIndice->n++;
//     }

//     indiceOrdenarChavesNoRegistro(registroIndice);
//     resetLeituraDeArquivo(args->arquivoIndiceBin, ramoPR);
//     indiceArvoreBArmazenarRegistro(args);

//     args->cabecalhoIndice->nroChaves++;
//     return true;
//   }

//   // Se ramoPR == -1: no folha
//   /**
//    * 1. se noRaiz: split1-2
//    *  - Atualizar novo no raiz
//    *  - Atualizar RRNproxNo com posicao final do arqIndice
//    *  - Atualizar nroNiveis (+1)
//    * 2. contrario: split2-3
//   */
//   if (proxRamoPR == -1) {
//     int64_t noRaiz = args->cabecalhoIndice->noRaiz;
//     if (ramoPR == noRaiz) {
//       REGISTRO_INDICE** novosRegs = split1para2(args);
//       args->byteOffPaiRegSplitado = ramoPR;
//       REGISTRO_INDICE* regPromovido = novosRegs[0];
//       REGISTRO_INDICE* regIrmaoDireito = novosRegs[1];

//       args->registroIndice = regPromovido;
//       // indiceArvoreBVarreduraDeBuscaArquivoBinario(args, ramoPR, ftnPorRegistro);

//       args->cabecalhoIndice->noRaiz = noRaiz + 
//         2 * args->registroIndice->tamanhoRegistroArvoreB;
//       args->cabecalhoIndice->RNNproxNo = 
//         args->cabecalhoIndice->noRaiz + 
//         args->registroIndice->tamanhoRegistroArvoreB;


//       args->cabecalhoIndice->nroNiveis++;
      

//       resetLeituraDeArquivo(arquivoIndice, ramoPR);
//       ftnPorRegistro(args);
//       int64_t bOffIrmaoDireito = 
//         args->cabecalhoIndice->RNNproxNo - 
//         2*args->registroIndice->tamanhoRegistroArvoreB;
//       resetLeituraDeArquivo(arquivoIndice, bOffIrmaoDireito);
//       args->registroIndice = regIrmaoDireito;
//       ftnPorRegistro(args);
//       args->registroIndice = regPromovido;
//       ftnPorRegistro(args);
//     }
//     else {
//       REGISTRO_INDICE** novosRegs = split1para2(args);
//       args->byteOffPaiRegSplitado = ramoPR;
//       REGISTRO_INDICE* regPromovido = novosRegs[0];
//       REGISTRO_INDICE* regIrmaoDireito = novosRegs[1];

//       // indiceArvoreBVarreduraDeBuscaArquivoBinario(args, ramoPR, ftnPorRegistro);

//       args->cabecalhoIndice->noRaiz = noRaiz + 
//         2 * args->registroIndice->tamanhoRegistroArvoreB;
//       args->cabecalhoIndice->RNNproxNo = 
//         args->cabecalhoIndice->noRaiz + 
//         args->registroIndice->tamanhoRegistroArvoreB;


//       args->cabecalhoIndice->nroNiveis++;
      

//       resetLeituraDeArquivo(arquivoIndice, ramoPR);
//       ftnPorRegistro(args);
//       args->registroIndice = regIrmaoDireito;
//       ftnPorRegistro(args);
//       args->registroIndice = regPromovido;
//       ftnPorRegistro(args);
//     }
//   }
// }

void indiceArvoreBRotinaSplitDeInsercao(ARGS* args) {
  REGISTRO_INDICE* registroIndice = args->registroIndice;
  int32_t numChaves = ORDEM_ARVOREB - 1;

  FILE* arquivoIndice = args->arquivoIndiceBin;
  int32_t regIdCrime = dadosObterIdCrime(args->registro);
  int64_t proxRamoPR = -1;
  int64_t ramoPR = args->byteOffPaiRegSplitado;
  if (registroIndice->n < numChaves) {
    bool deslocado = indiceDeslocarChavesParaDireita(registroIndice);
    if (deslocado) {
      registroIndice->chaves[0]->C = dadosObterIdCrime(args->registro);
      registroIndice->chaves[0]->Pr = dadosObterByteoffset(args->registro);
      registroIndice->n++;
    }

    indiceOrdenarChavesNoRegistro(registroIndice);
    resetLeituraDeArquivo(args->arquivoIndiceBin, ramoPR);
    indiceArvoreBArmazenarRegistro(args);

    args->cabecalhoIndice->nroChaves++;
    return;
  }
}

void indiceArvoreBInserirRegistro(ARGS* args) {
  if (args == NULL) return;

  FILE* arquivoIndice = args->arquivoIndiceBin;
  char* tipoDado = entradaObterTipoDado(args->entrada);

  /**
   * Se noRaiz == -1: indice vazio
   * 1. criar novo registro em RAM
   * 2. set noRaiz byteOffset pelo tamanho do cabecalho
   * 2. atualizar novo registro com (idCrime, PR1)
   * 3. armazenar registro no arquivo de indice
   * */ 

  REGISTRO_INDICE* registroIndice = indiceRegistroInit(tipoDado);
  int64_t noRaiz = args->cabecalhoIndice->noRaiz;
  args->registroIndice = registroIndice;

  int64_t tamRegIndiceCabec = args->cabecalhoIndice->tamanhoRegistroArvoreB;
  int64_t tamRegIndice = args->registroIndice->tamanhoRegistroArvoreB;

  int64_t noRaizBoff = 
    (noRaiz * tamRegIndice) + 
    tamRegIndiceCabec;

  if (noRaiz == -1) {
    args->cabecalhoIndice->noRaiz = 0;
    noRaiz = args->cabecalhoIndice->noRaiz;
    noRaizBoff = (noRaiz * tamRegIndice) + tamRegIndiceCabec;
    args->registroIndice->chaves[0]->C = dadosObterIdCrime(args->registro);
    args->registroIndice->chaves[0]->Pr = dadosObterByteoffset(args->registro);
    args->registroIndice->n++;

    args->cabecalhoIndice->RNNproxNo++;
    args->cabecalhoIndice->proxByteOffsetNoRaiz = noRaiz;

    args->cabecalhoIndice->nroChaves++;
    resetLeituraDeArquivo(args->arquivoIndiceBin, noRaizBoff);
    indiceArvoreBArmazenarRegistro(args);

    return;
  }

  /**
   * Se noRaiz != -1: indice ja inicializado
   * 1. Ler registro noRaiz
   * 2. Buscar registro
   * 3. Realizar insercao
  */

  resetLeituraDeArquivo(args->arquivoIndiceBin, noRaizBoff);

  indiceArvoreBLerRegistro(arquivoIndice, registroIndice);
  args->idCrimeBuscado = dadosObterIdCrime(args->registro);
  indiceArvoreBVarreduraDeBuscaArquivoBinario(args, indiceArvoreBRotinaSplitDeInsercao);

  // executarSplit(args, args->cabecalhoIndice->noRaiz);

  return;
}

bool argsInitInformativoParaArvoreB(ARGS* args) {
  if (args == NULL) return false;

  char* arquivoIndice = entradaObterArquivoIndice(args->entrada);
  FILE* arquivoIndiceBin = fopen(arquivoIndice, MODO_LEITURA_ARQUIVO);
  if (arquivoIndiceBin == NULL) {
    erroGenerico();
    return false;
  };

  CABECALHO_INDICE* cabecalhoIndice = indiceCabecalhoInit();
  args->cabecalhoIndice = cabecalhoIndice;
  args->arquivoIndiceBin = arquivoIndiceBin;

  return true;
}

bool indiceBuscaPorIndiceArvoreB(ENTRADA* entrada, ARGS* args, void (*ftnPorBusca)()) {
  if (entrada == NULL || args == NULL) return false;
  if (ftnPorBusca == NULL) return false;

  CABECALHO_INDICE* cabecalhoIndice = args->cabecalhoIndice;
  indiceLerCabecalhoDoArquivoBinario(args->arquivoIndiceBin, cabecalhoIndice, indiceArvoreBLerCabecalhoDoArquivoBinario);

  if (cabecalhoIndice->status == '0') {
    indiceCabecalhoApagar(&cabecalhoIndice);
    erroGenerico();
    return false;
  }

  if (cabecalhoIndice->nroChaves == 0) {
    indiceCabecalhoApagar(&cabecalhoIndice);
    erroSemRegistros();
    return false;
  }

  FILE* arquivoIndice = args->arquivoIndiceBin;
  char* tipoDado = entradaObterTipoDado(args->entrada);

  REGISTRO_INDICE* registroIndice = indiceRegistroInit(tipoDado);
  args->registroIndice = registroIndice;

  cabecalhoIndice->proxByteOffsetNoRaiz = 
    (cabecalhoIndice->noRaiz * args->registroIndice->tamanhoRegistroArvoreB) +
    cabecalhoIndice->tamanhoRegistroArvoreB;

  cabecalhoIndice->proxByteOffsetProxNo = 
    (cabecalhoIndice->RNNproxNo * args->registroIndice->tamanhoRegistroArvoreB) + 
    cabecalhoIndice->tamanhoRegistroArvoreB;

  int32_t noRaiz = args->cabecalhoIndice->noRaiz;
  int64_t proxBoffRaiz = cabecalhoIndice->proxByteOffsetNoRaiz;
  if (noRaiz == -1) {
    indiceRegistroApagar(&registroIndice, tipoDado);
    return false;
  }

  resetLeituraDeArquivo(arquivoIndice, proxBoffRaiz);
  indiceLerRegistroDoArquivoBinario(arquivoIndice, registroIndice, indiceArvoreBLerRegistro);

  indiceArvoreBVarreduraDeBuscaArquivoBinario(args, ftnPorBusca);

  indiceRegistroApagar(&registroIndice, tipoDado);

  return true;
}

CABECALHOBESTRELA *ArvoreBCriarCabecalho() {
  CABECALHOBESTRELA *cabecalho = malloc(sizeof(CABECALHOBESTRELA));
  if (cabecalho == NULL) {
    printf("Sem memória RAM. Abandonando programa!");
    exit(1);
  }
  return cabecalho;
}

void imprimirCabecalhoArvoreB(CABECALHOBESTRELA *cabecalho) {
  printf("CABECALHO:\nStatus: %c\n", cabecalho->status);
  printf("NoRaiz: %d\n", cabecalho->noRaiz);
  printf("proxRRN: %d\n", cabecalho->proxRRN);
  printf("nroNiveis: %d\n", cabecalho->nroNiveis);
  printf("nroChaves: %d\n", cabecalho->nroChaves);
}

void removerCabecalhoArvoreB(CABECALHOBESTRELA** cabecalho) {
  free(*cabecalho);
  *cabecalho = NULL;
}

CABECALHOBESTRELA *lerCabecalhoArvoreB(FILE *indiceBinario) {
  CABECALHOBESTRELA *cabecalho = ArvoreBCriarCabecalho();
  fread(&cabecalho->status, sizeof(char), 1, indiceBinario);
  fread(&cabecalho->noRaiz, sizeof(int32_t), 1, indiceBinario);
  fread(&cabecalho->proxRRN, sizeof(int32_t), 1, indiceBinario);
  fread(&cabecalho->nroNiveis, sizeof(int32_t), 1, indiceBinario);
  fread(&cabecalho->nroChaves, sizeof(int32_t), 1, indiceBinario);
  return cabecalho;
}

BTPAGE *criarPaginaArvoreB() {
  BTPAGE *pagina = malloc(sizeof(BTPAGE));
  if (pagina == NULL) {
    printf("Sem memória RAM. Abandonando programa!");
    exit(1);
  }
  return pagina;
}

BTPAGE *preencherPaginaArvoreB(int nivel, int contadorDeOcupacao, CHAVE chaves[], int Pfinal) {
  BTPAGE* pagina = criarPaginaArvoreB();
  pagina->nivel = nivel;
  pagina->contadorDeOcupacao = contadorDeOcupacao;
  for(int i = 0; i < 4; i++) {
    if(i < contadorDeOcupacao) {
      pagina->chaves[i] = chaves[i];
      pagina->P[i] = chaves[i].Pp;
    } else {
      pagina->chaves[i].C = -1;
      pagina->chaves[i].Pr = -1;
      pagina->P[i] = -1;
    }
    pagina->P[contadorDeOcupacao] = Pfinal;
  }
  return pagina;
}

void removerPaginaArvoreB(BTPAGE** pagina) {
  free(*pagina);
  *pagina = NULL;
}

int arvoreBObterNivel(BTPAGE* pagina) {
  if(pagina != NULL) {
    return pagina->nivel;
  }
  return -1;
}

int arvoreBObterOcupacao(BTPAGE* pagina) {
  if(pagina != NULL) {
    return pagina->contadorDeOcupacao;
  }
  return -1;
}

int *arvoreBObterP(BTPAGE* pagina) {
  if(pagina != NULL) {
    return pagina->P;
  }
  return NULL;
}

CHAVE *arvoreBObterChaves(BTPAGE* pagina) {
  if(pagina != NULL) {
    return pagina->chaves;
  }
  return NULL;
}

void acessarPaginaArvoreB(FILE *indiceBinario, BTPAGE* pagAux, int32_t RRN) {
  fseek(indiceBinario, (RRN+1)*76, SEEK_SET);
  fread(&pagAux->nivel, sizeof(int32_t), 1, indiceBinario);
  fread(&pagAux->contadorDeOcupacao, sizeof(int32_t), 1, indiceBinario);
  for(int i = 0; i < 4; i++) {
    fread(&pagAux->P[i], sizeof(int32_t), 1, indiceBinario);
    fread(&pagAux->chaves[i].C, sizeof(int32_t), 1, indiceBinario);
    fread(&pagAux->chaves[i].Pr, sizeof(int64_t), 1, indiceBinario);
  }
  fread(&pagAux->P[4], sizeof(int32_t), 1, indiceBinario);
}

void imprimirPaginaArvoreB(BTPAGE *pagina) {
  int i;
  printf("\nPAGINA:\nNivel: %d\n", pagina->nivel);
  printf("Contador de Ocupacao: %d\n", pagina->contadorDeOcupacao);
  for(i = 0; i < pagina->contadorDeOcupacao; i++) {
    printf("\nPonteiro %d p/ Página: %d\n", i+1, pagina->P[i]);
    printf("\n--Chave %d: %d\n", i+1, pagina->chaves[i].C);
    printf("--Ponteiro %d p/ Arquivo: %ld\n", i+1, pagina->chaves[i].Pr);
  }
  printf("\nPonteiro %d p/ Página: %d\n", i+1, pagina->P[i]);
}


// TABELA.C
FILE *abrirIndiceArvoreB(char *nomeIndice) {
  char indisponivel = '0';
  FILE *indiceBinario = fopen(nomeIndice, "rb+");
  if (indiceBinario == NULL) {
    printf("Erro ao abrir o arquivo!");
    exit(1);
  }
  fwrite(&indisponivel, sizeof(char), 1, indiceBinario);
  fseek(indiceBinario, 0, SEEK_SET);
  return indiceBinario;
}

void fecharIndiceArvoreB(FILE* indiceBinario, bool error) {
  if(!error) {
    char disponivel = '1';
    fseek(indiceBinario, 0, SEEK_SET);
    fwrite(&disponivel, sizeof(char), 1, indiceBinario);
  }
  fclose(indiceBinario);
}

void atualizarPagina(FILE* indiceBinario, BTPAGE* pagina, int32_t RRN) {
  fseek(indiceBinario, 76*(RRN+1), SEEK_SET);
  int nivel = arvoreBObterNivel(pagina);
  int contadorDeOcupacao= arvoreBObterOcupacao(pagina);
  int campoVazio = -1;
  int *P = arvoreBObterP(pagina);
  CHAVE *chaves = arvoreBObterChaves(pagina);
  
  fwrite(&nivel, sizeof(int32_t), 1, indiceBinario);
  fwrite(&contadorDeOcupacao, sizeof(int32_t), 1, indiceBinario);
  
  for(int i = 0; i < 5; i++) {
    printf("\n%d\n", P[i]);
  }
  
  for(int i = 0; i < 4; i++) {
    if (i < contadorDeOcupacao) {
      fwrite(&P[i], sizeof(int32_t), 1, indiceBinario);
      fwrite(&chaves[i].C, sizeof(int32_t), 1, indiceBinario);
      fwrite(&chaves[i].Pr, sizeof(int64_t), 1, indiceBinario);
    } 
    if(i == contadorDeOcupacao) {
      fwrite(&P[contadorDeOcupacao], sizeof(int32_t), 1, indiceBinario);
      fwrite(&campoVazio, sizeof(int32_t), 1, indiceBinario);
      fwrite(&campoVazio, sizeof(int64_t), 1, indiceBinario);
    }
    if(i > contadorDeOcupacao) {
      fwrite(&campoVazio, sizeof(int32_t), 1, indiceBinario);
      fwrite(&campoVazio, sizeof(int32_t), 1, indiceBinario);
      fwrite(&campoVazio, sizeof(int64_t), 1, indiceBinario);
    } 
  }
}

int novaPagina(FILE* indiceBinario, CABECALHOBESTRELA* cabecalho, int32_t nivel, int32_t byteOffsetAtual) {
  CHAVE* chaves;
  BTPAGE* pagina = preencherPaginaArvoreB(nivel, 0, NULL, 0);
  fseek(indiceBinario, 76*(cabecalho->proxRRN+1), SEEK_SET);
  
  fseek(indiceBinario, byteOffsetAtual, SEEK_SET);
  char status;
  int noRaiz;
  int proxRRN;
  int nroNiveis;
  int nroChaves;
}

int64_t ArvoreBBuscar(FILE* indiceBinario, int RRN, int64_t chave) {
  if(RRN == -1) {
    return RRN;
  }
  int i;
  int32_t nivel;
  int32_t taxaDeOcupacao;
  int32_t ponteiroPagina;
  int32_t chaveComp;
  int64_t ponteiroArquivo;
  void* lixo = NULL;

  fseek(indiceBinario, (RRN+1)*76, SEEK_SET);
  fread(&nivel, sizeof(int32_t), 1, indiceBinario);
  fread(&taxaDeOcupacao, sizeof(int32_t), 1, indiceBinario);
  
  for(i = 0; i < taxaDeOcupacao; i++) {
    
    fread(&ponteiroPagina, sizeof(int32_t), 1, indiceBinario);
    fread(&chaveComp, sizeof(int32_t), 1, indiceBinario);
    fread(&ponteiroArquivo, sizeof(int64_t), 1, indiceBinario);
    
    if(chave < chaveComp) {
      return ArvoreBBuscar(indiceBinario, ponteiroPagina, chave);
    }
    if(chave == chaveComp) {
      return ponteiroArquivo;
    }
  }
  
  fread(&ponteiroPagina, sizeof(int32_t), 1, indiceBinario);
  return ArvoreBBuscar(indiceBinario, ponteiroPagina, chave); 
}

void ordenarChaves(CHAVE chaves[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (chaves[j].C > chaves[j + 1].C) {
                CHAVE temp = chaves[j];
                chaves[j] = chaves[j + 1];
                chaves[j + 1] = temp;
            }
        }
    }
}

void shiftPagina(FILE* indiceBinario, int RRN, CHAVE chave, int32_t nivel, int32_t taxaDeOcupacao) { 
  int paginaInexistente = -1;
  int novaTaxaDeOcupacao = taxaDeOcupacao+1;
  CHAVE chavesArquivadas[novaTaxaDeOcupacao];
  int ultimoPonteiroPagina;
  
  for(int i = 0; i < novaTaxaDeOcupacao; i++) {
    fread(&chavesArquivadas[i].Pp, sizeof(int32_t), 1, indiceBinario);
    fread(&chavesArquivadas[i].C, sizeof(int32_t), 1, indiceBinario);
    fread(&chavesArquivadas[i].Pr, sizeof(int64_t), 1, indiceBinario);
  }
  fread(&ultimoPonteiroPagina, sizeof(int64_t), 1, indiceBinario);
  chavesArquivadas[taxaDeOcupacao] = chave;

  ordenarChaves(chavesArquivadas, novaTaxaDeOcupacao);

  BTPAGE* pag = preencherPaginaArvoreB(nivel, novaTaxaDeOcupacao, chavesArquivadas, ultimoPonteiroPagina);
  atualizarPagina(indiceBinario, pag, RRN);
  removerPaginaArvoreB(&pag);

}

CHAVE* split1para2(FILE* indiceBinario, CABECALHOBESTRELA* cabecalho, int RRN, CHAVE chave, int32_t nivel) {
  int paginaInexistente = -1;
  CHAVE chavesArquivadas[4];
  int ultimoPonteiroPagina;
  
  for(int i = 0; i < 4; i++) {
    fread(&chavesArquivadas[i].Pp, sizeof(int32_t), 1, indiceBinario);
    fread(&chavesArquivadas[i].C, sizeof(int32_t), 1, indiceBinario);
    fread(&chavesArquivadas[i].Pr, sizeof(int64_t), 1, indiceBinario);
  }
  
}

// CHAVE* split(FILE* indiceBinario, CABECALHOBESTRELA* cabecalho, int RRN, CHAVE chave, int32_t nivel) {
//   if(nivel == 1 || nivel == cabecalho->nroNiveis) {
//     return split1para2(indiceBinario, cabecalho, RRN, chave, nivel);
//   } else {
    
//   }
  
  
  
//   for(int i = 0; i < 4; i++) {
//     fread(&chavesArquivadas[i].Pp, sizeof(int32_t), 1, indiceBinario);
//     fread(&chavesArquivadas[i].C, sizeof(int32_t), 1, indiceBinario);
//     fread(&chavesArquivadas[i].Pr, sizeof(int64_t), 1, indiceBinario);
//   }
  
// }

CHAVE* inserirArvoreB(FILE* indiceBinario, CABECALHOBESTRELA* cabecalho, int RRN, CHAVE chave) {
  int32_t nivel;
  int32_t taxaDeOcupacao;
  int32_t ponteiroPagina;
  int32_t chaveComp;
  int64_t ponteiroArquivo;
  int i;
  void* lixo = NULL;

  fseek(indiceBinario, (RRN+1)*76, SEEK_SET);
  fread(&nivel, sizeof(int32_t), 1, indiceBinario);
  fread(&taxaDeOcupacao, sizeof(int32_t), 1, indiceBinario);
  
  if(nivel == 1) {
    if(taxaDeOcupacao == 4) {
      //SPLIT
      printf("\nSPLIT!\n");
      
    } else {
      //SHIFT
      printf("\nSHIFT!\n");
      shiftPagina(indiceBinario, RRN, chave, 1, taxaDeOcupacao);
      return NULL;
    }
  } else {
    for(i = 0; i < taxaDeOcupacao; i++) {
    
      fread(&ponteiroPagina, sizeof(int32_t), 1, indiceBinario);
      fread(&chaveComp, sizeof(int32_t), 1, indiceBinario);
      fread(&ponteiroArquivo, sizeof(int64_t), 1, indiceBinario);
      
      if(chave.C < chaveComp) {
        return inserirArvoreB(indiceBinario, cabecalho, ponteiroPagina, chave);
      }
      if(chave.C == chaveComp) {
        //REGISTRO JÁ EXISTE
        printf("ERRO! Registro já existe.");
      }
    }
    
    fread(&ponteiroPagina, sizeof(int32_t), 1, indiceBinario);
    return inserirArvoreB(indiceBinario, cabecalho, ponteiroPagina, chave);
    
  }
  return NULL;
}