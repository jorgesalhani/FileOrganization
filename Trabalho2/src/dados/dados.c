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
  bool atualizadoInplace;
};

enum sequenciaCamposAInserir {
  idCrimeInput, dataCrimeInput, numeroArtigoInput, lugarCrimeInput,
  descricaoCrimeInput, marcaCelularInput
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
  void (*ftnPorRegistro)()
);
void dadosAdicionarRegistroEmArvoreBinaria(ARGS* args);
void dadosOrdernarRegistrosEmArvoreBinaria (ARGS* args) ;
ARGS* dadosRegistrosEmArvoreBinaria(ENTRADA* entrada);
void dadosArvoreBinariaApagarNo(ARGS* args, NO* raiz) ;
bool dadosArvoreBinariaApagar(ARGS* args);

int64_t dadosObterByteoffset(REGISTRO* registro) {
  if (registro == NULL) return -1;
  return registro->byteOffset;
}

int64_t dadosObterRemovido(REGISTRO* registro) {
  if (registro == NULL) return -1;
  return registro->removido;
}

void dadosAtualizarByteoffset(REGISTRO* registro, int64_t byteOffset) {
  if (registro == NULL) return;
  registro->byteOffset = byteOffset;
}

void preencherString(char* dest, char* orig, int tamanho, bool comDelimitador) {
  int i = 0;
  for (i; i < strlen(orig); i++) {
    dest[i] = orig[i];
  }
  if (comDelimitador && i < tamanho) {
    dest[i] = '|';
    i++;
  }
  for (i; i < tamanho; i++) {
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
  preencherString(dataCrimePreenchida, registro->dataCrime, TAMANHO_DATA_CRIME, false);

  fwrite(registro->dataCrime, sizeof(char), TAMANHO_DATA_CRIME, arquivoBinarioDados);

  fwrite(&registro->numeroArtigo, sizeof(int32_t), 1, arquivoBinarioDados);

  char marcaCelularPreenchida[TAMANHO_MARCA_CELULAR] = "";
  preencherString(marcaCelularPreenchida, registro->marcaCelular, TAMANHO_MARCA_CELULAR, false);

  fwrite(marcaCelularPreenchida, sizeof(char), TAMANHO_MARCA_CELULAR, arquivoBinarioDados);

  fwrite(registro->lugarCrime, strlen(registro->lugarCrime), 1, arquivoBinarioDados);
  fwrite(&registro->delimitadorCampos, sizeof(char), 1, arquivoBinarioDados);

  fwrite(registro->descricaoCrime, strlen(registro->descricaoCrime), 1, arquivoBinarioDados);
  if (!registro->atualizadoInplace) {
    fwrite(&registro->delimitadorCampos, sizeof(char), 1, arquivoBinarioDados);    
  }
  
  fwrite(&registro->delimitador, sizeof(char), 1, arquivoBinarioDados);
  fflush(arquivoBinarioDados);
  return true;
}

int dadosObterIdCrime(REGISTRO* registro) {
  if (registro == NULL) return -1;
  return registro->idCrime;
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
  regDest->delimitadorCampos = regOrig->delimitadorCampos;
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
  registro->atualizadoInplace = false;

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
  args->cabecalho = NULL;
  args->cabecalhoIndice = NULL;
  args->registroIndice = NULL;
  args->byteOffPaiRegSplitado = -1;
  args->idCrimeBuscado = -1;
  return args;
}

bool argsApagar(ARGS** args) {
  if (args == NULL || *args == NULL) return false;
  dadosArvoreBinariaApagar(*args);
  dadosCabecalhoApagar(&((*args)->cabecalho));
  char* tipoDado = entradaObterTipoDado((*args)->entrada);
  REGISTRO_INDICE* registroIndice = (*args)->registroIndice;
  indiceRegistroApagar(&registroIndice, tipoDado);

  CABECALHO_INDICE* cabecalhoIndice = (*args)->cabecalhoIndice;
  indiceCabecalhoApagar(&cabecalhoIndice);
  free(*args);
  *args = NULL;
  args = NULL;
  return true;
}

void dadosEncontrar(ARGS* args, void (*ftnPorRegistro)()) {
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

    char* tipoCampo = dadosObterTipoCampo(campo);

    char* valorStr = "";
    int32_t valorInt = -1;
    if (strcmp(tipoCampo, "string") == 0) {
      char delimQuote[] = "\"";
      linhaSplit = strtok(NULL, delimQuote);
      char valorTrunc[TAMANHO_CHAVE_BUSCA+1] = "";
      strcpy(valorTrunc, linhaSplit);
      valorTrunc[TAMANHO_CHAVE_BUSCA] = '\0';

      char valorLimpo[TAMANHO_CHAVE_BUSCA+1] = "";
      char* valorReg = (char*) valorRegistro;
      removerPreenchimento(valorLimpo, valorReg);
      valorLimpo[TAMANHO_CHAVE_BUSCA] = '\0';
      if (strcmp(valorLimpo, valorTrunc) != 0) match = false;

    } else {
      linhaSplit = strtok(NULL, delimEspaco);
      valorInt = (int32_t) atoi(linhaSplit);
      int32_t* valorReg = (int32_t*) valorRegistro;
      if (*valorReg != valorInt) match = false;
    }
  }  

  if (match) {
    args->regEncontradoEmBusca = true;
    ftnPorRegistro(args);
  }
}

