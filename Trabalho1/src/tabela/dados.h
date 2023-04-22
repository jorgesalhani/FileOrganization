#ifndef DADOS_H
    #define DADOS_H
    #define TAMANHO_DATA_CRIME 10
    #define TAMANHO_MARCA_CELULAR 12
    #define NUMERO_CAMPOS_VARIAVEIS 2

    #include <stdint.h>
    #include <stdlib.h>
    #include <stdbool.h>

    typedef struct metadados_ METADADOS;
    typedef struct dados_ DADOS;

    /**
     * @brief Alocacao de memoria para armazenar dados de ocorrencia criminal
     * @param uint32_t idCrime. Codigo identificador do crime
     * @param char[TAMANHO_DATA_CRIME] dataCrime. Data de ocorrencia do crime. Formato char[10] DD/MM/AAAA
     * @param uint32_t numeroArtigo. Numero do artigo no codigo penal relacionado ao crime
     * @param char[TAMANHO_MARCA_CELULAR] marcaCelular. Marca do celular furtado/roubado. Formato char[12]
     * @param char* lugarCrime. Lugar no qual o crime ocorreu
     * @param char* descricaoCrime. Descricao detalhada do crime
     * @param char removido. Indica se o registro esta logicamente removido. Assume valores '0' ou '1'
     * @return DADO* Ponteiro para o TAD dados
    */
    DADOS* dadosCriar(uint32_t idCrime, char dataCrime[TAMANHO_DATA_CRIME], uint32_t numeroArtigo, 
                      char marcaCelular[TAMANHO_MARCA_CELULAR], char* lugarCrime, char* descricaoCrime, char removido);

    METADADOS* dadosCriarMetadados();

    /**
     * @brief Imprime oconteudo dos dados
     * @param DADOS* dados. Ponteiro para o TAD dados
    */
    void dadosImprimir(DADOS* dados);

    /**
     * @brief Verifica existencia do TAD dados criado
     * @param DADOS* dados. Ponteiro para o TAD dados
     * @return bool true: caso existente e nao nulo. false: caso contrario
    */
    bool dadosExiste(DADOS* dados);

    /**
     * @brief Atualiza campo idCrime
     * @param DADOS* dados. Ponteiro para o TAD dados
     * @param uint32_t novoIdCrime. Novo valor para o campo idCrime
     * @return bool true: caso atualizacao bem-sucedida. false: caso contrario
    */
    bool dadosAtualizarIdCrime(DADOS* dados, uint32_t novoIdCrime);
    
    /**
     * @brief Atualiza campo dataCrime
     * @param DADOS* dados. Ponteiro para o TAD dados
     * @param char* novoDataCrime. Novo valor para o campo dataCrime
     * @return bool true: caso atualizacao bem-sucedida. false: caso contrario
    */
    bool dadosAtualizarDataCrime(DADOS* dados, char* novoDataCrime);
    
    /**
     * @brief Atualiza campo numeroArtigo
     * @param DADOS* dados. Ponteiro para o TAD dados
     * @param uint32_t novoNumeroArtigo. Novo valor para o campo numeroArtigo
     * @return bool true: caso atualizacao bem-sucedida. false: caso contrario
    */
    bool dadosAtualizarNumeroArtigo(DADOS* dados, uint32_t novoNumeroArtigo);
    
    /**
     * @brief Atualiza campo marcaCelular
     * @param DADOS* dados. Ponteiro para o TAD dados
     * @param char* novoMarcaCelular. Novo valor para o campo marcaCelular
     * @return bool true: caso atualizacao bem-sucedida. false: caso contrario
    */
    bool dadosAtualizarMarcaCelular(DADOS* dados, char* novoMarcaCelular);
    
    /**
     * @brief Atualiza campo lugarCrime
     * @param DADOS* dados. Ponteiro para o TAD dados
     * @param char* novoLugarCrime. Novo valor para o campo lugarCrime
     * @return bool true: caso atualizacao bem-sucedida. false: caso contrario
    */
    bool dadosAtualizarLugarCrime(DADOS* dados, char* novoLugarCrime, METADADOS* metadados);
    
    /**
     * @brief Atualiza campo descricaoCrime
     * @param DADOS* dados. Ponteiro para o TAD dados
     * @param char* novoDescricaoCrime. Novo valor para o campo descricaoCrime
     * @return bool true: caso atualizacao bem-sucedida. false: caso contrario
    */
    bool dadosAtualizarDescricaoCrime(DADOS* dados, char* novoDescricaoCrime, METADADOS* metadados);
    
    /**
     * @brief Atualiza campo removido
     * @param DADOS* dados. Ponteiro para o TAD dados
     * @param char removido. Novo valor para o campo removido
     * @return bool true: caso atualizacao bem-sucedida. false: caso contrario
    */
    bool dadosAtualizarRemovido(DADOS* dados, char removido);

    /**
     * @brief Obter campo idCrime
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @return uint32_t idCrime
    */
    uint32_t dadosObterIdCrime(DADOS* dados);

    /**
     * @brief Obter campo dataCrime
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @return char* dataCrime
    */
    char* dadosObterDataCrime(DADOS* dados);
    
    /**
     * @brief Obter campo numeroArtigo
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @return uint32_t numeroArtigo
    */
    uint32_t dadosObterNumeroArtigo(DADOS* dados);
    
    /**
     * @brief Obter campo marcaCelular
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @return char* marcaCelular
    */
    char* dadosObterMarcaCelular(DADOS* dados);
    
    /**
     * @brief Obter campo lugarCrime
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @return char* lugarCrime
    */
    char* dadosObterLugarCrime(DADOS* dados);
    
    /**
     * @brief Obter campo descricaoCrime
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @return char* descricao Crime
    */
    char* dadosObterDescricaoCrime(DADOS* dados);
    
    /**
     * @brief Obter campo removido
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @return char removido
    */
    char dadosObterRemovido(DADOS* dados);

    /**
     * @brief Obter campo delimitador de registro
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @return char delimitador
    */
    char dadosObterDelimitadorRegistro(DADOS* dados);

    /**
     * @brief Libera memoria alocada para o TAD dados
     * @param DADOS** Endereco do ponteiro do TAD dados
     * @return bool true: caso desalocacao bem-sucedida. false: caso contratio
    */
    bool dadosDeletar(DADOS** dados);

    /**
     * @brief Obter tamanho do campo variavel lugarCrime para um dado registro
     * @param METADADOS* Ponteiro para TAD metadados
     * @return uint64_t tamanho do campo lugarCrime
    */
    uint64_t dadosMetadadosObterTamanhoLugarCrime(METADADOS* metadados);

    /**
     * @brief Obter tamanho do campo variavel descricaoCrime para um dado registro
     * @param METADADOS* Ponteiro para TAD metadados
     * @return uint64_t tamanho do campo descricaoCrime
    */
    uint64_t dadosMetadadosObterTamanhoDescricaoCrime(METADADOS* metadados); 

    /**
     * @brief Obter tamanho do campo variavel tamanhoRegistro para um dado registro
     * @param DADOS* Ponteiro para TAD dados
     * @param METADADOS* Ponteiro para TAD metadados
     * @return uint64_t tamanho do campo tamanhoRegistro
    */
    uint64_t dadosMetadadosObterTamanhoRegistro(DADOS* dados, METADADOS* metadados);

    /**
     * @brief Desalocar memoria da TAD metadados
     * @param METADADOS** Endereco do ponteiro da TAD metadados
     * @return bool true: caso desalocacao bem-sucedida. false: caso contratio
    */
    bool dadosMetadadosDeletar(METADADOS** metadados);

#endif