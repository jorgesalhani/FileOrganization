#include "entrada.h"

struct entrada_ {
  int funcionalidade;
  char arquivoEntrada[100];
  char arquivoSaida[100];
  char arquivoIndice[100];
  char campoIndexado[50];
  char tipoDado[50];
  int numeroCamposBuscados;
  char** linhasBusca;
  char** linhasAtualizacoes;
  int proxLinhaDeBusca;
  bool indiceEmArvoreB;
};

bool strVaziaEntrada(char* str) {
  if(strcmp(str, "") == 0) {
    strcpy(str, STR_VAZIA);
    return true;
  }
  return false;
}

char* dadosObterTipoCampo(char* campoIndexado) {
  enum camposIndexados campo_i = 0;
  char* nomeCampo = camposIndexadosTipos[campo_i];
  for (campo_i = 0; campo_i < ENUM_FIM; ++campo_i) {
    if (strcmp(campoIndexado, camposIndexadosNomes[campo_i]) == 0) {
      nomeCampo = camposIndexadosTipos[campo_i];
    }
  }
  return nomeCampo;
}


ENTRADA* lerEntradas() {
  ENTRADA* entrada = (ENTRADA*) malloc(sizeof(ENTRADA));
  if (entrada == NULL) return NULL;
  
  scanf("%d", &entrada->funcionalidade);
  
  entrada->numeroCamposBuscados = 0;
  entrada->linhasBusca = NULL;
  entrada->linhasAtualizacoes = NULL;
  entrada->indiceEmArvoreB = false;

  if (entrada->funcionalidade == 8 ||
    entrada->funcionalidade == 9 ||
    entrada->funcionalidade == 10
  ) {
    entrada->indiceEmArvoreB = true;
  }
  
  if (entrada->funcionalidade == 1) {
    scanf("%s", entrada->arquivoEntrada);
    scanf("%s", entrada->arquivoSaida);
    return entrada;
  }

  if (entrada->funcionalidade == 2) {
    scanf("%s", entrada->arquivoEntrada);
    return entrada;
  }

  if (entrada->funcionalidade == 3 ||
      entrada->funcionalidade == 8
    ) {
    scanf("%s", entrada->arquivoEntrada);
    scanf("%s", entrada->campoIndexado);
    scanf("%s", entrada->tipoDado);
    scanf("%s", entrada->arquivoIndice);
    return entrada;
  }

  if (entrada->funcionalidade == 4 ||
      entrada->funcionalidade == 5 ||
      entrada->funcionalidade == 9
  ) {
    scanf("%s", entrada->arquivoEntrada);
    scanf("%s", entrada->campoIndexado);
    scanf("%s", entrada->tipoDado);
    scanf("%s", entrada->arquivoIndice);
    scanf("%d", &entrada->numeroCamposBuscados);

    int numLinhas = entrada->numeroCamposBuscados;
    entrada->linhasBusca = (char**) malloc(sizeof(char*)*numLinhas);
    entrada->proxLinhaDeBusca = 0;

    size_t bufLinha1 = 5;
    char* linha1Buff = NULL;
    scanf("%s", linha1Buff);

    size_t tam = 250;
    for (int i = 0; i < entrada->numeroCamposBuscados; i++) {
      entrada->linhasBusca[i] = (char*) malloc(sizeof(char)*250);
      getline(&(entrada->linhasBusca)[i], &tam, stdin);
    }

    return entrada;
  }

  if (entrada->funcionalidade == 6) {
    scanf("%s", entrada->arquivoEntrada);
    scanf("%s", entrada->campoIndexado);
    scanf("%s", entrada->tipoDado);
    scanf("%s", entrada->arquivoIndice);
    scanf("%d", &entrada->numeroCamposBuscados);

    int numLinhas = entrada->numeroCamposBuscados;
    entrada->linhasBusca = (char**) malloc(sizeof(char*)*numLinhas);
    entrada->proxLinhaDeBusca = 0;

    size_t bufLinha1 = 5;
    char* linha1Buff = NULL;
    scanf("%s ", linha1Buff);


    size_t tam = 300;
    for (int i = 0; i < entrada->numeroCamposBuscados; i++) {
      entrada->linhasBusca[i] = (char*) malloc(sizeof(char)*tam);
      char valor[300] = "";
      char token[60] = "";
      scan_quote_string(token);
      strVaziaEntrada(token);
      strcat(valor, token);
      strcat(valor, ",");
      memset(token, 0, sizeof(token));

      scan_quote_string(token);
      strVaziaEntrada(token);
      strcat(valor, token);
      strcat(valor, ",");
      memset(token, 0, sizeof(token));

      scan_quote_string(token);
      strVaziaEntrada(token);
      strcat(valor, token);
      strcat(valor, ",");
      memset(token, 0, sizeof(token));

      scan_quote_string(token);
      strVaziaEntrada(token);
      strcat(valor, token);
      strcat(valor, ",");
      memset(token, 0, sizeof(token));

      scan_quote_string(token);
      strVaziaEntrada(token);
      strcat(valor, token);
      strcat(valor, ",");
      memset(token, 0, sizeof(token));

      scan_quote_string(token);
      strVaziaEntrada(token);
      strcat(valor, token);
      strcat(valor, ",FIM,");
      memset(token, 0, sizeof(token));

      strcpy(entrada->linhasBusca[i], valor);
    }

    return entrada;
  }

  if (entrada->funcionalidade == 7) { 
    scanf("%s", entrada->arquivoEntrada);
    scanf("%s", entrada->campoIndexado);
    scanf("%s", entrada->tipoDado);
    scanf("%s", entrada->arquivoIndice);
    scanf("%d", &entrada->numeroCamposBuscados);

    int numLinhas = entrada->numeroCamposBuscados;
    entrada->linhasBusca = (char**) malloc(sizeof(char*)*numLinhas);
    entrada->linhasAtualizacoes = (char**) malloc(sizeof(char*)*numLinhas);
    entrada->proxLinhaDeBusca = 0;

    size_t tamBuff = 5;
    char* linha1Buff = NULL;
    scanf("%s ", linha1Buff);
    linha1Buff = (char*) malloc(sizeof(char)*tamBuff);
    
    size_t tam = 250;
    for (int i = 0; i < entrada->numeroCamposBuscados; i++) {
      entrada->linhasBusca[i] = (char*) malloc(sizeof(char)*250);
      getline(&(entrada->linhasBusca)[i], &tam, stdin);

      entrada->linhasAtualizacoes[i] = (char*) malloc(sizeof(char)*250);
      getline(&(entrada->linhasAtualizacoes)[i], &tam, stdin);

      getline(&(linha1Buff), &tamBuff, stdin);
    }

    free(linha1Buff);
    return entrada;
  }
}