bool dadosBuscaSequencialArquivoBinario(ENTRADA* entrada, ARGS* args, void (*ftnPorRegistro)()) {

  dadosLerCabecalhoDoArquivoBinario(args->arquivoDadosBin, args->cabecalho);

  int32_t nroRegArq = args->cabecalho->nroRegArq;
  int64_t byteOffset = args->cabecalho->tamanhoRegistro;

  if (args->cabecalho->status == '0') {
    erroGenerico();
    return false;
  }

  if (args->cabecalho->nroRegArq == 0) {
    erroSemRegistros();
    return false;
  }

  resetLeituraDeArquivo(args->arquivoDadosBin, args->cabecalho->tamanhoRegistro);

  while (nroRegArq--) {
    REGISTRO* registro = dadosRegistroInit();
    if (registro == NULL) continue;
    args->registro = registro;
    
    dadosLerRegistroDoArquivoBinario(args->arquivoDadosBin, registro);
    if (registro->removido == '1') {
      byteOffset += registro->tamanhoRegistro;
      dadosRegistroApagar(&registro);
      continue;
    }

    registro->byteOffset = byteOffset;
    byteOffset += registro->tamanhoRegistro;

    dadosEncontrar(args, ftnPorRegistro);

    dadosRegistroApagar(&registro);
  }
  
  dadosArmazenarCabecalho(args->cabecalho, args->arquivoDadosBin);
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
  args->cabecalho = cabecalho;

  bool indiceArvoreB = entradaObterIndiceEmArvoreB(entrada);
  if (indiceArvoreB) {
    argsInitInformativoParaArvoreB(args);
  }

  resetLeituraDeArquivo(arquivoBinarioDados, cabecalho->tamanhoRegistro);

  while (nroRegArq--) {
    REGISTRO* registro = dadosRegistroInit();
    if (registro == NULL) continue;
    args->registro = registro;
    
    dadosLerRegistroDoArquivoBinario(arquivoBinarioDados, registro);
    if (registro->removido == '1') {
      byteOffset += registro->tamanhoRegistro;
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
  void (*ftnPorRegistro)()
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

  bool removido = dadosObterRemovido(args->registro) == '1' ? true : false;
  if (removido) return;

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


REGISTRO* construirRegistroAInserir(char* linhaDeBusca) {
  REGISTRO* registro = dadosRegistroInit();
  char delimVirgula[] = ",";
  char delimQuote[] = "\"";

  char linhaDeBuscaCp[100] = "";
  strcpy(linhaDeBuscaCp, linhaDeBusca);
  char linhaSplit[300];
  strcpy(linhaSplit, strtok(linhaDeBuscaCp, delimVirgula)); 
  int32_t valorInt;

  enum sequenciaCamposAInserir idCampo = idCrime;
  while (linhaSplit != NULL) {
    if (strcmp(linhaSplit, "\n") == 0) break;
    if (strcmp(linhaSplit, "FIM") == 0) break;
    
    char* nomeCampo = camposIndexadosTipos[idCampo];

    if (strcmp(nomeCampo, "string") == 0) {
      if (strcmp(linhaSplit, "NULO") == 0) memset(linhaSplit, 0, sizeof(linhaSplit));
      if (idCampo == dataCrimeInput) strcpy(registro->dataCrime, linhaSplit);
      if (idCampo == lugarCrimeInput) strcpy(registro->lugarCrime, linhaSplit);
      if (idCampo == descricaoCrimeInput) strcpy(registro->descricaoCrime, linhaSplit);
      if (idCampo == marcaCelularInput) strcpy(registro->marcaCelular, linhaSplit);
    } else {
      valorInt = (int32_t) atoi(linhaSplit);
      if (strcmp(linhaSplit, "NULO") == 0) {
        memset(linhaSplit, 0, sizeof(linhaSplit));
        valorInt = -1;
      }
      if (idCampo == idCrimeInput) registro->idCrime = valorInt;
      if (idCampo == numeroArtigoInput) registro->numeroArtigo = valorInt;
    }
    strcpy(linhaSplit, strtok(NULL, delimVirgula)); 
    idCampo++;
  }
  dadosAtualizarTamanhoRegistro(registro);
  
  return registro;
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
    char* tipoCampo = dadosObterTipoCampo(campo);

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

bool atualizarRegistro(REGISTRO* registro, char* linhaDeAtualizacao, char* campoIndexado) {
  if (registro == NULL) return false;

  char delimEspaco[] = " ";

  char linhaDeAtualizacaoCp[100];
  strcpy(linhaDeAtualizacaoCp, linhaDeAtualizacao);
  char* linhaSplit = strtok(linhaDeAtualizacaoCp, delimEspaco);

  while ((linhaSplit = strtok(NULL, delimEspaco)) != NULL) {
    if (strcmp(linhaSplit, "\n") == 0) break;
    char* campo = "";
    campo = linhaSplit;
    char* tipoCampo = dadosObterTipoCampo(campo);
    void* regValorAtual = dadosObterCampoIndexado(registro, campo);

    char valorStr[100] = "";
    int32_t valorInt = -1;
    if (registro->byteOffset == 29294) {
      printf("AA\n");
    }
    if (strcmp(tipoCampo, "string") == 0) {
      char delimQuote[] = "\"";
      linhaSplit = strtok(NULL, delimQuote);
      char* regValorStr = (char*)regValorAtual;
      if (strcmp(campo, "dataCrime") == 0) {
        strcpy(valorStr, linhaSplit);
      }
      if (strcmp(campo, "marcaCelular") == 0) {
        preencherString(valorStr, linhaSplit, TAMANHO_MARCA_CELULAR, false); 
      }
      if (strcmp(campo, "lugarCrime") == 0 ||
          strcmp(campo, "descricaoCrime") == 0
      ) {
        strcpy(valorStr, linhaSplit);
      }

      strcpy(regValorStr, valorStr);
    } else {
      linhaSplit = strtok(NULL, delimEspaco);
      valorInt = (int32_t) atoi(linhaSplit);
      int32_t* val = (int32_t*) regValorAtual;
      *val = valorInt;
    }
  }

  dadosAtualizarTamanhoRegistro(registro);
  
  return true;
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

    char* tipoCampo = dadosObterTipoCampo(campo);

    char* valorStr = "";
    int32_t valorInt = -1;
    if (strcmp(tipoCampo, "string") == 0) {
      char delimQuote[] = "\"";
      linhaSplit = strtok(NULL, delimQuote);
      char valorTrunc[TAMANHO_CHAVE_BUSCA+1] = "";
      strcpy(valorTrunc, linhaSplit);
      valorTrunc[TAMANHO_CHAVE_BUSCA] = '\0';

      char valorLimpo[TAMANHO_CHAVE_BUSCA+1] = "";
      char* valorReg = (char*) valorRegistro;
      removerPreenchimento(valorLimpo, valorReg);
      valorLimpo[TAMANHO_CHAVE_BUSCA] = '\0';

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

void detectarIdCrimeBuscado(ARGS* args) {
  if (args == NULL) return;

  char delimEspaco[] = " ";
  char* linhaDeBusca = entradaObterLinhaDeBusca(args->entrada);

  char linhaDeBuscaCp[100];
  strcpy(linhaDeBuscaCp, linhaDeBusca);
  char* linhaSplit = strtok(linhaDeBuscaCp, delimEspaco);
  int32_t valorInt = -1;
  
  while ((linhaSplit = strtok(NULL, delimEspaco)) != NULL) {

    if (strcmp(linhaSplit, "idCrime") != 0) continue;
    
    linhaSplit = strtok(NULL, delimEspaco);
    valorInt = (int32_t) atoi(linhaSplit);
    args->idCrimeBuscado = valorInt;
    return;
  }
}

bool dadosBuscarPorCampos(ENTRADA* entrada, void (*ftnPorBusca)) {
  if (entrada == NULL) return false;

  char* campoIndexado = entradaObterCampoIndexado(entrada);
  bool indiceEmArvoreB = entradaObterIndiceEmArvoreB(entrada);
  int nroCamposBuscados = entradaObterNumeroCamposBuscados(entrada);
  
  ARGS* args = argsInit();
  args->entrada = entrada;

  CABECALHO* cabecalho = dadosCabecalhoInit();
  args->cabecalho = cabecalho;

  CABECALHO_INDICE* cabecalhoIndice = indiceCabecalhoInit();
  args->cabecalhoIndice = cabecalhoIndice;

  char* arquivoEntrada = entradaObterArquivoEntrada(entrada);
  FILE* arquivoBinarioDados = fopen(arquivoEntrada, MODO_LEITURA_ARQUIVO);
  if (arquivoBinarioDados == NULL) {
    erroGenerico();
    return false;
  };
  args->arquivoDadosBin = arquivoBinarioDados;

  char* arquivoIndice = entradaObterArquivoIndice(entrada);
  FILE* arquivoBinarioIndice = fopen(arquivoIndice, MODO_LEITURA_ARQUIVO);
  if (arquivoBinarioIndice == NULL) {
    erroGenerico();
    return false;
  };
  args->arquivoIndiceBin = arquivoBinarioIndice;

  bool busca = false;

  char* linhaDeBusca = entradaObterLinhaDeBusca(entrada);
  for (int i = 0; i < nroCamposBuscados; i++) {
    args->regEncontradoEmBusca = false;
    if ((entradaObterFuncionalidade(entrada) == 4) ||
        (entradaObterFuncionalidade(entrada) == 9)    
    ) {
      printf("Resposta para a busca %d\n", i+1);
    }
    bool buscaIndexada = verificarSeCriterioDeBuscaIndexado(linhaDeBusca, campoIndexado);
    if (!buscaIndexada) {
      busca = dadosBuscaSequencialArquivoBinario(entrada, args, ftnPorBusca);
      if (!busca) break;
    } else {
      if (indiceEmArvoreB) {
        detectarIdCrimeBuscado(args);
        busca = indiceBuscaPorIndiceArvoreB(entrada, args, ftnPorBusca);
        if (!busca) break;
      } else 
      busca = indiceBuscaBinariaArquivoBinario(entrada, args, ftnPorBusca);
      if (!busca) break;
    }

    if (!args->regEncontradoEmBusca && (
        (entradaObterFuncionalidade(entrada) == 4) ||
        (entradaObterFuncionalidade(entrada) == 9)
    )) erroSemRegistros();

    linhaDeBusca = entradaProximaLinhaDeBusca(entrada);
  }
  if (busca) dadosArmazenarCabecalho(args->cabecalho, args->arquivoDadosBin);
  fclose(args->arquivoDadosBin);
  fclose(args->arquivoIndiceBin);
  args->registro = NULL;
  args->registroIndice = NULL;
  argsApagar(&args);
  return busca;
}

bool dadosRemoverRegistroLogicamente(ARGS* args) {
  if (args->registro->removido == '1') return true;
  FILE* arquivoDadosBin = args->arquivoDadosBin;
  args->registro->removido = '1';
  args->cabecalho->nroRegRem += 1;
  resetLeituraDeArquivo(arquivoDadosBin, args->registro->byteOffset);
  char removido = '1';
  fwrite(&removido, sizeof(char), 1, arquivoDadosBin);
  int64_t proxBOff = args->registro->byteOffset + args->registro->tamanhoRegistro;
  resetLeituraDeArquivo(arquivoDadosBin, proxBOff);
  return true;
}

bool dadosAppendRegistroNoArquivoBinario(ARGS* args) {
  if (args == NULL) return false;
  resetLeituraDeArquivo(args->arquivoDadosBin, args->cabecalho->proxByteOffset);
  dadosArmazenarRegistro(args->registro, args->arquivoDadosBin);
  args->cabecalho->nroRegArq++;
  args->cabecalho->proxByteOffset += args->registro->tamanhoRegistro;
  return true;
}


bool dadosInserirNovosRegistros(ENTRADA* entrada) {
  if (entrada == NULL) return false;
  
  ARGS* args = argsInit();
  args->entrada = entrada;

  CABECALHO* cabecalho = dadosCabecalhoInit();
  args->cabecalho = cabecalho;

  char* arquivoEntrada = entradaObterArquivoEntrada(entrada);
  FILE* arquivoBinarioDados = fopen(arquivoEntrada, MODO_LEITURA_ARQUIVO);
  if (arquivoBinarioDados == NULL) {
    erroGenerico();
    return false;
  };
  args->arquivoDadosBin = arquivoBinarioDados;
  dadosLerCabecalhoDoArquivoBinario(args->arquivoDadosBin, args->cabecalho);
  int32_t nroRegArq = args->cabecalho->nroRegArq;

  int nroCamposBuscados = entradaObterNumeroCamposBuscados(entrada);
  char* linhaDeBusca = entradaObterLinhaDeBusca(entrada);
  for (int i = 0; i < nroCamposBuscados; i++) {
    REGISTRO* registro = construirRegistroAInserir(linhaDeBusca);
    args->registro = registro;
    dadosAppendRegistroNoArquivoBinario(args);
    linhaDeBusca = entradaProximaLinhaDeBusca(entrada);
    dadosRegistroApagar(&registro);
  }

  dadosArmazenarCabecalho(args->cabecalho, args->arquivoDadosBin);
  fclose(args->arquivoDadosBin);
  argsApagar(&args);
}

bool dadosAtualizarRegistros(ARGS* args) {
  if (args == NULL) return false;

  if (!args->regEncontradoEmBusca) {
    dadosAppendRegistroNoArquivoBinario(args);
    resetLeituraDeArquivo(
      args->arquivoDadosBin, 
      args->registro->byteOffset + args->registro->tamanhoRegistro
    );
    return true;
  }

  REGISTRO* regAtualizado = args->registro;
  REGISTRO* regAnteriorCp = dadosRegistroInit();
  copiarRegistro(regAtualizado, regAnteriorCp);

  int64_t tamAnterior = regAtualizado->tamanhoRegistro;
  atualizarRegistro(
    regAtualizado, 
    entradaObterLinhaDeAtualizacao(args->entrada),
    entradaObterCampoIndexado(args->entrada)
  );

  int64_t byteOffset = regAtualizado->byteOffset;
  FILE* arqDadosBin = args->arquivoDadosBin;
  if (tamAnterior >= regAtualizado->tamanhoRegistro) {
    int dif = tamAnterior - regAtualizado->tamanhoRegistro;
    int tamNovaDescri = strlen(regAtualizado->descricaoCrime) + dif + 1;
    resetLeituraDeArquivo(arqDadosBin, byteOffset);
    char descPreench[150] = "";
    strcpy(descPreench, regAtualizado->descricaoCrime);
    preencherString(descPreench, regAtualizado->descricaoCrime, tamNovaDescri, true);
    regAtualizado->atualizadoInplace = true;
    strcpy(regAtualizado->descricaoCrime, descPreench);
    dadosArmazenarRegistro(regAtualizado, args->arquivoDadosBin);
    dadosRegistroApagar(&regAnteriorCp);
    return true;
  }

  regAnteriorCp->removido = '1';
  resetLeituraDeArquivo(arqDadosBin, byteOffset);
  dadosArmazenarRegistro(regAnteriorCp, args->arquivoDadosBin);
  args->cabecalho->nroRegRem++;

  dadosAppendRegistroNoArquivoBinario(args);
  byteOffset += tamAnterior;
  resetLeituraDeArquivo(arqDadosBin, byteOffset);

  dadosRegistroApagar(&regAnteriorCp);

  return true;
}