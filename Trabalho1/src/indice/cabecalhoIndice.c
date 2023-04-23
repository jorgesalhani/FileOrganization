#include <stdlib.h>
#include "cabecalhoIndice.h"

/*******************
 * ESTRUTURA DE DADO
 * *****************
*/

struct cabecalho_indice_ {
    char status;
};

/*********************
 * FUNCOES UTILITARIAS
 * *******************
*/

bool cabecalhoIndiceStatusValido(char status) {
    return status == '0' || status == '1' ? true : false;
}

/********************
 * FUNCOES PRINCIPAIS
 * ******************
*/

bool cabecalhoIndiceExiste(CABECALHO_INDICE* cabecalhoIndice) {
    return cabecalhoIndice != NULL ? true : false;
}

CABECALHO_INDICE* cabecalhoIndiceCriar(char status) {
    CABECALHO_INDICE* cabecalhoIndice = (CABECALHO_INDICE*) malloc(sizeof(CABECALHO_INDICE));
    if (!cabecalhoIndiceExiste(cabecalhoIndice)) return NULL;
    if (!cabecalhoIndiceStatusValido(status)) return NULL;

    cabecalhoIndice->status = status;
    return cabecalhoIndice;
}

bool cabecalhoIndiceAtualizarStatus(CABECALHO_INDICE* cabecalhoIndice, char novoStatus) {
    if (!cabecalhoIndiceExiste(cabecalhoIndice) || !cabecalhoIndiceStatusValido(novoStatus)) return false;
    cabecalhoIndice->status = novoStatus;
    return true;
}

char cabecalhoIndiceObterStatus(CABECALHO_INDICE* cabecalhoIndice) {
    if (!cabecalhoIndiceExiste(cabecalhoIndice)) return '-';
    return cabecalhoIndice->status;
}

bool cabecalhoIndiceDeletar(CABECALHO_INDICE** cabecalhoIndice) {
    if (cabecalhoIndice == NULL || !cabecalhoIndiceExiste(*cabecalhoIndice)) return false;
    free(*cabecalhoIndice);
    *cabecalhoIndice = NULL;
    cabecalhoIndice = NULL;
    return true;
}
