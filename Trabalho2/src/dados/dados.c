#include "dados.h"
#include "../indice/indice.h"

struct cabecalho_ {
  char status;
  int64_t proxByteOffset;
  int32_t nroRegArq;
  int32_t nroRegRem;
  int32_t tamanhoRegistro;
};

struct registro_ {
  int32_t idCrime;
  char* dataCrime;
  int32_t numeroArtigo; 
  char* marcaCelular;
  char* lugarCrime;
  char* descricaoCrime;
  char removido;
  char delimitadorCampos;
  char delimitador;

  int64_t tamanhoRegistro;
  int64_t byteOffset;
};


enum camposIndexados { 
  idCrime, dataCrime, numeroArtigo, marcaCelular, 
  lugarCrime, descricaoCrime, removido, delimitadorCampos,
  delimitador, tamanhoRegistro,
  ENUM_FIM
};

static const char* camposIndexadosNomes[ENUM_FIM] = {
  "idCrime", "dataCrime", "numeroArtigo", "marcaCelular", 
  "lugarCrime", "descricaoCrime", "removido", "delimitadorCampos", 
  "delimitador", "tamanhoRegistro"
};

const char* camposIndexadosTipos[ENUM_FIM] = {
  "inteiro", "string", "inteiro", "string", 
  "string", "descricaoCrime", "char", "char", 
  "char", "inteiro"
};

bool dadosArmazenarRegistro(REGISTRO* registro, FILE* arquivoBinarioDados);
bool resetLeituraDeArquivo(FILE* arquivoBin, int64_t byteOffset);
bool dadosAtualizarTamanhoRegistro(REGISTRO* registro);
bool dadosArmazenarCabecalho(CABECALHO* cabecalho, FILE* arquivoBinarioDados);
CABECALHO* dadosCabecalhoInit();
void copiarRegistro(REGISTRO* regOrig, REGISTRO* regDest);
REGISTRO* dadosRegistroInit();
bool dadosRegistroApagar(REGISTRO** registro) ;
bool dadosCabecalhoApagar(CABECALHO** cabecalho);
bool dadosCriarArquivoBinario(ENTRADA* entrada);
bool dadosLerCabecalhoDoArquivoBinario(FILE* arquivoBinarioDados, CABECALHO* cabecalho);
bool dadosLerRegistroDoArquivoBinario(FILE* arquivoBinarioDados, REGISTRO* registro);
bool strVazia(char* str);
void dadosImprimirRegistro(ARGS* args);
ARGS* argsInit();
bool argsApagar(ARGS** args);
ARGS* dadosVarreduraSequencialArquivoBinario(ENTRADA* entrada, void (*ftnPorRegistro)());
void* dadosObterCampoIndexado(REGISTRO* registro, char* campoIndexado);
int64_t dadosObterByteoffset(REGISTRO* registro);
bool dadosImprimirArquivoBinario(ENTRADA* entrada);
void dadosVarreduraDeBuscaArvoreBinaria(
  ARGS* args, NO* raiz, NO* anterior, 
  void (*ftnPorRegistro)(ARGS* args)
);
void dadosAdicionarRegistroEmArvoreBinaria(ARGS* args);
void dadosOrdernarRegistrosEmArvoreBinaria (ARGS* args) ;
ARGS* dadosRegistrosEmArvoreBinaria(ENTRADA* entrada);
void dadosArvoreBinariaApagarNo(ARGS* args, NO* raiz) ;
bool dadosArvoreBinariaApagar(ARGS* args);
const char* dadosObterTipoCampo(char* campoIndexado);

int64_t dadosObterByteoffset(REGISTRO* registro) {
  if (registro == NULL) return -1;
  return registro->byteOffset;
}

void preencherString(char* dest, char* orig, int tamanho) {
  for (int i = 0; i < strlen(orig); i++) {
    dest[i] = orig[i];
  }
  for (int i = strlen(orig); i < tamanho; i++) {
    dest[i] = '$';
  }
}

void removerPreenchimento(char* dest, char* orig) {
  for (int i = 0; i < strlen(orig); i++) {
    if (i > TAMANHO_CHAVE_BUSCA) break;
    if (orig[i] != '$') dest[i] = orig[i];
    else break;
  }
}

