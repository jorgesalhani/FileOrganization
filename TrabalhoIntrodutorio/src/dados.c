#include <stdio.h>
#include <string.h>
#include "dados.h"

/********************
 * ESTRUTURA DE DADOS
 * ******************
*/

struct metadados_ {
  int tamanhoLugarCrime;
  int tamanhoDescricaoCrime;
  uint64_t tamanhoRegistro;
};

struct dados_ {
    uint32_t idCrime;
    char dataCrime[TAMANHO_DATA_CRIME];
    uint32_t numeroArtigo; 
    char marcaCelular[TAMANHO_MARCA_CELULAR];
    char* lugarCrime;
    char* descricaoCrime;
    char removido;
    char delimitador;
};

/*********************
 * FUNCOES UTILITARIAS
 * *******************
*/

bool removidoValido(char removido) {
    return (removido != '0' && removido != '1') ? false : true;
}

bool stringFixaValida(char* entrada, size_t tamanho, char delimitador) {
    return (strlen(entrada) <= sizeof(char)*tamanho) ? true : false;
}

bool dadosEntrada4BytesValida(uint32_t entrada) {
    return (sizeof(entrada) <= sizeof(uint32_t)) ? true : false;
}

bool dadosEntradasValidas(char removido, uint32_t idCrime, char* dataCrime, uint32_t numeroArtigo, char* marcaCelular) {
    if (
        removidoValido(removido) && stringFixaValida(dataCrime, TAMANHO_DATA_CRIME, '$') && 
        stringFixaValida(marcaCelular, TAMANHO_MARCA_CELULAR, '$') && dadosEntrada4BytesValida(idCrime) &&
        dadosEntrada4BytesValida(numeroArtigo)
    ) return true;
    return false;
}

void preencherCamposFixos(char* novoCampo, char* campoEntrada, int tamanho) {
    for (int i = 0; i < strlen(campoEntrada); i++) novoCampo[i] = campoEntrada[i];
    for (int i = strlen(campoEntrada); i < tamanho; i++) novoCampo[i] = '$';
}

bool deletarStringsFixas(char* stringFixa) {
    if (stringFixa == NULL) return false;
    free(stringFixa);
    stringFixa = NULL;
}

bool metadadosExiste(METADADOS* metadados) {
  return metadados != NULL ? true : false;
}

/********************
 * FUNCOES PRINCIPAIS
 * ******************
*/

bool dadosExiste(DADOS* dados) {
    return dados != NULL ? true : false;
}

DADOS* dadosCriar(
    uint32_t idCrime, char dataCrime[TAMANHO_DATA_CRIME], 
    uint32_t numeroArtigo, char marcaCelular[TAMANHO_MARCA_CELULAR], 
    char* lugarCrime, char* descricaoCrime, char removido) {
    
    DADOS* dados = (DADOS*) malloc(sizeof(DADOS));
    
    if (!dadosExiste(dados)) return NULL;
    if (!dadosEntradasValidas(removido, idCrime, dataCrime, numeroArtigo, marcaCelular)) {
        dadosDeletar(&dados);
        return NULL;
    }

    dados->removido = removido;
    dados->idCrime = idCrime;
    strcpy(dados->dataCrime, dataCrime);
    dados->numeroArtigo = numeroArtigo;
    strcpy(dados->marcaCelular, marcaCelular);
    
    dados->lugarCrime = malloc(sizeof(char));
    strcpy(dados->lugarCrime, lugarCrime);
  
    dados->descricaoCrime = malloc(sizeof(char));  
    strcpy(dados->descricaoCrime, descricaoCrime);

    dados->delimitador = '#';

    return dados;
}

METADADOS* dadosCriarMetadados() {
  METADADOS* metadados = (METADADOS*) malloc(sizeof(METADADOS));
  if (!metadadosExiste(metadados)) return NULL;
  metadados->tamanhoDescricaoCrime = 0;
  metadados->tamanhoLugarCrime = 0;
  return metadados;
}

void dadosImprimir(DADOS* dados) {
    if (!dadosExiste(dados)) return;
    printf("Removido: %c\n", dadosObterRemovido(dados));
    printf("Id: %d\n", dadosObterIdCrime(dados));
    printf("Data: %s\n", dadosObterDataCrime(dados));
    printf("Numero do Artigo: %d\n", dadosObterNumeroArtigo(dados));
    printf("Marca do Celular: %s\n", dadosObterMarcaCelular(dados));
    printf("Local do Crime: %s\n", dadosObterLugarCrime(dados));
    printf("Descricao: %s\n", dadosObterDescricaoCrime(dados));

}

bool dadosAtualizarRemovido(DADOS* dados, char removido) {
  if (!dadosExiste(dados)) return false;
  dados->removido = removido;
  return true;
}

bool dadosAtualizarIdCrime(DADOS* dados, uint32_t novoIdCrime) {
  if (!dadosExiste(dados)) return false;
  dados->idCrime = novoIdCrime;
  return true;
}

