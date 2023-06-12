#ifndef ITEM_H
    #define ITEM_H

    #include "../indice/indice.h"
    #include "../tabela/tabela.h"

    typedef struct item_ ITEM;

    /**
     * @brief Alocar memoria para o TAD item criado, utilizado para ordenacao dos registros do arquivo binario de dados
     * @param int32_t chave. Identificador do item
     * @param int64_t byteOffset. Valor do byteOffset do registro no arquivo binario de dados
     * @param void* valorCampo. Ponteiro para o valor do campo de busca (inteiro ou string) escolhido para indexacao
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @param METADADOS* metadados. Ponteiro para o TAD metadados criado
     * @return ITEM*. Ponteiro para o TAD item criado
    */
    ITEM* itemCriar(int32_t chave, int64_t byteOffset, void* valorCampo, DADOS* dados, METADADOS* metadados);

    /**
     * @brief Verificar se TAD item existe (alocado corretamente)
     * @param ITEM* item. Ponteiro para o TAD item criado
     * @return bool. true: caso existente (nao nulo). false: caso contrario
    */
    bool itemExiste(ITEM* item);

    /**
     * @brief Obter chave (identificador) do item
     * @param ITEM* item. Ponteiro para o TAD item criado
     * @return it32_t. Valor do identificador do item
    */
    int32_t itemObterChave(ITEM* item);

    /**
     * @brief Obter valor do 'byteOffset' associado ao item, que por sua vez esta associado a um registro do arquivo binario de dados
     * @param ITEM* item. Ponteiro para o TAD item criado
     * @return int64_t. Valor do byteOffset associado ao TAD item
    */
    int64_t itemObterByteOffset(ITEM* item);

    /**
     * @brief Obter registro de dados associado ao item
     * @param ITEM* item. Ponteiro para o TAD item criado
     * @return DADOS*. Ponteiro para o TAD dados ao qual o TAD item esta associado
    */
    DADOS* itemObterDados(ITEM* item);

    /**
     * @brief Obter metadados associados ao item (tamanho de campos variados do registro de dados)
     * @param ITEM* item. Ponteiro para o TAD item criado
     * @return METADADOS*. Ponteiro para o TAD metadados ao qual o TAD item esta associado
    */
    METADADOS* itemObterMetadados(ITEM* item);

    /**
     * @brief Atualizar identificador ('chave') do TAD item
     * @param ITEM* item. Ponteiro para o TAD item criado
     * @param int32_t novaChave. Valor da nova chave
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool itemAtualizarChave(ITEM* item, int32_t novaChave);

    /**
     * @brief Atualizar valor do campo de busca (string ou inteiro) vinculado ao TAD item
     * @param ITEM* item. Ponteiro para o TAD item criado
     * @param void* valorCampo. Ponteiro para o novo valor do campo de busca
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool itemAtualizarValorCampo(ITEM* item, void* valorCampo);

    /**
     * @brief Liberar memoria alocada para o TAD item
     * @param ITEM** item. Endereco do Ponteiro para o TAD item criado
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool itemDeletar(ITEM** item);

#endif