bool dadosArmazenarRegistro(REGISTRO* registro, FILE* arquivoBinarioDados) {
  if (registro == NULL || arquivoBinarioDados == NULL) return false;

  fwrite(&registro->removido, sizeof(char), 1, arquivoBinarioDados);
  fwrite(&registro->idCrime, sizeof(int32_t), 1, arquivoBinarioDados);

  char dataCrimePreenchida[TAMANHO_DATA_CRIME] = "";
  preencherString(dataCrimePreenchida, registro->dataCrime, TAMANHO_DATA_CRIME);

  fwrite(registro->dataCrime, sizeof(char), TAMANHO_DATA_CRIME, arquivoBinarioDados);

  fwrite(&registro->numeroArtigo, sizeof(int32_t), 1, arquivoBinarioDados);

  char marcaCelularPreenchida[TAMANHO_MARCA_CELULAR] = "";
  preencherString(marcaCelularPreenchida, registro->marcaCelular, TAMANHO_MARCA_CELULAR);

  fwrite(marcaCelularPreenchida, sizeof(char), TAMANHO_MARCA_CELULAR, arquivoBinarioDados);

  fwrite(registro->lugarCrime, strlen(registro->lugarCrime), 1, arquivoBinarioDados);
  fwrite(&registro->delimitadorCampos, sizeof(char), 1, arquivoBinarioDados);

  fwrite(registro->descricaoCrime, strlen(registro->descricaoCrime), 1, arquivoBinarioDados);
  fwrite(&registro->delimitadorCampos, sizeof(char), 1, arquivoBinarioDados);    
  
  fwrite(&registro->delimitador, sizeof(char), 1, arquivoBinarioDados);

  return true;
}

bool resetLeituraDeArquivo(FILE* arquivoBin, int64_t byteOffset) {
  if (arquivoBin == NULL) return false;
  fseek(arquivoBin, byteOffset, SEEK_SET);
  return true;
}

bool dadosAtualizarTamanhoRegistro(REGISTRO* registro) {
  if (registro == NULL) return false;

  registro->tamanhoRegistro = 
    sizeof(registro->idCrime) + sizeof(char)*TAMANHO_DATA_CRIME + 
    sizeof(registro->numeroArtigo) + sizeof(char)*TAMANHO_MARCA_CELULAR + 
    sizeof(registro->removido) + sizeof(registro->delimitador);

  int64_t tamLugar = (int64_t) strlen(registro->lugarCrime);
  int64_t tamDesc = (int64_t) strlen(registro->descricaoCrime);
  registro->tamanhoRegistro += tamLugar + tamDesc;

  registro->tamanhoRegistro += sizeof(char) * (NUMERO_CAMPOS_VARIAVEIS);
}

bool dadosArmazenarCabecalho(CABECALHO* cabecalho, FILE* arquivoBinarioDados) {
  if (arquivoBinarioDados == NULL || cabecalho == NULL) return false;
  resetLeituraDeArquivo(arquivoBinarioDados, 0);
  fwrite(&cabecalho->status, sizeof(char), 1, arquivoBinarioDados);
  fwrite(&cabecalho->proxByteOffset, sizeof(int64_t), 1, arquivoBinarioDados);
  fwrite(&cabecalho->nroRegArq, sizeof(int32_t), 1, arquivoBinarioDados);
  fwrite(&cabecalho->nroRegRem, sizeof(int32_t), 1, arquivoBinarioDados);
  return true;
}

CABECALHO* dadosCabecalhoInit() {
  CABECALHO* cabecalho = (CABECALHO*) malloc(sizeof(CABECALHO));
  if (cabecalho == NULL) return false;

  cabecalho->status = '0';
  cabecalho->proxByteOffset = 0;
  cabecalho->nroRegArq = 0;
  cabecalho->nroRegRem = 0;
  cabecalho->tamanhoRegistro = 
    sizeof(cabecalho->status) + 
    sizeof(cabecalho->proxByteOffset ) + 
    sizeof(cabecalho->nroRegArq) + 
    sizeof(cabecalho->nroRegRem);

  return cabecalho;
}

void copiarRegistro(REGISTRO* regOrig, REGISTRO* regDest) {
  if (regOrig == NULL || regDest == NULL) return;
  regDest->idCrime = regOrig->idCrime;
  strcpy(regDest->dataCrime, regOrig->dataCrime);
  regDest->numeroArtigo = regOrig->numeroArtigo;
  strcpy(regDest->marcaCelular, regOrig->marcaCelular);
  strcpy(regDest->lugarCrime, regOrig->lugarCrime);
  strcpy(regDest->descricaoCrime, regOrig->descricaoCrime);
  regDest->removido = regOrig->removido;
  regDest->delimitadorCampos = regOrig->delimitador;
  regDest->delimitador = regOrig->delimitador;
  regDest->tamanhoRegistro = regOrig->tamanhoRegistro;
  regDest->byteOffset = regOrig->byteOffset;
}

