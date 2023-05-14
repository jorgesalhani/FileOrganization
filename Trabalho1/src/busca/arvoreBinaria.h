#ifndef ARVORE_BINARIA_H
    #define ARVORE_BINARIA_H

    #include "item.h"

    typedef struct arvore_binaria_ ARVORE_BINARIA;

    /**
     * @brief Alocar espaco de memoria para estrutura de arvore binaria para buscas
     * @param char* campoIndexado. Nome do campo indexado de busca. Para que a busca seja otimizada, a funcionalidade 2. deve ser chamada anteriormente com este mesmo campoIndexado.
     * @return ARVORE_BINARIA*. Ponteiro para o TAD arvore_binaria criado
    */
    ARVORE_BINARIA* arvoreBinariaCriar(char* campoIndexado);

    /**
     * @brief Verificar se alocacao feita corretamente para o TAD arvore_binaria
     * @param ARVORE_BINARIA* arvoreBinaria. Ponteiro para o TAD arvore_binaria criado
     * @return bool. true: caso existente (nao nulo). false: caso contrario
    */
    bool arvoreBinariaExiste(ARVORE_BINARIA* arvoreBinaria);

    /**
     * @brief Liberar memoria utilizada para o TAD arvore_binaria criado
     * @param ARVORE_BINARIA** arvoreBinaria. Endereco para ponteiro para o TAD arvore_binaria criado
     * @return bool. true: caso bem-sucedido. false: caso contrario
    */
    bool arvoreBinariaDeletar(ARVORE_BINARIA** arvoreBinaria);

    /**
     * @brief Armazenar em estrutura de arvore binaria todos os registros de dados existentes (nao removidos e nao nulos em relacao ao campo indexado), com ordenacao relativa ao campo indexado informado
     * @param ARVORE_BINARIA* arvoreBinaria. Ponteiro para o TAD arvore_binaria criado
     * @param TABELA* tabela. Ponteiro para o TAD tabela criado
     * @param CABECALHO* cabecalho. Ponteiro para o TAD cabecalho criado
     * @param char* tipoDado. Tipo de dado do atributo utilizado como chave de busca. Formatos esperados: string ou inteiro
     * @return bool. true: caso armazenamento bem-sucedido. false: caso contrario
    */
    bool arvoreBinariaArmazenarRegistrosOrdenados(ARVORE_BINARIA* arvoreBinaria, TABELA* tabela, CABECALHO* cabecalho, char* tipoDado);

    /**
     * @brief Obter campo indexado sob o qual a arvore binaria foi (ou sera) construida
     * @param ARVORE_BINARIA* arvoreBinaria. Ponteiro para o TAD arvore_binaria criado
     * @return char*. Nome do campo indexado escolhido
    */
    char* arvoreBinariaObterCampoIndexado(ARVORE_BINARIA* arvoreBinaria);

    /**
     * @brief Obter numero total de registros de dados de indice armazenados e ordenados na estrutura de arvore (campo 'qtdReg')
     * @param ARVORE_BINARIA* arvoreBinaria. Ponteiro para o TAD arvore_binaria criado
     * @return int32_t. Total de registros ('qtdReg') de dados que constam na arvore binaria
    */
    int32_t arvoreBinariaObterQtdReg(ARVORE_BINARIA* arvoreBinaria);

    /**
     * @brief A partir da estrutura de arvore, armazenar (escrever) no arquivo de indice linear cada um dos registros de dados em ordem crescente (em relacao ao campo indexado e ao byteOffset, caso valores iguais do campo indexado)
     * @param INDICE* indice. Ponteiro para o TAD indice criado
     * @param ARVORE_BINARIA* arvoreBinaria. Ponteiro para o TAD arvore_binaria criado
     * @param char* tipoDado. Tipo de dado do atributo utilizado como chave de busca. Formatos esperados: string ou inteiro
     * @return bool. true: caso armazenamento bem-sucedido. false: caso contrario
    */
    bool arvoreBinariaIndiceArmazenarRegistrosOrdemCrescente(INDICE* indice, ARVORE_BINARIA* arvoreBinaria, char* tipoDado);

    /**
     * @brief Adicionar novo registro de dado na estrutura de arvore binaria.
     * @param ARVORE_BINARIA* arvoreBinaria. Ponteiro para o TAD arvore_binaria criado
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @param METADADOS* metadados. Ponteiro para o TAD metadados criado
     * @param int32_t chave. Identificador do item a ser adicionado
     * @param int64_t byteOffset. Valor do byteOffset relativo ao registro de dados no arquivo de dados
     * @param char* campoIndexado Nome do campo utilizado como chave de busca para a criacao do indice linear correspondente
     * @return bool. true: caso adicao bem-sucedida. false: caso contrario
    */
    bool arvoreBinariaAdicionar(ARVORE_BINARIA* arvoreBinaria, DADOS* dados, METADADOS* metadados, int32_t chave, int64_t byteOffset, char* campoIndexado);

    /**
     * @brief Imprimir resultado da busca por campo indexado utilizando estrutura de busca em arvore binaria
     * @param ARVORE_BINARIA* arvoreBinaria. Ponteiro para o TAD arvore_binaria criado
     * @param char* campoIndexado Nome do campo utilizado como chave de busca para a criacao do indice linear correspondente
     * @param char** listaCamposDeBusca. Lista contendo os campos de busca desejados. Exemplo: {'idCrime', 'marcaCelular'}
     * @param void** listaValoresDeBusca. Lista contendo os valores de busca desejados. Exemplo: {1182, 'MOTOROLA'}
     * @param int numeroParesCampoValor. Numero de pares 'campo: valor' desejados. Ou tambem, numero de elementos das listas 'listaCamposDeBusca' e 'listaValoresDeBusca'
    */
    int arvoreBinariaBusca(
        ARVORE_BINARIA* arvoreBinaria, TABELA* tabela, CABECALHO* cabecalho, char* campoIndexado, 
        char** listaCamposDeBusca, void** listaValoresDeBusca, int numeroParesCampoValor, 
        char** listaCamposDeAtualizacao, void** listaValoresDeAtualizacao, int numeroParesCampoValorAtualizacao,
        int processamento
    );

#endif