bool stringApagar(char* campo) {
  if (campo == NULL) return false;
  free(campo);
  campo = NULL;
  return true;
}


bool entradaApagar(ENTRADA** entrada) {
  if (entrada == NULL || *entrada == NULL) return false;

  bool apresentaAtualizacoes = (*entrada)->linhasAtualizacoes == NULL ? false : true;
  if ((*entrada)->numeroCamposBuscados != 0) {
    for (int i = 0; i < (*entrada)->numeroCamposBuscados; i++) {
      stringApagar((*entrada)->linhasBusca[i]);
      if (apresentaAtualizacoes) {
        stringApagar((*entrada)->linhasAtualizacoes[i]);
      }
    }
    free((*entrada)->linhasBusca);
    (*entrada)->linhasBusca = NULL;
    if (apresentaAtualizacoes) {
      free((*entrada)->linhasAtualizacoes);
      (*entrada)->linhasAtualizacoes = NULL;
    }
  }
  free(*entrada);
  *entrada = NULL;
  entrada = NULL;
  return true;
}

int entradaObterFuncionalidade(ENTRADA* entrada) {
  if (entrada == NULL) return -1;
  return entrada->funcionalidade;
} 

char* entradaObterArquivoEntrada(ENTRADA* entrada) {
  if (entrada == NULL) return NULL;
  return entrada->arquivoEntrada;
} 

char* entradaObterArquivoSaida(ENTRADA* entrada) {
  if (entrada == NULL) return NULL;
  return entrada->arquivoSaida;
} 

char* entradaObterArquivoIndice(ENTRADA* entrada) {
  if (entrada == NULL) return NULL;
  return entrada->arquivoIndice;
} 
char* entradaObterCampoIndexado(ENTRADA* entrada) {
  if (entrada == NULL) return NULL;
  return entrada->campoIndexado;
} 

char* entradaObterTipoDado(ENTRADA* entrada) {
  if (entrada == NULL) return NULL;
  return entrada->tipoDado;
} 

int entradaObterNumeroCamposBuscados(ENTRADA* entrada) {
  if (entrada == NULL) return -1;
  return entrada->numeroCamposBuscados;
} 

char** entradaObterLinhasBusca(ENTRADA* entrada) {
  if (entrada == NULL) return NULL;
  return entrada->linhasBusca;
} 

char* entradaProximaLinhaDeBusca(ENTRADA* entrada) {
  if (entrada == NULL) return NULL;
  entrada->proxLinhaDeBusca++;
  if (entrada->numeroCamposBuscados <= entrada->proxLinhaDeBusca) {
    return NULL;
  }
  return entrada->linhasBusca[entrada->proxLinhaDeBusca];
} 

char* entradaObterLinhaDeBusca(ENTRADA* entrada) {
  if (entrada == NULL) return NULL;
  return entrada->linhasBusca[entrada->proxLinhaDeBusca];
}

char* entradaObterLinhaDeAtualizacao(ENTRADA* entrada) {
  if (entrada == NULL) return NULL;
  return entrada->linhasAtualizacoes[entrada->proxLinhaDeBusca];
} 

bool entradaObterIndiceEmArvoreB(ENTRADA* entrada) {
  if (entrada == NULL) return NULL;
  return entrada->indiceEmArvoreB;
} 