REGISTRO* dadosRegistroInit() {
  REGISTRO* registro = (REGISTRO*) malloc(sizeof(REGISTRO));
  if (registro == NULL) return NULL;

  registro->idCrime = -1;
  registro->dataCrime = (char*) calloc((TAMANHO_DATA_CRIME + 1), sizeof(char));
  registro->numeroArtigo = -1; 
  registro->marcaCelular = (char*) calloc((TAMANHO_MARCA_CELULAR + 1), sizeof(char));
  registro->lugarCrime = (char*) calloc(TAM_MAX_STR, sizeof(char));
  registro->descricaoCrime = (char*) calloc(TAM_MAX_STR, sizeof(char));

  registro->removido = '0';
  registro->delimitadorCampos = '|';
  registro->delimitador = '#';
  registro->tamanhoRegistro = dadosAtualizarTamanhoRegistro(registro);
  registro->byteOffset = 0;

  return registro;
}

bool dadosRegistroApagar(REGISTRO** registro) {
  if (registro == NULL || *registro == NULL) return false;
  free((*registro)->dataCrime);
  (*registro)->dataCrime = NULL;
  free((*registro)->marcaCelular);
  (*registro)->marcaCelular = NULL;
  free((*registro)->lugarCrime);
  (*registro)->lugarCrime = NULL;
  free((*registro)->descricaoCrime);
  (*registro)->descricaoCrime = NULL;
  free(*registro);
  *registro = NULL;
  registro = NULL;
  return true;
}

bool dadosCabecalhoApagar(CABECALHO** cabecalho) {
  if (cabecalho == NULL || *cabecalho == NULL) return false;
  free(*cabecalho);
  *cabecalho = NULL;
  cabecalho = NULL;
  return true;
}

bool dadosCriarArquivoBinario(ENTRADA* entrada) {
  if (entrada == NULL) return false;

  char* arquivoEntrada = entradaObterArquivoEntrada(entrada);
  char* arquivoSaida = entradaObterArquivoSaida(entrada);

  FILE* arquivoDadosCsv = fopen(arquivoEntrada, "r");
  if (arquivoDadosCsv == NULL) {
    erroGenerico();
    return false;
  };
  FILE* arquivoDadosBin = fopen(arquivoSaida, MODO_ESCRITA_ARQUIVO);

  CABECALHO* cabecalho = dadosCabecalhoInit();
  
  int32_t tamanhoRegistroDados = 0;
  int32_t tamanhoRegistroCabecalho = 0;
  int32_t novoNroRegArq = 0;
  int32_t novoNroRegRem = 0;

  REGISTRO* registro = dadosRegistroInit();

  dadosArmazenarCabecalho(cabecalho, arquivoDadosBin);
  int64_t byteOffset = cabecalho->tamanhoRegistro;

  char str[256] = "";
  fgets(str, 256, arquivoDadosCsv);
  while(fgets(str, 256, arquivoDadosCsv)) {
      if(str[0] == '\n')
      continue;

      int j = 0, k = 0;
      char token[256] = "";
      for(int i = 0; i < strlen(str); i++) {
      if(str[i] == '\n' || str[i] == ',' || (i == strlen(str)-1 && feof(arquivoDadosCsv))) {
        
        if(i == strlen(str)-1 && feof(arquivoDadosCsv)) {
        token[j] = str[i];
        }
        
        switch(k) {
        case 0:
          if(strcmp(token, "") == 0) {
              erroGenerico(); 
              return NULL;
          }
          registro->idCrime = atoi(token);
          break;
        case 1:
          strcpy(registro->dataCrime, token);
          break;
        case 2:
          if(strlen(token) == 0)
            registro->numeroArtigo = -1;
          else
            registro->numeroArtigo = atoi(token);
          break;
        case 3:
          strcpy(registro->lugarCrime, token);
          break;
        case 4:
          strcpy(registro->descricaoCrime, token);
          break;
        case 5:
          strcpy(registro->marcaCelular, token);
        }
        k++;
        memset(token, 0, strlen(token));
        j = 0;
        continue;
      }
      token[j] = str[i];
      j++;
      }
  
      dadosArmazenarRegistro(registro, arquivoDadosBin);

      novoNroRegArq++;
      dadosAtualizarTamanhoRegistro(registro);
      tamanhoRegistroDados += registro->tamanhoRegistro;
      registro->byteOffset = byteOffset;
      byteOffset += tamanhoRegistroDados;

      if (registro->removido == '1') {
          novoNroRegRem = (int32_t)(novoNroRegRem + 1);
      }
  }

  cabecalho->status = '1';
  cabecalho->nroRegRem = novoNroRegRem;
  cabecalho->nroRegArq = novoNroRegArq;
  
  int64_t novoProxByteOffset = (int64_t)(tamanhoRegistroDados + cabecalho->tamanhoRegistro);
  cabecalho->proxByteOffset = novoProxByteOffset;
  cabecalho->nroRegRem = novoNroRegRem;

  dadosArmazenarCabecalho(cabecalho, arquivoDadosBin);

  dadosRegistroApagar(&registro);
  dadosCabecalhoApagar(&cabecalho);

  fclose(arquivoDadosBin);
  fclose(arquivoDadosCsv);

  binarioNaTela(arquivoSaida);
}

