#ifndef TABELA_H
    #define TABELA_H

    #include "cabecalho.h"
    #include "dados.h"

    typedef struct tabela_ TABELA;

    TABELA* tabelaCriar(char* nome);
    bool tabelaAtualizarCabecalho(TABELA* tabela, CABECALHO* cabecalho);
    bool tabelaAtualizarDados(TABELA* tabela, DADOS* dados, char delimitadorCampos);
    char* tabelaObterNomeArquivo(TABELA* tabela);
    bool tabelaFecharArquivo(TABELA* tabela);
    bool tabelaDeletar(TABELA** tabela, bool manterArquivo);

#endif