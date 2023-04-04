#include <stdio.h>
#include <stdlib.h>
#include "cabecalho.h"
#include "dados.h"
#include "funcoesFornecidas.h"
#include "tabela.h"

int main(void) {

    CABECALHO* cabecalho = cabecalhoCriar(
        '0', 1, 2, 3
    );

    DADOS* dados = dadosCriar(
        '0', 2, "01/02/2082", 1, "celularA", "Lugar Crime", "Descricao Crime"
    );


    TABELA* tabela = tabelaCriar("out.bin");
    tabelaAtualizarCabecalho(tabela, cabecalho);
    tabelaAtualizarDados(tabela, dados, '|');
    tabelaDeletar(&tabela, true);

    cabecalhoDeletar(&cabecalho);
    dadosDeletar(&dados);


    return 0;
}