bool dadosLerCabecalhoDoArquivoBinario(FILE* arquivoBinarioDados, CABECALHO* cabecalho) {
  if (arquivoBinarioDados == NULL || cabecalho == NULL) return false;
  resetLeituraDeArquivo(arquivoBinarioDados, 0);
  fread(&cabecalho->status, sizeof(char), 1, arquivoBinarioDados);

  // char statusEmProgresso = '0';
  // resetLeituraDeArquivo(arquivoBinarioDados, 0);
  // fwrite(&statusEmProgresso, sizeof(char), 1, arquivoBinarioDados);
  fread(&cabecalho->proxByteOffset, sizeof(int64_t), 1, arquivoBinarioDados);
  fread(&cabecalho->nroRegArq, sizeof(int32_t), 1, arquivoBinarioDados);
  fread(&cabecalho->nroRegRem, sizeof(int32_t), 1, arquivoBinarioDados);
  return true;
}

bool dadosLerRegistroDoArquivoBinario(FILE* arquivoBinarioDados, REGISTRO* registro) {
  if (arquivoBinarioDados == NULL || registro == NULL) return false;

  char removidoAux = '0';
  fread(&registro->removido, sizeof(char), 1, arquivoBinarioDados);

  char descricaoAux[256] = "";
  char lugarAux[256] = ""; 

  fread(&registro->idCrime, sizeof(int32_t), 1, arquivoBinarioDados);
  fread(registro->dataCrime, sizeof(char), TAMANHO_DATA_CRIME , arquivoBinarioDados);
  fread(&registro->numeroArtigo, sizeof(int32_t), 1, arquivoBinarioDados);
  fread(registro->marcaCelular, sizeof(char), TAMANHO_MARCA_CELULAR, arquivoBinarioDados);

  int lugarAuxIndice = 0;
  while(1) {
    fread(&lugarAux[lugarAuxIndice], sizeof(char), 1, arquivoBinarioDados);
    if(lugarAux[lugarAuxIndice] == '|') {
      lugarAux[lugarAuxIndice] = '\0';
      break;
    }
    lugarAuxIndice++;
  }
  strcpy(registro->lugarCrime, lugarAux);

  int descricaoAuxIndice = 0;
  while(1) {
    fread(&descricaoAux[descricaoAuxIndice], sizeof(char), 1, arquivoBinarioDados);
    if(descricaoAux[descricaoAuxIndice] == '|') {
      descricaoAux[descricaoAuxIndice] = '\0';
      break;
    }
    descricaoAuxIndice++;
  }
  strcpy(registro->descricaoCrime, descricaoAux);
  
  fread(&registro->delimitador, sizeof(char), 1, arquivoBinarioDados);

  dadosAtualizarTamanhoRegistro(registro);

  return true;
}

bool strVazia(char* str) {
  if(strcmp(str, "") == 0 || str[0] == '$') {
    strcpy(str, STR_VAZIA);
    return true;
  }
  return false;
}

void dadosImprimirRegistro(ARGS* args) {
  REGISTRO* registro = args->registro;
  if (registro == NULL) return;

  int32_t idAux = registro->idCrime;
  int32_t numArtAux = registro->numeroArtigo;

  char dataAux[TAMANHO_DATA_CRIME+1] = ""; 
  strcpy(dataAux, registro->dataCrime);  

  char marcaAux[TAMANHO_MARCA_CELULAR+1] = "";
  strcpy(marcaAux, registro->marcaCelular);

  if(!strVazia(marcaAux)) {
    for(int j = 0; j < TAMANHO_MARCA_CELULAR-1; j++) {
      if(marcaAux[j] == '$') {
        marcaAux[j] = '\0';
        break;
      }
    }
  }
  
  char* lugarAux = registro->lugarCrime;
  char* descricaoAux = registro->descricaoCrime;
  
  strVazia(lugarAux);
  strVazia(descricaoAux);

  if(numArtAux == -1) {
    printf("%d, %s, %s, %s, %s, %s\n", idAux, dataAux, STR_VAZIA, lugarAux, descricaoAux, marcaAux); 
  } else {
    printf("%d, %s, %d, %s, %s, %s\n", idAux, dataAux, numArtAux, lugarAux, descricaoAux, marcaAux); 
  }
}

