#include <stdio.h>
#include <string.h>
#include "dados.h"

/********************
 * ESTRUTURA DE DADOS
 * ******************
*/

struct dados_ {
    uint32_t idCrime;
    char dataCrime[TAMANHO_DATA_CRIME];
    uint32_t numeroArtigo; 
    char marcaCelular[TAMANHO_MARCA_CELULAR];
    char* lugarCrime;
    char* descricaoCrime;
    char removido;
};

/*********************
 * FUNCOES UTILITARIAS
 * *******************
*/

bool removidoValido(char removido) {
    return (removido != '0' && removido != '1') ? false : true;
}

int tamanhoString(char* str) {
    int count; 
    for (count = 0; str[count] != '\0'; ++count);
    return count; 
}

bool stringFixaValida(char* entrada, size_t tamanho, char delimitador) {
    return (tamanhoString(entrada) <= sizeof(char)*tamanho) ? true : false;
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
    for (int i = 0; i < tamanhoString(campoEntrada); i++) novoCampo[i] = campoEntrada[i];
    for (int i = tamanhoString(campoEntrada); i < tamanho; i++) novoCampo[i] = '$';
}

bool deletarStringsFixas(char* stringFixa) {
    if (stringFixa == NULL) return false;
    free(stringFixa);
    stringFixa = NULL;
}

/********************
 * FUNCOES PRINCIPAIS
 * ******************
*/

bool dadosExiste(DADOS* dados) {
    return dados != NULL ? true : false;
}

DADOS* dadosCriar(
    uint32_t idCrime, char dataCrime[TAMANHO_DATA_CRIME], uint32_t numeroArtigo, char marcaCelular[TAMANHO_MARCA_CELULAR], 
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
    dados->lugarCrime = lugarCrime;
    dados->descricaoCrime = descricaoCrime;

    return dados;
}

void dadosImprimir(DADOS* dados) {
    if (!dadosExiste(dados)) return;


    // printf(
    //     "removido: %c\nidCrime: %d\ndataCrime: %s\nnumeroArtigo: %d\nmarcaCelular: %s\nlugarCrime: %s\ndescricaoCrime: %s\ndelimitadorDados: %c\n\n",
    //     dados->removido, dados->idCrime, dataParaImpressao, dados->numeroArtigo, 
    //     celularParaImpressao, dados->lugarCrime, dados->descricaoCrime, dados->delimitadorDados
    // );

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
  dados->dataCrime = novoDataCrime;
  return true;
}

bool dadosAtualizarNumeroArtigo(DADOS* dados, uint32_t novoNumeroArtigo) {
  if (!dadosExiste(dados)) return false;
  dados->numeroArtigo = novoNumeroArtigo;
  return true;
}

bool dadosAtualizarMarcaCelular(DADOS* dados, char* novoMarcaCelular) {
  if (!dadosExiste(dados)) return false;
  dados->marcaCelular = novoMarcaCelular;
  return true;
}

bool dadosAtualizarLugarCrime(DADOS* dados, char* novoLugarCrime) {
  if (!dadosExiste(dados)) return false;
  dados->lugarCrime = novoLugarCrime;
  return true;
}

bool dadosAtualizarDescricaoCrime(DADOS* dados, char* novoDescricaoCrime) {
  if (!dadosExiste(dados)) return false;
  dados->descricaoCrime = novoDescricaoCrime;
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

bool dadosDeletar(DADOS** dados) {
    if (dados == NULL || !dadosExiste(*dados)) return false;
    free(*dados);
    *dados = NULL;
    dados = NULL;
    return true;
}
