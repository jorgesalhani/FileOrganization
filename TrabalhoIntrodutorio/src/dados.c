#include <stdio.h>
#include "dados.h"

/********************
 * ESTRUTURA DE DADOS
 * ******************
*/

struct dados_ {
    char removido;
    int32_t idCrime;
    char* dataCrime;
    int32_t numeroArtigo; 
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

bool stringFixaValida(char* entrada, size_t tamanho) {
    return (sizeof(entrada) <= sizeof(char)*tamanho) ? true : false;
}

bool dadosEntrada4BytesValida(int32_t entrada) {
    return (sizeof(entrada) <= sizeof(int32_t)) ? true : false;
}

bool dadosEntradasValidas(char removido, int32_t idCrime, char dataCrime[10], int32_t numeroArtigo, char marcaCelular[12]) {
    if (
        removidoValido(removido) && stringFixaValida(dataCrime, 10) && 
        stringFixaValida(marcaCelular, 10) && dadosEntrada4BytesValida(idCrime) &&
        dadosEntrada4BytesValida(numeroArtigo)
    ) return true;
    return false;
}


/********************
 * FUNCOES PRINCIPAIS
 * ******************
*/

DADOS* dadosCriar(
    char removido, int32_t idCrime, char* dataCrime, int32_t numeroArtigo, 
    char* marcaCelular, char* lugarCrime, char* descricaoCrime) {
    
    DADOS* dados = (DADOS*) malloc(sizeof(DADOS));
    if (!dadosExiste(dados)) return NULL;
    if (!dadosEntradasValidas(removido, idCrime, dataCrime, numeroArtigo, marcaCelular)) return NULL;

    dados->removido = removido;
    dados->idCrime = idCrime;
    dados->dataCrime = dataCrime;
    dados->numeroArtigo = numeroArtigo;
    dados->marcaCelular = marcaCelular;
    dados->lugarCrime = lugarCrime;
    dados->descricaoCrime = descricaoCrime;
    dados->delimitadorDados = '#';

    return dados;
}

void dadosImprimir(DADOS* dados) {
    if (!dadosExiste(dados)) return;
    printf(
        "removido: %c\nidCrime: %d\ndataCrime: %s\nnumeroArtigo: %d\nmarcaCelular: %s\nlugarCrime: %s\ndescricaoCrime: %s\ndelimitadorDados: %c\n\n",
        dados->removido, dados->idCrime, dados->dataCrime, dados->numeroArtigo, 
        dados->marcaCelular, dados->lugarCrime, dados->descricaoCrime, dados->delimitadorDados
    );
}

bool dadosAtualizarRemovido(DADOS* dados, char removido) {

}

bool dadosAtualizarIdCrime(DADOS* dados, int32_t novoIdCrime) {

}

bool dadosAtualizarDataCrime(DADOS* dados, char* novoDataCrime) {

}

bool dadosAtualizarNumeroArtigo(DADOS* dados, int32_t novoNumeroArtigo) {

}

bool dadosAtualizarMarcaCelular(DADOS* dados, char* novoMarcaCelular) {

}

bool dadosAtualizarLugarCrime(DADOS* dados, char* novoLugarCrime) {

}

bool dadosAtualizarDescricaoCrime(DADOS* dados, char* novoDescricaoCrime) {

}

bool dadosDeletar(DADOS** dados) {
    if (dados == NULL || !dadosExiste(*dados)) return false;
    free(*dados);
    *dados = NULL;
    dados = NULL;
    return true;
}