ARGS* argsInit() {
  ARGS* args = (ARGS*) malloc(sizeof(ARGS));
  args->arvoreBinaria = NULL;
  args->registro = NULL;
  args->entrada = NULL;
  args->arquivoDadosBin = NULL;
  args->arquivoIndiceBin = NULL;
  args->regEncontradoEmBusca = false;
  args->idProxRamo = 0;
  args->proxRamoArvore = NULL;
  args->bOffPrimOcorArqIndice = -1;
  return args;
}

bool argsApagar(ARGS** args) {
  if (args == NULL || *args == NULL) return false;
  dadosArvoreBinariaApagar(*args);
  free(*args);
  *args = NULL;
  args = NULL;
  return true;
}

ARGS* dadosVarreduraSequencialArquivoBinario(ENTRADA* entrada, void (*ftnPorRegistro)()) {
  char* arquivoEntrada = entradaObterArquivoEntrada(entrada);
  FILE* arquivoBinarioDados = fopen(arquivoEntrada, MODO_LEITURA_ARQUIVO);
  if (arquivoBinarioDados == NULL) {
    erroGenerico();
    return false;
  };

  CABECALHO* cabecalho = dadosCabecalhoInit();
  dadosLerCabecalhoDoArquivoBinario(arquivoBinarioDados, cabecalho);

  int32_t nroRegArq = cabecalho->nroRegArq;
  int64_t byteOffset = cabecalho->tamanhoRegistro;

  if (cabecalho->status == '0') {
    erroGenerico();
    fclose(arquivoBinarioDados);
    dadosCabecalhoApagar(&cabecalho);
    return NULL;
  }

  if (cabecalho->nroRegArq == 0) {
    erroSemRegistros();
    fclose(arquivoBinarioDados);
    dadosCabecalhoApagar(&cabecalho);
    return NULL;
  }
  ARGS* args = argsInit();
  args->entrada = entrada;
  args->arquivoDadosBin = arquivoBinarioDados;

  resetLeituraDeArquivo(arquivoBinarioDados, cabecalho->tamanhoRegistro);

  while (nroRegArq--) {
    REGISTRO* registro = dadosRegistroInit();
    if (registro == NULL) continue;
    args->registro = registro;
    
    dadosLerRegistroDoArquivoBinario(arquivoBinarioDados, registro);
    if (registro->removido == '1') {
      dadosRegistroApagar(&registro);
      continue;
    }

    registro->byteOffset = byteOffset;
    byteOffset += registro->tamanhoRegistro;

    ftnPorRegistro(args);

    dadosRegistroApagar(&registro);
  }

  dadosCabecalhoApagar(&cabecalho);
  fclose(arquivoBinarioDados);
  return args;
}

void* dadosObterCampoIndexado(REGISTRO* registro, char* campoIndexado) {
  if (registro == NULL) return NULL;

  void *dadosCamposEmRegistro[ENUM_FIM] = {
    &(registro->idCrime), registro->dataCrime, &(registro->numeroArtigo), registro->marcaCelular, 
    registro->lugarCrime, registro->descricaoCrime, &(registro->removido), &(registro->delimitadorCampos), 
    &(registro->delimitador), &(registro->tamanhoRegistro)
  };

  if (campoIndexado == NULL) return NULL;
  enum camposIndexados campo_i;
  for (campo_i = 0; campo_i < ENUM_FIM; ++campo_i) {
    if (strcmp(campoIndexado, camposIndexadosNomes[campo_i]) == 0) {
      return dadosCamposEmRegistro[campo_i];
    }
  }

  return NULL;
}

bool dadosImprimirArquivoBinario(ENTRADA* entrada) {
  if (entrada == NULL) return false;

  ARGS* args = dadosVarreduraSequencialArquivoBinario(entrada, dadosImprimirRegistro);
  argsApagar(&args);
}

