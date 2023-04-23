#ifndef TABELA_H
    #define TABELA_H

    #include "cabecalho.h"
    #include "dados.h"

    typedef struct tabela_ TABELA;

    /**
     * @brief Alocacao de memoria para armazenar e manipular arquivo binario
     * @param char* nome. Nome do arquivo a ser criado. Formato de saidoa: <nome>.bin
     * @param char* modoAberturaArquivo. Modo de abertura do arquivo. Padrao esperado: "wb+" ou "rb+"
     * @return TABELA* Ponteiro para o TAD tabela
    */
    TABELA* tabelaCriar(char* nome, char* modoAberturaArquivo);

    /**
     * @brief Atualizar cabecalho do arquivo binario presente no TAD tabela
     * @param TABELA* tabela. Ponteiro para o TAD tabela criado
     * @param CABECALHO* cabecalho. Ponteiro para o TAD cabecalho criado e atualizado
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool tabelaAtualizarCabecalho(TABELA* tabela, CABECALHO* cabecalho);

    /**
     * @brief Atualizar dados do arquivo binario presente no TAD tabela
     * @param TABELA* tabela. Ponteiro para o TAD tabela criado
     * @param DADOS* dados. Ponteiro para o TAD dados criado e atualizado
     * @param char delimitadorCampos. Delimitador de cada campo do registro de dados. Padrao: "|"
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool tabelaAtualizarDados(TABELA* tabela, DADOS* dados, METADADOS* metadados, char delimitadorCampos);

    /**
     * @brief Obter nome do arquivo aberto e armazenado no TAD tabela
     * @param TABELA* tabela. Ponteiro para o TAD tabela criado
     * @return char* nome do arquivo aberto
    */
    char* tabelaObterNomeArquivo(TABELA* tabela);

    /**
     * @brief Fechar arquivo aberto e armazenado no TAD tabela, sem desalocacao de memoria
     * @param TABELA* tabela. Ponteiro para o TAD tabela criado
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool tabelaFecharArquivo(TABELA* tabela);

    /**
     * @brief Fechar arquivo e desalocar memoria do TAD tabela
     * @param TABELA* tabela. Ponteiro para o TAD tabela criado
     * @param bool manterArquivo. Verificador para manter (true) ou excluir (false) arquivo binario criado
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool tabelaDeletar(TABELA** tabela, bool manterArquivo);

    /**
     * @brief Verificar se alocacao de memoria para TAD tabela foi bem sucedido
     * @param TABELA* tabela. Ponteiro para o TAD tabela criado
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool tabelaExiste(TABELA* tabela);

    /**
     * @brief Funcionalidade 1. Implementacao de funcao analoga ao CREATE TABLE (SQL)
     * @param char* nomeEntrada. Nome do arquivo de entrada. Formato esperado: <nomeEntrada>.csv
     * @param char* nomeSaida. Nome do arquivo de said. Formato esperado: <nomeSaida>.bin
     * @return TABELA*. Ponteiro para o TAD tabela criado
    */
    TABELA* tabelaCriarBinario(char* nomeEntrada, char* nomeSaida);

    /**
     * @brief Funcionalidade 2. Implementacao de funcao analoga ao SELECT (SQL)
     * @param char* entrada. Nome do arquivo de entrada. Formato esperado: <entrada>.bin
     * @return TABELA*. Ponteiro para o TAD tabela criado
    */
    TABELA* tabelaLerImprimirBinario(char* entrada);

    CABECALHO* tabelaLerArmazenarCabecalho(TABELA* tabela);
    
    DADOS* tabelaLerArmazenarDado(TABELA* tabela);

    METADADOS* tabelaLerArmazenarMetadado(DADOS* dados);

    bool modoAbrirArquivoValido(char* modoAberturaArquivo);
    

#endif