#ifndef DADOS_H
    #define DADOS_H
    #define TAMANHO_DATA_CRIME 10
    #define TAMANHO_MARCA_CELULAR 12
    #define NUMERO_CAMPOS_VARIAVEIS 2
    #define STR_VAZIA "NULO"


    #include <stdint.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include "../indice/dadosIndice.h"

    typedef struct metadados_ METADADOS;
    typedef struct dados_ DADOS;

    /**
     * @brief Alocacao de memoria para armazenar dados de ocorrencia criminal
     * @param int32_t idCrime. Codigo identificador do crime
     * @param char[TAMANHO_DATA_CRIME] dataCrime. Data de ocorrencia do crime. Formato char[10] DD/MM/AAAA
     * @param int32_t numeroArtigo. Numero do artigo no codigo penal relacionado ao crime
     * @param char[TAMANHO_MARCA_CELULAR] marcaCelular. Marca do celular furtado/roubado. Formato char[12]
     * @param char* lugarCrime. Lugar no qual o crime ocorreu
     * @param char* descricaoCrime. Descricao detalhada do crime
     * @param char removido. Indica se o registro esta logicamente removido. Assume valores '0' ou '1'
     * @return DADO* Ponteiro para o TAD dados
    */
    DADOS* dadosCriar(int32_t idCrime, char dataCrime[TAMANHO_DATA_CRIME], int32_t numeroArtigo, 
                      char marcaCelular[TAMANHO_MARCA_CELULAR], char* lugarCrime, char* descricaoCrime, char removido);

    /**
     * @brief Alocacao de memoria para armazenar metadados sobre um registro de dados
     * @param int tamanhoDescricaoCrime. Tamanho da string 'descricaoCrime', originalmente campo de tamanho variado no registro de dados
     * @param int tamanhoLugarCrime. Tamanho da string 'lugarCrime', originalmente campo de tamanho variado no registro de dados
     * @return METADADOS*. Ponteiro para o TAD metadados criado
    */
    METADADOS* dadosCriarMetadados(int tamanhoDescricaoCrime, int tamanhoLugarCrime, int tamanhoPreenchimento);

    /**
     * @brief Imprime oconteudo dos dados
     * @param DADOS* dados. Ponteiro para o TAD dados
    */
    void dadosImprimir(DADOS* dados, METADADOS* metadados);

    /**
     * @brief Verifica existencia do TAD dados criado
     * @param DADOS* dados. Ponteiro para o TAD dados
     * @return bool true: caso existente e nao nulo. false: caso contrario
    */
    bool dadosExiste(DADOS* dados);

    /**
     * @brief Atualiza campo idCrime
     * @param DADOS* dados. Ponteiro para o TAD dados
     * @param int32_t novoIdCrime. Novo valor para o campo idCrime
     * @return bool true: caso atualizacao bem-sucedida. false: caso contrario
    */
    bool dadosAtualizarIdCrime(DADOS* dados, int32_t novoIdCrime);
    
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
     * @param int32_t novoNumeroArtigo. Novo valor para o campo numeroArtigo
     * @return bool true: caso atualizacao bem-sucedida. false: caso contrario
    */
    bool dadosAtualizarNumeroArtigo(DADOS* dados, int32_t novoNumeroArtigo);
    
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
     * @return int32_t idCrime
    */
    int32_t dadosObterIdCrime(DADOS* dados);

    /**
     * @brief Obter campo dataCrime
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @return char* dataCrime
    */
    char* dadosObterDataCrime(DADOS* dados);
    
    /**
     * @brief Obter campo numeroArtigo
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @return int32_t numeroArtigo
    */
    int32_t dadosObterNumeroArtigo(DADOS* dados);
    
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
     * @return int64_t tamanho do campo lugarCrime
    */
    int64_t dadosMetadadosObterTamanhoLugarCrime(METADADOS* metadados);

    /**
     * @brief Obter tamanho do campo variavel descricaoCrime para um dado registro
     * @param METADADOS* Ponteiro para TAD metadados
     * @return int64_t tamanho do campo descricaoCrime
    */
    int64_t dadosMetadadosObterTamanhoDescricaoCrime(METADADOS* metadados); 

    /**
     * @brief Obter tamanho do campo variavel tamanhoRegistro para um dado registro
     * @param DADOS* Ponteiro para TAD dados
     * @param METADADOS* Ponteiro para TAD metadados
     * @return int64_t tamanho do campo tamanhoRegistro
    */
    int64_t dadosMetadadosObterTamanhoRegistro(DADOS* dados, METADADOS* metadados);

    /**
     * @brief Desalocar memoria da TAD metadados
     * @param METADADOS** Endereco do ponteiro da TAD metadados
     * @return bool true: caso desalocacao bem-sucedida. false: caso contratio
    */
    bool dadosMetadadosDeletar(METADADOS** metadados);

    /**
     * @brief Verificar se TAD metadados existe
     * @param METADADOS* metadados. Ponteiro para o TAD metadados criado
     * @return bool true: caso ponteiro nao-nulo. false: caso contratio
    */
    bool metadadosExiste(METADADOS* metadados);

    /**
     * @brief Verificar validade do campo 'removido' 
     * @param char removido. Char que representa caso registro removido ou nao. Valores esperados: '0' ou '1'
     * @return bool true: caso campo valido. false: caso contrario
    */
    bool removidoValido(char removido);

    /**
     * @brief Atualizar registro de dados com novos valores informados
     * @param DADOS* dados. Ponteiro para o TAD dados criado.
     * @param METADADOS* metadados. Ponteiro para o TAD metadados criado.
     * @param char removido. Identificador de remocao logica de campo
     * @param int32_t novoIdCrime. Novo valor 'idCrime'
     * @param char* novoDataCrime. Novo valor 'dataCrime'
     * @param int32_t novoNumeroArtigo. Novo valor 'numeroArtigo'
     * @param char* novoMarcaCelular. Novo valor 'marcaCelular'
     * @param char* novoLugarCrime. Novo valor 'lugarCrime'
     * @param char* novoDescricaoCrime. Novo valor 'descricaoCrime'
     * @return bool. true: caso registro de dados atualizado com sucesso. false: caso contrario
    */
    bool dadosAtualizarRegistro(
        DADOS* dados, METADADOS* metadados,
        char removido, int32_t novoIdCrime, 
        char* novoDataCrime, int32_t novoNumeroArtigo, 
        char* novoMarcaCelular, char* novoLugarCrime, 
        char* novoDescricaoCrime
    );

    /**
     * @brief Verificar validade do valor informado 'campoIndexado' 
     * @param char* campoIndexado. Nome do campo utilizado como chave de busca para a criacao do indice linear correspondente
     * @return bool. true: caso valido. false: caso contrario
    */
    bool dadosCampoIndexadoValido(char* campoIndexado);

    /**
     * @brief Obter numero relativo ao campo indexado. Forma de associar um inteiro com a lista de possibilidades. Sao Elas:
     *        {idCrime, dataCrime, numeroArtigo, marcaCelular, lugarCrime, descricaoCrime, removido, delimitador}
     * @param char* campoIndexado. Nome do campo utilizado como chave de busca para a criacao do indice linear correspondente
     * @return int. Valor relativo a posicao do id na lista de campos indexados possiveis
    */
    int dadosObterNumeroCampoIndexado(char* campoIndexado);

    /**
     * @brief Obter valor do registro de dados relativo ao campo indexado de desejo
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @param char* campoIndexado. Nome do campo utilizado como chave de busca para a criacao do indice linear correspondente
     * @return void*. Ponteiro opaco para o valor do campo do registro de dados. 
    */
    void* dadosObterCampoIndexado(DADOS* dados, char* campoIndexado);

    /**
     * @brief Verificar se um registro de dados corresponde aos valores desejados como campos de busca
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @param char** listaCamposDeBusca. Lista contendo os campos de busca desejados. Exemplo: {'idCrime', 'marcaCelular'}
     * @param void** listaValoresDeBusca. Lista contendo os valores de busca desejados. Exemplo: {1182, 'MOTOROLA'}
     * @param int numeroParesCampoValor. Numero de pares 'campo: valor' desejados. Ou tambem, numero de elementos das listas 'listaCamposDeBusca' e 'listaValoresDeBusca'
     * @return bool. true: caso dado de registro corresponde aos campos buscados. false: caso contrario
    */
    bool dadosBuscaCorrespondenciaCompleta(
        DADOS* dados, char** listaCamposDeBusca,
        void** listaValoresDeBusca, int numeroParesCampoValor
    );

    /**
     * @brief Verificar validade do valor relativo ao campoIndexado para um dado registro de dados 
     * @param DADOS* dados. Ponteiro para o TAD dados criado
     * @param char* campoIndexado Nome do campo utilizado como chave de busca para a criacao do indice linear correspondente
     * @param char* tipoDado. Tipo de dado do atributo utilizado como chave. Valor esperado: "string" ou "inteiro"
     * @return bool. true: caso registro contenha valores nao nulos para o campo desejado. false: caso contrario
    */
    bool dadosValorIndexadoValido(DADOS* dados, char* campoIndexado, char* tipoDado);

    bool dadosAtualizaCamposEspecificados(DADOS* dados, METADADOS* metadados, char** listaCamposDeAtualizacao, void** listaValoresDeAtualizacao, int numeroParesCampoValorAtualizacao);

    bool dadosRemovido(DADOS* dados);

    int dadosMetadadosObterTamanhoPreenchimento(METADADOS* metadados);
    bool dadosMetadadosAtualizarTamanhoPreenchimento(METADADOS* metadados, int novoTamanhoPreenchimento);
    int dadosObterEnumCampo(char* campoIndexado);

#endif