void dadosVarreduraDeBuscaArvoreBinaria(
  ARGS* args, NO* raiz, NO* anterior, 
  void (*ftnPorRegistro)(ARGS*)
) {
  if (args == NULL || anterior == NULL) return;
  if (ftnPorRegistro == NULL || raiz == NULL) return;

  char* campoIndexado = entradaObterCampoIndexado(args->entrada);

  void* campoIndexadoPadrao = dadosObterCampoIndexado(args->registro, campoIndexado);
  void* campoIndexadoRaiz = dadosObterCampoIndexado(raiz->registro, campoIndexado);
  void* campoIndexadoAnterior = dadosObterCampoIndexado(anterior->registro, campoIndexado);

  char* tipoDado = entradaObterTipoDado(args->entrada);

  int ordem = 0;

  if (strcmp(tipoDado, "string") == 0) {
    char* valorPadrao = (char*) campoIndexadoPadrao;
    char* valorRaiz = (char*) campoIndexadoRaiz;
    char* valorAnterior = (char*) campoIndexadoAnterior;

    if (strlen(valorPadrao) == 0 || valorPadrao[0] == '$') {
      return;
    }

    if (strcmp(valorRaiz, valorPadrao) > 0) ordem = -1;
    else 
    if (strcmp(valorRaiz, valorPadrao) < 0) ordem = 1;
    else ordem = 0;
    
  } else {
    int32_t* valorPadrao = (int32_t*) campoIndexadoPadrao;
    int32_t* valorRaiz = (int32_t*) campoIndexadoRaiz;
    int32_t* valorAnterior = (int32_t*) campoIndexadoAnterior;

    if (*valorPadrao == -1) {
      return;
    }

    if (*valorPadrao < *valorRaiz) ordem = -1; 
    else
    if ((*valorPadrao > *valorRaiz)) ordem = 1;
    else ordem = 0;
  }

  args->idProxRamo = ordem;
  args->proxRamoArvore = raiz;

  if (ordem == 1) {
    dadosVarreduraDeBuscaArvoreBinaria(args, raiz->direita, raiz, ftnPorRegistro);
  } else 
  if (ordem == -1) {
    dadosVarreduraDeBuscaArvoreBinaria(args, raiz->esquerda, raiz, ftnPorRegistro);
  } else {
    ftnPorRegistro(args);
  }

  if (raiz->esquerda == NULL && ordem == -1) ftnPorRegistro(args);
  if (raiz->direita == NULL && ordem == 1) ftnPorRegistro(args);

  return;
}


void dadosAdicionarRegistroEmArvoreBinaria(ARGS* args) {
  if (args == NULL) return;

  NO* novoNo = (NO*) malloc(sizeof(NO));
  NO* anterior = args->proxRamoArvore;
  int ordem = args->idProxRamo;
  novoNo->registro = dadosRegistroInit();
  copiarRegistro(args->registro, novoNo->registro);
  novoNo->direita = NULL;
  novoNo->esquerda = NULL;
  args->arvoreBinaria->totalRegistros++;

  if (ordem == 1) {
    anterior->direita = novoNo;
    return;
  } else 
  if (ordem == -1) {
    anterior->esquerda = novoNo;
    return;
  } else {
    novoNo->esquerda = anterior->esquerda;
    anterior->esquerda = novoNo;

    if (args->arvoreBinaria->raiz == anterior) {
      REGISTRO* regRaizAux = args->arvoreBinaria->raiz->registro;
      args->arvoreBinaria->raiz->registro = novoNo->registro;
      novoNo->registro = regRaizAux;
    }

    if (novoNo->esquerda == NULL) {
      novoNo->direita = NULL;
    } else {
      novoNo->direita = novoNo->esquerda->direita;
      novoNo->esquerda->direita = NULL;
    }
  }
  return;
}

void dadosOrdernarRegistrosEmArvoreBinaria (ARGS* args) {
  if (args == NULL || args->registro == NULL) return;

  if (args->arvoreBinaria == NULL) {
    args->arvoreBinaria = (ARVORE_BINARIA*) malloc(sizeof(ARVORE_BINARIA));
    args->arvoreBinaria->totalRegistros = 1;
    args->arvoreBinaria->raiz = (NO*) malloc(sizeof(NO));
    args->arvoreBinaria->raiz->registro = dadosRegistroInit();
    copiarRegistro(args->registro, args->arvoreBinaria->raiz->registro);
    args->arvoreBinaria->raiz->direita = NULL;
    args->arvoreBinaria->raiz->esquerda = NULL;
    args->proxRamoArvore = args->arvoreBinaria->raiz;
    return;
  }

  dadosVarreduraDeBuscaArvoreBinaria(
    args, args->arvoreBinaria->raiz, 
    args->arvoreBinaria->raiz, 
    dadosAdicionarRegistroEmArvoreBinaria
  );
}

ARGS* dadosRegistrosEmArvoreBinaria(ENTRADA* entrada) {
  if (entrada == NULL) return NULL;
  ARGS* args = dadosVarreduraSequencialArquivoBinario(entrada, dadosOrdernarRegistrosEmArvoreBinaria);
  
  return args;
}

