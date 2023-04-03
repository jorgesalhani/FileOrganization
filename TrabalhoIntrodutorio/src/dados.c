#include <stdio.h>
#include <string.h>
#include "dados.h"

/********************
 * ESTRUTURA DE DADOS
 * ******************
*/

struct dados_ {
    char removido;
    uint32_t idCrime;
    char* dataCrime;
    uint32_t numeroArtigo; 
    char* marcaCelular;
    char* lugarCrime;
    char* descricaoCrime;
    char delimitadorDados;
};

/*********************
 * FUNCOES UTILITARIAS
 * *******************
*/

bool dadosExiste(DADOS* dados) {
    return dados != NULL ? true : false;
}

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

DADOS* dadosCriar(
    char removido, uint32_t idCrime, char* dataCrime, uint32_t numeroArtigo, 
    char* marcaCelular, char* lugarCrime, char* descricaoCrime) {
    
    DADOS* dados = (DADOS*) malloc(sizeof(DADOS));
    char* data = (char*) malloc(sizeof(char)*TAMANHO_DATA_CRIME);
    char* celular = (char*) malloc(sizeof(char)*TAMANHO_MARCA_CELULAR);
    
    if (!dadosExiste(dados)) return NULL;
    if (!dadosEntradasValidas(removido, idCrime, dataCrime, numeroArtigo, marcaCelular)) {
        dadosDeletar(&dados);
        return NULL;
    }

    preencherCamposFixos(data, dataCrime, TAMANHO_DATA_CRIME);
    preencherCamposFixos(celular, marcaCelular, TAMANHO_MARCA_CELULAR);

    dados->removido = removido;
    dados->idCrime = idCrime;
    dados->dataCrime = data;
    dados->numeroArtigo = numeroArtigo;
    dados->marcaCelular = celular;
    dados->lugarCrime = lugarCrime;
    dados->descricaoCrime = descricaoCrime;
    dados->delimitadorDados = '#';

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

}

bool dadosAtualizarIdCrime(DADOS* dados, uint32_t novoIdCrime) {

}

bool dadosAtualizarDataCrime(DADOS* dados, char* novoDataCrime) {

}

bool dadosAtualizarNumeroArtigo(DADOS* dados, uint32_t novoNumeroArtigo) {

}

bool dadosAtualizarMarcaCelular(DADOS* dados, char* novoMarcaCelular) {

}

bool dadosAtualizarLugarCrime(DADOS* dados, char* novoLugarCrime) {

}

bool dadosAtualizarDescricaoCrime(DADOS* dados, char* novoDescricaoCrime) {

}

bool dadosDeletar(DADOS** dados) {
    if (dados == NULL || !dadosExiste(*dados)) return false;
    free((*dados)->dataCrime);
    free((*dados)->marcaCelular);
    free(*dados);
    *dados = NULL;
    dados = NULL;
    return true;
}