bool dadosAtualizarDataCrime(DADOS* dados, char* novoDataCrime) {
  if (!dadosExiste(dados)) return false;
  strcpy(dados->dataCrime, novoDataCrime);
  if(strlen(novoDataCrime) == 0) {
    for(int i = 0; i < TAMANHO_DATA_CRIME; i++) {
      dados->dataCrime[i] = '$';
    }
  } 
  return true;
}

bool dadosAtualizarNumeroArtigo(DADOS* dados, uint32_t novoNumeroArtigo) {
  if (!dadosExiste(dados)) return false;
  dados->numeroArtigo = novoNumeroArtigo;
  return true;
}

bool dadosAtualizarMarcaCelular(DADOS* dados, char* novoMarcaCelular) {
  if (!dadosExiste(dados)) return false;
  strcpy(dados->marcaCelular, novoMarcaCelular);
  int tam = strlen(novoMarcaCelular);
  for(int i = tam; i < TAMANHO_MARCA_CELULAR; i++) {
    dados->marcaCelular[i] = '$';
  }
  return true;
}

bool dadosAtualizarLugarCrime(DADOS* dados, char* novoLugarCrime, METADADOS* metadados) {
  if (!dadosExiste(dados) || !metadadosExiste(metadados)) return false;
  
  metadados->tamanhoLugarCrime = strlen(novoLugarCrime);
  dados->lugarCrime = realloc(
    dados->lugarCrime, sizeof(char)*strlen(novoLugarCrime) + 1
  );

  for (int i = 0; i < strlen(novoLugarCrime) + 1; i++) {
    (dados->lugarCrime)[i] = novoLugarCrime[i];
  }
  
  return true;
}

bool dadosAtualizarDescricaoCrime(DADOS* dados, char* novoDescricaoCrime, METADADOS* metadados) {
  if (!dadosExiste(dados) || !metadadosExiste(metadados)) return false;
  
  metadados->tamanhoDescricaoCrime = strlen(novoDescricaoCrime);
  dados->descricaoCrime = realloc(
    dados->descricaoCrime, sizeof(char)*strlen(novoDescricaoCrime) + 1
  );
  for (int i = 0; i < strlen(novoDescricaoCrime) + 1; i++) {
    dados->descricaoCrime[i] = novoDescricaoCrime[i];
  }
  return true;
}

char dadosObterRemovido(DADOS* dados) {
    if (!dadosExiste(dados)) return 'X';
    return dados->removido;
}

uint32_t dadosObterIdCrime(DADOS* dados) {
    if (!dadosExiste(dados)) return -1;
    return dados->idCrime;
}

char* dadosObterDataCrime(DADOS* dados) {
    if (!dadosExiste(dados)) return "$$$$$$$$$$";
    return dados->dataCrime;
}

uint32_t dadosObterNumeroArtigo(DADOS* dados) {
    if (!dadosExiste(dados)) return -1;
    return dados->numeroArtigo;
}

char* dadosObterMarcaCelular(DADOS* dados) {
    if (!dadosExiste(dados)) return "$$$$$$$$$$$$";
    return dados->marcaCelular;
}

char* dadosObterLugarCrime(DADOS* dados) {
    if (!dadosExiste(dados)) return "|";
    return dados->lugarCrime;
}

char* dadosObterDescricaoCrime(DADOS* dados) {
    if (!dadosExiste(dados)) return "|";
    return dados->descricaoCrime;
}

char dadosObterDelimitadorRegistro(DADOS* dados) {
    if (!dadosExiste(dados)) return '#';
    return dados->delimitador;
}

bool dadosDeletar(DADOS** dados) {
    if (dados == NULL || !dadosExiste(*dados)) return false;
    free((*dados)->lugarCrime);  
    free((*dados)->descricaoCrime);
    free(*dados);
    *dados = NULL;
    dados = NULL;
    return true;
}

int dadosMetadadosObterTamanhoLugarCrime(METADADOS* metadados) {
  if (!metadadosExiste(metadados)) return -1;
  return metadados->tamanhoLugarCrime;
}

int dadosMetadadosObterTamanhoDescricaoCrime(METADADOS* metadados) {
  if (!metadadosExiste(metadados)) return -1;
  return metadados->tamanhoDescricaoCrime;
}

bool dadosMetadadosDeletar(METADADOS** metadados) {
  if (metadados == NULL || !metadadosExiste(*metadados)) return false;
  free(*metadados);
  *metadados = NULL;
  metadados = NULL;
  return true;
}

int dadosMetadadosObterTamanhoRegistro(DADOS* dados, METADADOS* metadados) {
  if (!dadosExiste(dados) || !metadadosExiste(metadados)) return -1;
  metadados->tamanhoRegistro = 0;
  metadados->tamanhoRegistro += 
    sizeof(dados->idCrime) + sizeof(dados->dataCrime) + 
    sizeof(dados->numeroArtigo) + sizeof(dados->marcaCelular) + 
    sizeof(dados->removido) + sizeof(dados->delimitador);

  metadados->tamanhoRegistro += 
    metadados->tamanhoLugarCrime + metadados->tamanhoDescricaoCrime;

  metadados->tamanhoRegistro += sizeof(char) * (NUMERO_CAMPOS - 1);
  
  return metadados->tamanhoRegistro;
}
