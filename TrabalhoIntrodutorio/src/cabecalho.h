#ifndef CABECALHO_H
    #define CABECALHO_H

    #include <stdlib.h>
    #include <stdint.h>
    #include <stdbool.h>

    typedef struct cabecalho_ CABECALHO;


    /**
     * @brief Alocacao de memoria para cabecalho
     * @param char status. Consistencia do arquivo de dados. Assume valores: '0' ou '1'.
     * @param int64_t proxByteOffset. Valor do proximo byte offset disponivel. Inicializado em 0.
     * @param int32_t nroRegArq. Numero de registros presentes no arquivo. Inclui registros logicamente marcados como removido. Inicializado em 0.
     * @param int32_t nroRegRem. Numero de registros logicamente marcados como removido. Inicializado em 0.
     * @return CABECALHO* Ponteiro para o TAD cabecalho
    */
    CABECALHO* cabecalhoCriar(char status, int64_t proxByteOffset, int32_t nroRegArq, int32_t nroRegRem);

    /**
     * @brief Imprime conteudo do cabecalho
     * @param CABECALHO* cabecalho. Ponteiro para o TAD cabecalho
    */
    void cabecalhoImprimir(CABECALHO* cabecalho);

    /**
     * @brief Verifica existencia do TAD cabecalho criado
     * @param CABECALHO* cabecalho. Ponteiro para o TAD cabecalho
     * @return bool true: caso existente e nao nulo. false: caso contrario
    */
    bool cabecalhoExiste(CABECALHO* cabecalho);

    /**
     * @brief Atualiza campo status do cabecalho
     * @param CABECALHO* Ponteiro para o TAD cabecalho
     * @param char novoStatus. Novo valor para o campo status
     * @return bool true: caso atualizacao bem-sucedida. false: caso contrario
    */
    bool cabecalhoAtualizarStatus(CABECALHO* cabecalho, char novoStatus);

    /**
     * @brief Atualiza campo proxByteOffset do cabecalho
     * @param CABECALHO* Ponteiro para o TAD cabecalho
     * @param int64_t novoProxByteOffset. Novo valor para o campo proxByteOffset
     * @return bool true: caso atualizacao bem-sucedida. false: caso contrario
    */
    bool cabecalhoAtualizarProxByteOffset(CABECALHO* cabecalho, int64_t novoProxByteOffset);
    
    /**
     * @brief Atualiza campo nroRegArq do cabecalho
     * @param CABECALHO* Ponteiro para o TAD cabecalho
     * @param int32_t novoNroRegArq. Novo valor para o campo nroRegArq
     * @return bool true: caso atualizacao bem-sucedida. false: caso contrario
    */
    bool cabecalhoAtualizarNroRegArq(CABECALHO* cabecalho, int32_t novoNroRegArq);
    
    /**
     * @brief Atualiza campo nroRegRem do cabecalho
     * @param CABECALHO* Ponteiro para o TAD cabecalho
     * @param int32_t novoNroRegRem. Novo valor para o campo nroRegRem
     * @return bool true: caso atualizacao bem-sucedida. false: caso contrario
    */
    bool cabecalhoAtualizarNroRegRem(CABECALHO* cabecalho, int32_t novoNroRegRem);

    /**
     * @brief Obter campo status do cabecalho
     * @param CABECALHO* Ponteiro para o TAD cabecalho
     * @return char status
    */
    char cabecalhoObterStatus(CABECALHO* cabecalho);

    /**
     * @brief Obter campo proxByteOffset do cabecalho
     * @param CABECALHO* Ponteiro para o TAD cabecalho
     * @return int64_t proxByteOffset
    */
    int64_t cabecalhoObterProxByteOffset(CABECALHO* cabecalho);

    /**
     * @brief Obter campo nroRegArq do cabecalho
     * @param CABECALHO* Ponteiro para o TAD cabecalho
     * @return int32_t nroRegArq
    */
    int32_t cabecalhoObterNroRegArq(CABECALHO* cabecalho);

    /**
     * @brief Obter campo nroRegRem do cabecalho
     * @param CABECALHO* Ponteiro para o TAD cabecalho
     * @return int32_t nroRegRem
    */
    int32_t cabecalhoObterNroRegRem(CABECALHO* cabecalho);

    /**
     * @brief Obter tamanho do registro de cabecalho
     * @param CABECALHO* Ponteiro para o TAD cabecalho
     * @return int64_t tamanho
    */
    int64_t cabecalhoObterTamanhoRegistro(CABECALHO* cabecalho);
    
    /**
     * @brief Libera memoria alocada para o TAD cabecalho
     * @param CABECALHO** endereco do ponteiro do TAD cabecalho
     * @return bool true: caso desalocacao bem-sucedida. false: caso contratio
    */
    bool cabecalhoDeletar(CABECALHO** cabecalho);

#endif