void dadosArvoreBinariaApagarNo(ARGS* args, NO* raiz) {
  if (args == NULL || raiz == NULL) return;
  dadosRegistroApagar(&(raiz->registro));
  raiz->direita = NULL;
  raiz->esquerda = NULL;
  free(raiz);
  raiz = NULL;
  return;
}

void dadosVarreduraCompletaArvoreBinaria(ARGS* args, NO* raiz, void (*ftnPorRegistro)()) {
  if (args == NULL || raiz == NULL || ftnPorRegistro == NULL) return;
  args->registro = raiz->registro;

  if (args->arvoreBinaria->ordem == preOrdem) ftnPorRegistro(args, raiz);
  dadosVarreduraCompletaArvoreBinaria(args, raiz->esquerda, ftnPorRegistro);

  if (args->arvoreBinaria->ordem == emOrdem) ftnPorRegistro(args, raiz);
  dadosVarreduraCompletaArvoreBinaria(args, raiz->direita, ftnPorRegistro);
  
  if (args->arvoreBinaria->ordem == posOrdem) ftnPorRegistro(args, raiz);
}

bool dadosArvoreBinariaApagar(ARGS* args) {
  if (args == NULL) return false;

  ARVORE_BINARIA* arvoreBinaria = args->arvoreBinaria;
  if (arvoreBinaria == NULL || arvoreBinaria->raiz == NULL) return false;

  args->registro = arvoreBinaria->raiz->registro;
  args->arvoreBinaria->ordem = posOrdem;
  dadosVarreduraCompletaArvoreBinaria(
    args, arvoreBinaria->raiz, dadosArvoreBinariaApagarNo);

  free(arvoreBinaria);
  arvoreBinaria = NULL;
  return true;
}

bool verificarSeCriterioDeBuscaIndexado(char* linhaDeBusca, char* campoIndexado) {
  char delimEspaco[] = " ";

  char linhaDeBuscaCp[100];
  strcpy(linhaDeBuscaCp, linhaDeBusca);
  char* linhaSplit = strtok(linhaDeBuscaCp, delimEspaco);

  bool match = false;
  while ((linhaSplit = strtok(NULL, delimEspaco)) != NULL) {
    if (strcmp(linhaSplit, "\n") == 0) break;
    char* campo = "";
    campo = linhaSplit;
    if (strcmp(campo, campoIndexado) == 0) {
      match = true;
      break;
    }
    const char* tipoCampo = dadosObterTipoCampo(campo);

    char* valorStr = "";
    int32_t valorInt = -1;
    if (strcmp(tipoCampo, "string") == 0) {
      char delimQuote[] = "\"";
      linhaSplit = strtok(NULL, delimQuote);
      valorStr = linhaSplit;
    } else {
      linhaSplit = strtok(NULL, delimEspaco);
      valorInt = (int32_t) atoi(linhaSplit);
    }
  }  
  
  return match;
}

int dadosCompararRegistroComChaveBusca(ARGS* args) {
  if (args == NULL) return 2;

  char* tipoDado = entradaObterTipoDado(args->entrada);

  char delimEspaco[] = " ";
  char* linhaDeBusca = entradaObterLinhaDeBusca(args->entrada);

  char linhaDeBuscaCp[100];
  strcpy(linhaDeBuscaCp, linhaDeBusca);
  char* linhaSplit = strtok(linhaDeBuscaCp, delimEspaco);

  while ((linhaSplit = strtok(NULL, delimEspaco)) != NULL) {
    if (strcmp(linhaSplit, "\n") == 0) break;
    char* campo = "";
    campo = linhaSplit;

    void* valorRegistro = dadosObterCampoIndexado(args->registro, campo);

    const char* tipoCampo = dadosObterTipoCampo(campo);

    char* valorStr = "";
    int32_t valorInt = -1;
    if (strcmp(tipoCampo, "string") == 0) {
      char delimQuote[] = "\"";
      linhaSplit = strtok(NULL, delimQuote);
      char valorTrunc[TAMANHO_CHAVE_BUSCA+1] = "";
      strcpy(valorTrunc, linhaSplit);

      char valorLimpo[TAMANHO_CHAVE_BUSCA+1] = "";
      char* valorReg = (char*) valorRegistro;
      removerPreenchimento(valorLimpo, valorReg);
      if (strcmp(campo, entradaObterCampoIndexado(args->entrada)) != 0) continue;
      return (int)strcmp(valorTrunc, valorLimpo);
      

    } else {
      linhaSplit = strtok(NULL, delimEspaco);
      valorInt = (int32_t) atoi(linhaSplit);
      int32_t* valorReg = (int32_t*) valorRegistro;
      if (strcmp(campo, entradaObterCampoIndexado(args->entrada)) != 0) continue;
      if (*valorReg < valorInt) return 1;
      if (*valorReg == valorInt) return 0;
      if (*valorReg > valorInt) return -1;
    }
  }
}

