#ifndef CABECALHO_INDICE_H
    #define CABECALHO_INDICE_H

    #include <stdbool.h>
    #include <stdlib.h>
    #include <stdint.h>

    typedef struct cabecalho_indice_ CABECALHO_INDICE;

    /**
     * @brief Alocar memoria para cabecalho do arquivo binario de indice linear criado
     * @param char status. Status do arquivo de indice. Formatos esperados: '0' ou '1'
     * @return CABECALHO_INDICE*. Ponteiro para o TAD cabecalho_indice criado
    */
    CABECALHO_INDICE* cabecalhoIndiceCriar(char status);

    /**
     * @brief Verificar se TAD cabecalho_indice foi alocado corretamente
     * @param CABECALHO_INDICE*. Ponteiro para o TAD cabecalho_indice criado
     * @return bool. true: caso alocacao bem-sucedida. false: caso contrario
    */
    bool cabecalhoIndiceExiste(CABECALHO_INDICE* cabecalhoIndice);

    /**
     * @brief Atualizar registro de cabecalho do arquivo de indice com novo campo 'status'
     * @param char novoStatus. Status do arquivo de indice. Formatos esperados: '0' ou '1'
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool cabecalhoIndiceAtualizarStatus(CABECALHO_INDICE* cabecalhoIndice, char novoStatus);

    /**
     * @brief Obter valor com campo 'status' do registro de cabecalho do arquivo de indices
     * @param CABECALHO_INDICE*. Ponteiro para o TAD cabecalho_indice criado
     * @return char. Valor do campo 'status' do registro de cabecalho
    */
    char cabecalhoIndiceObterStatus(CABECALHO_INDICE* cabecalhoIndice);

    /**
     * @brief Liberar memoria utilizada para o registro de cabecalho do arquivo binario de indice
     * @param CABECALHO_INDICE**. endereco do ponteiro para o TAD cabecalho_indice criado
     * @return bool. true: caso liberacao bem-sucedida. false: caso contrario
    */
    bool cabecalhoIndiceDeletar(CABECALHO_INDICE** cabecalhoIndice);

    /**
     * @brief Atualizar registro de cabecalho do arquivo de indice com novo campo 'qtdReg'
     * @param int32_t novoQtdReg. Novo valor do total de registros presentes no arquivo de indice.
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool cabecalhoIndiceAtualizarQtdReg(CABECALHO_INDICE* cabecalhoIndice, int32_t novoQtdReg);

    /**
     * @brief Obter valor do campo 'qtdReg' do registro de cabecalho do arquivo de indices
     * @param CABECALHO_INDICE*. Ponteiro para o TAD cabecalho_indice criado
     * @return char. Valor do campo 'qtdReg' do registro de cabecalho
    */
    int32_t cabecalhoIndiceObterQtdReg(CABECALHO_INDICE* cabecalhoIndice);

    bool cabecalhoIndiceConsistente(CABECALHO_INDICE* cabecalhoIndice);
    
#endif