/**
 * **********************************************************************
 * @public Codigo desenvolvido para Disciplina de Organizacao de Arquivos
 * 
 * @author Henrique Garcia Gomes do Ó
 *         no USP: 10734331
 * 
 * @author Jorge Augusto Salgado Salhani
 *         no USP: 8927418
 *  
 * @date Abril, 2023
 * **********************************************************************
 * 
 * @brief Obter dados de um arquivo .csv de entrada e gerar um
 *        arquivo binario com os mesmos
 * 
 * @details A aplicacao usa as TADs
 *          DADOS: leitura e armazenamento de dados presentes no arquivo .csv
 *          CABECALHO: leitura e armazenamento de metadados do arquivo .csv
 *          TABELA: escrita e atualizacao do arquivo de saida .bin
 *          FUNCOESFORNECIDAS: manipulacao e tratamento de entrada e saida
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cabecalho.h"
#include "dados.h"
#include "funcoesFornecidas.h"
#include "tabela.h"

int main(void) {

    CABECALHO* cabecalho = cabecalhoCriar(
        '0', 1, 2, 3
    );

    DADOS* dados = dadosCriar(
        2, "01/02/2082", 1, "celularAAAB", "Lugar Crime", "Descricao Crime", '0'
    );


    TABELA* tabela = tabelaCriar("out.bin");
    tabelaAtualizarCabecalho(tabela, cabecalho);
    tabelaAtualizarDados(tabela, dados, '|', '#');
    tabelaDeletar(&tabela, true);

    cabecalhoDeletar(&cabecalho);
    dadosDeletar(&dados);

    binarioNaTela("out.bin");


    return 0;
}