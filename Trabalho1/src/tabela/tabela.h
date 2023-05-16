#ifndef TABELA_H
    #define TABELA_H

    #include "cabecalho.h"
    #include "dados.h"
    #include "../indice/indice.h"

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

    /**
     * @brief Ler e armazenar registro de cabecalho do arquivo binario de dados (ex: binario1.bin)
     * @param TABELA* tabela. Ponteiro para o TAD tabela criado
     * @return CABECALHO*. Ponteiro para o TAD cabecalho criado
    */
    CABECALHO* tabelaLerArmazenarCabecalho(TABELA* tabela);
    
    /**
     * @brief Ler e armazenar proximo registro de dados encontrado no arquivo binario de dados (ex: binario1.bin)
     * @param TABELA* tabela. Ponteiro para o TAD tabela criado
     * @return DADOS*. Ponteiro para o TAD dados criado
    */
    DADOS* tabelaLerArmazenarDado(TABELA* tabela);

    /**
     * @brief Armazenar metadados (ex: tamanho de campos de tamanho variavel) correspondentes ao conjunto de dados informado
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @return METADADOS*. Ponteiro para o TAD metadados criado
    */
    METADADOS* tabelaLerArmazenarMetadado(DADOS* dados);

    /**
     * @brief Verificar validade do modo de abertura do arquivo.
     * @param char* modoAberturaArquivo. Modo de abertura escolhido. Formato esperado: "rb", "wb+"
     * @return bool. true: caso valido. false: caso contrario
    */
    bool modoAbrirArquivoValido(char* modoAberturaArquivo);

    /**
     * @brief Funcionalidade 4. Implementacao de funcao analoga ao SELECT * FROM _ (SQL)
     * @param char* nomeArquivoEntrada. Nome do arquivo binario de dados de entrada. Formato esperado: <nomeArquivoEntrada>.bin
     * @param char* campoIndexado. Nome do campo indexado de busca. Para que a busca seja otimizada, a funcionalidade 2. deve ser chamada anteriormente com este mesmo campoIndexado.
     * @param char* tipoDado. Tipo de dado do atributo utilizado como chave de busca. Formatos esperados: string ou inteiro
     * @param char* nomeArquivoIndice. Nome do arquivo binario de indice criado sobre o campo campoIndexado. Formato esperado: <nomeArquivoIndice>.bin
     * @param int numeroCamposBuscados. Quantidade de buscas que devem ser realizadas. Cada busca deve ser especificada em linhas separadas
     * @return TABELA* tabela. Ponteiro para o TAD tabela criado
    */
    TABELA* tabelaLerImprimirBusca(
        char* nomeArquivoEntrada, char* campoIndexado, char* tipoDado, 
        char* nomeArquivoIndice, int numeroCamposBuscados
    );

    /**
     * @brief Reposicionar ponteiro de leitura para o comeco do arquivo binario de dados (SEEK_SET)
     * @param TABELA* tabela. Ponteiro par o TAD tabela criado
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool tabelaResetLeituraArquivoBinario(TABELA* tabela, int64_t byteOffset);
    

    TABELA* tabelaLerAtualizar(
        char* nomeArquivoEntrada, char* campoIndexado, char* tipoDado, 
        char* nomeArquivoIndice, int numeroAtualizacoes
    );

    METADADOS* tabelaLerArmazenarMetadado(DADOS* dados);

    bool tabelaAtualizarDadoComoRemovido(TABELA* tabela, int64_t byteOffset);

    bool tabelaEscreverChar(TABELA* tabela, char* valor);

#endif