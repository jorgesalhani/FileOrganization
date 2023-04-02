#include <stdio.h>
#include <stdlib.h>
#include "cabecalho.h"
#include "dados.h"
#include "funcoesFornecidas.h"

int main(void) {

    CABECALHO* cabecalho = cabecalhoCriar(
        '0', 0, 0, 0
    );

    DADOS* dados = dadosCriar(
        '0', 0, "01/02/2082", 1, "celularAA1aa", "Lugar Crime", "Descricao Crime"
    );

    cabecalhoImprimir(cabecalho);
    dadosImprimir(dados);

    cabecalhoDeletar(&cabecalho);
    dadosDeletar(&dados);


    return 0;
}