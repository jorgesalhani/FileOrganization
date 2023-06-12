#ifndef INDICE_H
    #define INDICE_H

    #define TAMANHO_CHAVE_BUSCA 12

    #include "dadosIndice.h"
    #include "cabecalhoIndice.h"
    #include "../utilitarios/funcoesFornecidas.h"
    #include "../tabela/tabela.h"
    
    typedef struct indice_ INDICE;

    /**
     * @brief Criacao e alocacao de memoria para arquivo de indice linear
     * @param char* nomeArquivoIndice. Nome do arquivo binario de indice criado ou lido. Formato esperado: <nomeArquivoIndice>.bin
     * @param char* modoAberturaArquivo. Modo de abertura do arquivo. Valores esperados: "rb", "wb+"
     * @return INDICE*. Ponteiro para o TAD indice criado
    */
    INDICE* indiceCriar(char* nomeArquivoIndice, char* modoAberturaArquivo);

    /**
     * @brief Verificar se o arquivo de indice foi devidamente criado e alocado em memoria
     * @param INDICE* indice. Ponteiro para o TAD indice criado
     * @return bool. true: caso indice alocaco corretamente. false: caso contrario
    */
    bool indiceExiste(INDICE* indice);

    /**
     * @brief Escrever cabecalho no arquivo binario de indice
     * @param INDICE* indice. Ponteiro para o TAD indice criado
     * @param CABECALHO_INDICE* cabecalhoIndice. Ponteiro para o TAD cabecalho_indice criado
     * @return true: caso bem-sucedido. false: caso contrario
    */
    bool indiceAtualizarCabecalho(INDICE* indice, CABECALHO_INDICE* cabecalhoIndice);

    /**
     * @brief Escrever registro de dados do indice com tipo inteiro no arquivo de indices lineares criado
     * @param INDICE* indice. Ponteiro para o TAD indice criado
     * @param DADOS_INDICE_INTEIRO* dadosIndiceInteiro. Ponteiro para o TAD dados_indice_inteiro criado
     * @return: true: caso bem-sucedido. false: caso contrario
    */
    bool indiceInteiroAtualizarDados(INDICE* indice, DADOS_INDICE_INTEIRO* dadosIndiceInteiro);

    /**
     * @brief Escrever registro de dados do indice com tipo string no arquivo de indices lineares criado
     * @param INDICE* indice. Ponteiro para o TAD indice criado
     * @param DADOS_INDICE_STRING* dadosIndiceString. Ponteiro para o TAD dados_indice_string criado
     * @return: true: caso bem-sucedido. false: caso contrario
    */
    bool indiceStringAtualizarDados(INDICE* indice, DADOS_INDICE_STRING* dadosIndiceString);

    /**
     * @brief Obter nome do arquivo de indice aberto em modo leitura ou escrita
     * @param INDICE* indice. Ponteiro para o TAD indice criado
     * @return char*. Nome do arquivo binario de indices lineares aberto
    */
    char* indiceObterNomeArquivo(INDICE* indice);

    /**
     * @brief Fechar arquivo de indices lineares aberto em modo leitura ou escrita
     * @param INDICE* indice. Ponteiro para o TAD indice criado
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool indiceFecharArquivo(INDICE* indice);

    /**
     * @brief Liberar memoria do TAD aberto para o arquivo de indice linear criado
     * @param INDICE** indice. Endereco do ponteiro do TAD indice criado
     * @param bool manterArquivo. Indicador se deve ou nao ser deletado o arquivo binario criado. True: arquivo mantido. False: arquivo sera deletado
     * @return bool. true caso bem-sucedido. false: caso contrario
    */
    bool indiceDeletar(INDICE** indice, bool manterArquivo);

    /**
     * @brief Alocar memoria para o arquivo binario de indices lineares, aberto em modo leitura ou escrita
     * @param char* nomeArquivoEntrada. Nome do arquivo binario de dados a partir do qual o arquivo de indices sera construido. Valor esperado: <nomeArquivoEntrada>.bin
     * @param char* campoIndexado. Nome do campo indexado de busca. Para que a busca seja otimizada, a funcionalidade 2. deve ser chamada anteriormente com este mesmo campoIndexado.
     * @param char* tipoDado. Tipo de dado do atributo utilizado como chave de busca. Formatos esperados: string ou inteiro
     * @param char* nomeArquivoIndice. Nome do arquivo binario de indices lineares a ser criado. Valor esperado: <nomeArquivoIndice>.bin
     * @return INDICE*. Ponteiro para o TAD indice criado
    */
    INDICE* indiceCriarBinario(char* nomeArquivoEntrada, char* campoIndexado, char* tipoDado, char* nomeArquivoIndice);

    /**
     * @brief Ler e armazenar proximo dado (tipo inteiro) a partir do arquivo binario de indices lineares informado
     * @param INDICE* indice. Ponteiro para o TAD indice criado.
     * @param char* tipoDado. Tipo de dado do atributo utilizado como chave de busca. Formatos esperados: string ou inteiro.
     * @return DADOS_INDICE_INTEIRO*. Ponteiro para o TAD dados_indice_inteiro criado
    */
    DADOS_INDICE_INTEIRO* indiceLerArmazenarDadosInteiro(INDICE* indice, char* tipoDado);
    
    /**
     * @brief Ler e armazenar proximo dado (tipo char*) a partir do arquivo binario de indices lineares informado
     * @param INDICE* indice. Ponteiro para o TAD indice criado.
     * @param char* tipoDado. Tipo de dado do atributo utilizado como chave de busca. Formatos esperados: string ou inteiro.
     * @return DADOS_INDICE_STRING*. Ponteiro para o TAD dados_indice_string criado
    */
    DADOS_INDICE_STRING* indiceLerArmazenarDadosString(INDICE* indice, char* tipoDado);
    
    /**
     * @brief Ler e armazenar valores do cabecalho presente no arquivo de indices lineares aberto
     * @param INDICE* indice. Ponteiro para o TAD indice criado.
     * @return CABECALHO_INDICE*. Ponteiro para o cabecalho_indice criado
    */
    CABECALHO_INDICE* indiceLerArmazenarCabecalho(INDICE* indice);

    /**
     * @brief Reposicionar para o comeco do arquivo binario de indices o cursor de leitura/escrita (SEEK_SET)
     * @param INDICE* indice. Ponteiro para o TAD indice criado.
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool indiceResetLeituraArquivoBinario(INDICE* indice);
    
#endif