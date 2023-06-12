#ifndef DADOS_INDICE_H
    #define DADOS_INDICE_H
    #define TAMANHO_CHAVE_BUSCA 12

    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdint.h>
    #include <string.h>

    typedef struct dados_indice_inteiro_ DADOS_INDICE_INTEIRO;
    typedef struct dados_indice_string_ DADOS_INDICE_STRING;

    /**
     * @brief Alocar memoria para armazenar registro de dados para o arquivo binario de indices lineares com cave de busca tipo inteiro.
     * @param char* tipoDado. Tipo de dado do atributo utilizado como chave de busca. Formato esperado: inteiro
     * @param int32_t chaveBuscaInteiro. Valor da chave de busca desejada
     * @param int64_t byteOffset. Valor do byteOffset do registro no arquivo binario de dados
     * @return DADOS_INDICE_INTEIRO*. Ponteiro para o TAD dados_indice_inteiro criado
    */
    DADOS_INDICE_INTEIRO* dadosIndiceInteiroCriar(char* tipoDado, int32_t chaveBuscaInteiro, int64_t byteOffset);
    
    /**
     * @brief Alocar memoria para armazenar registro de dados para o arquivo binario de indices lineares com cave de busca tipo string (char*).
     * @param char* tipoDado. Tipo de dado do atributo utilizado como chave de busca. Formato esperado: string
     * @param char* chaveBuscaString. Valor da chave de busca desejada
     * @param int64_t byteOffset. Valor do byteOffset do registro no arquivo binario de dados
     * @return DADOS_INDICE_STRING*. Ponteiro para o TAD dados_indice_string criado
    */
    DADOS_INDICE_STRING* dadosIndiceStringCriar(char* tipoDado, char chaveBuscaString[TAMANHO_CHAVE_BUSCA], int64_t byteOffset);

    /**
     * @brief Verificar validade do campo 'tipoDado' informado para o caso string
     * @param char* tipoDado. Tipo de dado do atributo utilizado como chave de busca. Formato esperado: string
     * @return bool. true: caso campo valido. false: caso contrario
    */
    bool tipoDadoStringValido(char* tipoDado);
    
    /**
     * @brief Verificar validade do campo 'tipoDado' informado para o caso inteiro
     * @param char* tipoDado. Tipo de dado do atributo utilizado como chave de busca. Formato esperado: inteiro
     * @return bool. true: caso campo valido. false: caso contrario
    */
    bool tipoDadoInteiroValido(char* tipoDado);

    /**
     * @brief Verificar alocacao correta para o TAD dados_indice_inteiro
     * @param DADOS_INDICE_INTEIRO* dadosIndiceInteiro. Ponteiro para o TAD dados_indice_inteiro criado
     * @return bool. true: caso ponteiro com conteudo alocado corretamente. false: caso contrario
    */
    bool dadosIndiceInteiroExiste(DADOS_INDICE_INTEIRO* dadosIndiceInteiro);
    
    /**
     * @brief Verificar alocacao correta para o TAD dados_indice_string
     * @param DADOS_INDICE_STRING* dadosIndiceString. Ponteiro para o TAD dados_indice_string criado
     * @return bool. true: caso ponteiro com conteudo alocado corretamente. false: caso contrario
    */
    bool dadosIndiceStringExiste(DADOS_INDICE_STRING* dadosIndiceString);

    /**
     * @brief Obter valor da chave de busca para o tipo de dado inteiro
     * @param DADOS_INDICE_INTEIRO* dadosIndiceInteiro. Ponteiro para o TAD dados_indice_inteiro criado 
     * @return int32_t. Valor da chave de busca. Caso TAD inexistente: -1
    */
    int32_t dadosIndiceObterChaveBuscaInteiro(DADOS_INDICE_INTEIRO* dadosIndiceInteiro);
    
    /**
     * @brief Obter valor da chave de busca para o tipo de dado string
     * @param DADOS_INDICE_STRING* dadosIndiceString. Ponteiro para o TAD dados_indice_string criado
     * @return char*. Valo da chave de busca. Caso TAD inexistente: "$"
    */
    char* dadosIndiceObterChaveBuscaString(DADOS_INDICE_STRING* dadosIndiceString);

    /**
     * @brief Obter valor do byteOffset do registro de dado do arquivo binario de indices, relativo ao registro no arquivo binario de dados indexado
     * @param DADOS_INDICE_INTEIRO* dadosIndiceInteiro. Ponteiro para o TAD dados_indice_inteiro criado 
     * @return int64_t. Valor do byteOffset. Caso TAD inexistente: -1
    */
    int64_t dadosIndiceInteiroObterByteOffset(DADOS_INDICE_INTEIRO* dadosIndiceInteiro);
    
    /**
     * @brief Obter valor do byteOffset do registro de dado do arquivo binario de indices, relativo ao registro no arquivo binario de dados indexado
     * @param DADOS_INDICE_STRING* dadosIndiceString. Ponteiro para o TAD dados_indice_inteiro criado 
     * @return int64_t. Valor do byteOffset. Caso TAD inexistente: -1
    */
    int64_t dadosIndiceStringObterByteOffset(DADOS_INDICE_STRING* dadosIndiceString);

    /**
     * @brief Atualizar registro de dados de indice linear com novo valor da chave de busca do tipo inteiro
     * @param DADOS_INDICE_INTEIRO* dadosIndice. Ponteiro para o TAD dados_indice_inteiro criado 
     * @param int32_t novaChaveBuscaInteiro. Novo valor da chave de busca do tipo inteiro
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool dadosIndiceInteiroAtualizarChaveBusca(DADOS_INDICE_INTEIRO* dadosIndice, int32_t novaChaveBuscaInteiro);
    
    /**
     * @brief Atualizar registro de dados de indice linear com novo valor da chave de busca do tipo string (char*)
     * @param DADOS_INDICE_STRING* dadosIndice. Ponteiro para o TAD dados_indice_string criado 
     * @param char* novaChaveBuscaString. Novo valor da chave de busca do tipo string (char*).
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool dadosIndiceStringAtualizarChaveBusca(DADOS_INDICE_STRING* dadosIndice, char novaChaveBuscaString[TAMANHO_CHAVE_BUSCA]);

    /**
     * @brief Atualizar registro de dados de indice linear com novo valor da byteOffset para indice do tipo inteiro
     * @param DADOS_INDICE_INTEIRO* dadosIndice. Ponteiro para o TAD dados_indice_inteiro criado 
     * @param int64_t novoByteOffset. Novo valor do byteOffset
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool dadosIndiceInteiroAtualizarByteOffset(DADOS_INDICE_INTEIRO* dadosIndice, int64_t novoByteOffset);
    
    /**
     * @brief Atualizar registro de dados de indice linear com novo valor da byteOffset para indice do tipo string
     * @param DADOS_INDICE_STRING* dadosIndice. Ponteiro para o TAD dados_indice_string criado 
     * @param int64_t novoByteOffset. Novo valor do byteOffset
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool dadosIndiceStringAtualizarByteOffset(DADOS_INDICE_STRING* dadosIndice, int64_t novoByteOffset);

    /**
     * @brief Desalocar memoria utilizada para armazenar registro de dados para campo indexado do tipo inteiro
     * @param DADOS_INDICE_INTEIRO** dadosIndiceInteiro. Endereco do ponteiro para o TAD dados_indice_inteiro criado
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool dadosIndiceInteiroDeletar(DADOS_INDICE_INTEIRO** dadosIndiceInteiro);

    /**
     * @brief Desalocar memoria utilizada para armazenar registro de dados para campo indexado do tipo string (char*)
     * @param DADOS_INDICE_STRING** dadosIndiceString. Endereco do ponteiro para o TAD dados_indice_string criado
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool dadosIndiceStringDeletar(DADOS_INDICE_STRING** dadosIndiceString);

    /**
     * @brief Truncar chave de busca do tipo string para um valor maximo de caracteres: TAMANHO_CHAVE_BUSCA
     * @param char* valor. Valor da chave de busca string a ser truncada
     * @return char*. Nova string com truncamento correspondente ao tamanho maximo permitido
    */
    char* dadosIndiceTruncarString(char* valor);

#endif