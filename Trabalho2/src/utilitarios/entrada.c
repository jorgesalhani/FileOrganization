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
  int proxLinhaDeBusca;
};

bool strVaziaEntrada(char* str) {
  if(strcmp(str, "") == 0) {
    strcpy(str, STR_VAZIA);
    return true;
  }
  return false;
}

ENTRADA* lerEntradas() {
  ENTRADA* entrada = (ENTRADA*) malloc(sizeof(ENTRADA));
  if (entrada == NULL) return NULL;
  
  entrada->numeroCamposBuscados = 0;

  scanf("%d", &entrada->funcionalidade);
  
  if (entrada->funcionalidade == 1) {
    scanf("%s", entrada->arquivoEntrada);
    scanf("%s", entrada->arquivoSaida);
    return entrada;
  }

  if (entrada->funcionalidade == 2) {
    scanf("%s", entrada->arquivoEntrada);
    return entrada;
  }

  if (entrada->funcionalidade == 3) {
    scanf("%s", entrada->arquivoEntrada);
    scanf("%s", entrada->campoIndexado);
    scanf("%s", entrada->tipoDado);
    scanf("%s", entrada->arquivoIndice);
    return entrada;
  }

  if (entrada->funcionalidade == 4 ||
      entrada->funcionalidade == 5
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
}

bool entradaApagar(ENTRADA** entrada) {
  if (entrada == NULL || *entrada == NULL) return false;

  if ((*entrada)->numeroCamposBuscados != 0) {
    for (int i = 0; i < (*entrada)->numeroCamposBuscados; i++) {
      free(((*entrada)->linhasBusca)[i]);
      (*entrada)->linhasBusca[i] = NULL;
    }
    free((*entrada)->linhasBusca);
    (*entrada)->linhasBusca = NULL;
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