void dadosEncontrarEImprimirRegistro(ARGS* args) {
  if (args == NULL) return;

  char* tipoDado = entradaObterTipoDado(args->entrada);

  char delimEspaco[] = " ";
  char* linhaDeBusca = entradaObterLinhaDeBusca(args->entrada);

  char linhaDeBuscaCp[100];
  strcpy(linhaDeBuscaCp, linhaDeBusca);
  char* linhaSplit = strtok(linhaDeBuscaCp, delimEspaco);

  bool match = true;
  while ((linhaSplit = strtok(NULL, delimEspaco)) != NULL) {
    if (strcmp(linhaSplit, "\n") == 0) break;
    char* campo = "";
    campo = linhaSplit;

    void* valorRegistro = dadosObterCampoIndexado(args->registro, campo);

    const char* tipoCampo = dadosObterTipoCampo(campo);

    char* valorStr = "";
    int32_t valorInt = -1;
    if (strcmp(tipoCampo, "string") == 0) {
      char delimQuote[] = "\"";
      linhaSplit = strtok(NULL, delimQuote);
      char valorTrunc[TAMANHO_CHAVE_BUSCA+1] = "";
      strcpy(valorTrunc, linhaSplit);

      char valorLimpo[TAMANHO_CHAVE_BUSCA+1] = "";
      char* valorReg = (char*) valorRegistro;
      removerPreenchimento(valorLimpo, valorReg);
      if (strcmp(valorLimpo, valorTrunc) != 0) match = false;

    } else {
      linhaSplit = strtok(NULL, delimEspaco);
      valorInt = (int32_t) atoi(linhaSplit);
      int32_t* valorReg = (int32_t*) valorRegistro;
      if (*valorReg != valorInt) match = false;
    }
  }  

  if (match) {
    dadosImprimirRegistro(args);
    args->regEncontradoEmBusca = true;
  }
}

// void dadosBuscaBinariaEImprimirRegistro(ARGS* args, NO* raiz, int ordem) {
//   char* campoIndexado = entradaObterCampoIndexado(args->entrada);
  
//   char delim[] = " ";
//   char* linhaSplit = strtok(linhaDeBusca, delim);
//   while (linhaSplit != NULL) {
//     linhaSplit = strtok(NULL, delim);
//     char* campo = linhaSplit;
//     linhaSplit = strtok(NULL, delim);
//     char* valor = linhaSplit;

//     if (strcmp(campoIndexado, campo) == 0) {
//       args->registro = 
//     }
//   }
// }

bool dadosBuscarPorCampos(ENTRADA* entrada) {
  if (entrada == NULL) return false;

  char* campoIndexado = entradaObterCampoIndexado(entrada);

  int nroCamposBuscados = entradaObterNumeroCamposBuscados(entrada);
  char** linhasDeBusca = entradaObterLinhasBusca(entrada);
  char* linhaDeBusca = entradaObterLinhaDeBusca(entrada);
  for (int i = 0; i < nroCamposBuscados; i++) {
    if (entradaObterFuncionalidade(entrada) == 4) {
      printf("resposta para a busca %d\n", i+1);
    }
    bool buscaIndexada = verificarSeCriterioDeBuscaIndexado(linhaDeBusca, campoIndexado);
    ARGS* args = NULL;

    if (!buscaIndexada) {
      args = dadosVarreduraSequencialArquivoBinario(entrada, dadosEncontrarEImprimirRegistro);
    } else {
      args = indiceBuscaBinariaArquivoBinario(entrada, dadosEncontrarEImprimirRegistro);
    }

    if (args == NULL) continue;
    if (!args->regEncontradoEmBusca) erroSemRegistros();
    argsApagar(&args);

    linhaDeBusca = entradaProximaLinhaDeBusca(entrada);
  }
}

const char* dadosObterTipoCampo(char* campoIndexado) {
  enum camposIndexados campo_i = 0;
  const char* nomeCampo = camposIndexadosTipos[campo_i];
  for (campo_i = 0; campo_i < ENUM_FIM; ++campo_i) {
    if (strcmp(campoIndexado, camposIndexadosNomes[campo_i]) == 0) {
      nomeCampo = camposIndexadosTipos[campo_i];
    }
  }
  return nomeCampo;
}