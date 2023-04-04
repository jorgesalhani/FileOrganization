#include <stdio.h>
#include "cabecalho.h"

/*******************
 * ESTRUTURA DE DADO
 * *****************
*/

struct cabecalho_ {
    char status;
    uint64_t proxByteOffset; 
    uint32_t nroRegArq; 
    uint32_t nroRegRem;
};

/*********************
 * FUNCOES UTILITARIAS
 * *******************
*/

bool statusValido(char status) {
    return (status != '0' && status != '1') ? false : true;
}

bool cabecalhoEntrada4BytesValida(uint32_t entrada) {
    return (sizeof(entrada) <= sizeof(uint32_t)) ? true : false;
}

bool cabecalhoEntrada8BytesValida(uint64_t entrada) {
    return (sizeof(entrada) <= sizeof(uint64_t)) ? true : false;
}

bool cabecalhoEntradasValidas(char status, uint64_t proxByteOffset, uint32_t nroRegArq, uint32_t nroRegRem) {
    if (
        statusValido(status) && cabecalhoEntrada8BytesValida(proxByteOffset) && 
        cabecalhoEntrada4BytesValida(nroRegArq) && cabecalhoEntrada4BytesValida(nroRegRem)
    ) return true;
    return false;
}

/********************
 * FUNCOES PRINCIPAIS
 * ******************
*/

bool cabecalhoExiste(CABECALHO* cabecalho) {
    return cabecalho != NULL ? true : false;
}

CABECALHO* cabecalhoCriar(char status, uint64_t proxByteOffset, uint32_t nroRegArq, uint32_t nroRegRem) {
    CABECALHO* cabecalho = (CABECALHO*) malloc(sizeof(CABECALHO));
    if (!cabecalhoExiste(cabecalho)) return NULL;
    if (!cabecalhoEntradasValidas(status, proxByteOffset, nroRegArq, nroRegRem)) return NULL;

    cabecalho->status = status;
    cabecalho->proxByteOffset = proxByteOffset;
    cabecalho->nroRegArq = nroRegArq;
    cabecalho->nroRegRem = nroRegRem;
    return cabecalho;
}

void cabecalhoImprimir(CABECALHO* cabecalho) {
    if (!cabecalhoExiste(cabecalho)) return;
    printf(
        "status: %c\nproxByteOffset: %ld\nnroRegArq: %d\nnroRegRem: %d\n\n", 
        cabecalho->status, cabecalho->proxByteOffset, cabecalho->nroRegArq, cabecalho->nroRegRem
    );
}

bool cabecalhoAtualizarStatus(CABECALHO* cabecalho, char novoStatus) {
    if (!cabecalhoExiste(cabecalho) || !statusValido(novoStatus)) return false;
    cabecalho->status = novoStatus;
    return true;
}

bool cabecalhoAtualizarProxByteOffset(CABECALHO* cabecalho, uint64_t novoProxByteOffset) {
    if (!cabecalhoExiste(cabecalho) || !cabecalhoEntrada8BytesValida(novoProxByteOffset)) return false;
    cabecalho->proxByteOffset = novoProxByteOffset;
    return true;
}

bool cabecalhoAtualizarNroRegArq(CABECALHO* cabecalho, uint32_t novoNroRegArq) {
    if (!cabecalhoExiste(cabecalho) || !cabecalhoEntrada4BytesValida(novoNroRegArq)) return false;
    cabecalho->nroRegArq = novoNroRegArq;
    return true;
}

bool cabecalhoAtualizarNroRegRem(CABECALHO* cabecalho, uint32_t novoNroRegRem) {
    if (!cabecalhoExiste(cabecalho) || !cabecalhoEntrada4BytesValida(novoNroRegRem)) return false;
    cabecalho->nroRegRem = novoNroRegRem;
    return true;
}

char cabecalhoObterStatus(CABECALHO* cabecalho) {
    if (!cabecalhoExiste(cabecalho)) return 'X';
    return cabecalho->status;
}

uint64_t cabecalhoObterProxByteOffset(CABECALHO* cabecalho) {
    if (!cabecalhoExiste(cabecalho)) return 0;
    return cabecalho->proxByteOffset;
}

uint32_t cabecalhoObterNroRegArq(CABECALHO* cabecalho) {
    if (!cabecalhoExiste(cabecalho)) return 0;
    return cabecalho->nroRegArq;
}

uint32_t cabecalhoObterNroRegRem(CABECALHO* cabecalho) {
    if (!cabecalhoExiste(cabecalho)) return 0;
    return cabecalho->nroRegRem;
}



bool cabecalhoDeletar(CABECALHO** cabecalho) {
    if (cabecalho == NULL || !cabecalhoExiste(*cabecalho)) return false;
    free(*cabecalho);
    *cabecalho = NULL;
    cabecalho = NULL;
    return true;
}
