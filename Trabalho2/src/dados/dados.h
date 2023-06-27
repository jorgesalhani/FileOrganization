#ifndef DADOS_H
  #define DADOS_H

  #define TAMANHO_DATA_CRIME 10
  #define TAMANHO_MARCA_CELULAR 12
  #define NUMERO_CAMPOS_VARIAVEIS 2
  #define STR_VAZIA "NULO"
  #define TAM_MAX_STR 250
  #define MODO_LEITURA_ARQUIVO "rb+"
  #define MODO_ESCRITA_ARQUIVO "wb+"

  #include <stdlib.h>
  #include <stdbool.h>
  #include <stdio.h>
  #include <stdint.h>
  #include "../utilitarios/entrada.h"
  #include "../utilitarios/erros.h"
  #include "../utilitarios/funcoesFornecidas.h"

  typedef struct registro_ REGISTRO;
  typedef struct cabecalho_ CABECALHO;
  typedef struct arvore_binaria_ ARVORE_BINARIA;
  typedef struct args_ ARGS;
  typedef struct no_ NO;

  typedef struct registro_indice_ REGISTRO_INDICE;

  typedef struct chave_ CHAVE;
  typedef struct cabecalho_indice_ CABECALHO_INDICE;

  enum ordemBuscaBinaria_ {
    emOrdem, preOrdem, posOrdem
  };

  struct no_ {
    REGISTRO* registro;
    NO* esquerda;
    NO* direita;
  };

  struct arvore_binaria_ {
    int32_t totalRegistros;
    enum ordemBuscaBinaria_ ordem;
    NO* raiz;
  };

  struct args_ {
    ENTRADA* entrada;
    REGISTRO* registro;
    CABECALHO* cabecalho;
    ARVORE_BINARIA* arvoreBinaria;
    FILE* arquivoDadosBin;
    FILE* arquivoIndiceBin;
    bool regEncontradoEmBusca;
    int idProxRamo;
    NO* proxRamoArvore;
    int64_t bOffPrimOcorArqIndice;
    CABECALHO_INDICE* cabecalhoIndice;
    REGISTRO_INDICE* registroIndice;
    int64_t byteOffPaiRegSplitado;
    int32_t idCrimeBuscado;
  };

  /**
   * @brief Criar arquivo binario a partir de dados.csv
   * @param ENTRADA* entrada. TAD contendo as informacoes de entrada para cada funcionalidade
   * @return bool. true caso bem-sucedido. false, caso contrario
  */
  bool dadosCriarArquivoBinario(ENTRADA* entrada);
  
  /**
   * @brief Imprimir todo conteudo do arquivo binario de dados
   * @param ENTRADA* entrada. TAD para campos de input informados
   * @return bool. true caso bem-sucedido. false, caso contrario
  */
  bool dadosImprimirArquivoBinario(ENTRADA* entrada);
  
  /**
   * @brief Armazenar registros de dados em uma arvore binaria para busca
   * @param ENTRADA* entrada. TAD para campos de input informados
   * @return ARGS*. Ponteiro para o TAD args, contendo informacoes da execucao, inclusa arvore binaria resultante
  */
  ARGS* dadosRegistrosEmArvoreBinaria(ENTRADA* entrada);
  
  /**
   * @brief Resetar ponteiro de leitura para determinado byteoffset em um arquivo
   * @param FILE* arquivoBin. Arquivo lido
   * @param int64_t byteOffset. Byteoffset para onde o ponteiro sera redirecionado
   * @return bool. true caso bem-sucedido. false, caso contrario
  */
  bool resetLeituraDeArquivo(FILE* arquivoBin, int64_t byteOffset);
  
  /**
   * @brief Executar varredura por toda arvore binaria com registros ordenados
   * @param ARGS* args. TAD args com informacoes da arvore binaria a ser varrida
   * @param NO* raiz. No raiz da arvore binaria
   * @param void (*ftnPorRegistro) funcao a ser executada por registro encontrado
  */
  void dadosVarreduraCompletaArvoreBinaria(ARGS* args, NO* raiz, void (*ftnPorRegistro)()); 
  
  /**
   * @brief Ler registro do arquivo binario de dados e armazenar conteudo em RAM
   * @param FILE* arquivoBinarioDados. Arquivo binario a ser lido
   * @param REGISTRO* registro. TAD registro resultante da consulta
   * @return bool. true caso bem-sucedido. false, caso contrario
  */
  bool dadosLerRegistroDoArquivoBinario(FILE* arquivoBinarioDados, REGISTRO* registro);
  
  /**
   * @brief Obter valor do campo indexado para um determinado registro
   * @param REGISTRO* registro. TAD registro a ser consultado
   * @param char* campoIndexado. Campo cujo valor deseja-se buscar
   * @return void*. Ponteiro para o conteudo do campo consultado
  */
  void* dadosObterCampoIndexado(REGISTRO* registro, char* campoIndexado);
  
  /**
   * @brief Obter valor de byteOffset de um registro do arquivo binario de dados
   * @param REGISTRO* registro. TAD registro de dados a ser consultado
   * @return int64_t. valor byteoffset do registro
  */
  int64_t dadosObterByteoffset(REGISTRO* registro);
  
  /**
   * @brief Liberar memoria utilizada para TAD args
   * @param ARGS** args. Endereco do ponteiro de args
   * @return bool. true caso bem-sucedido. false, caso contrario
  */
  bool argsApagar(ARGS** args);
  
  /**
   * @brief Liberar memoria utilizada para TAD registro de dados
   * @param REGISTRO** registro. Endereco do ponteiro de registro de dados
   * @return bool. true caso bem-sucedido. false, caso contrario
  */
  bool dadosRegistroApagar(REGISTRO** registro);
  
  /**
   * @brief Preecher string com trailling '$'
   * @param char* dest. String destino preenchida
   * @param char* orig. String original
   * @param int tamanho. Tamanho da string apos o preenchimento
   * @param bool comDelimitador. Adicionar delimitador de campo '|'
  */
  void preencherString(char* dest, char* orig, int tamanho, bool comDelimitador);
  
  /**
   * @brief Executar busca de registro de dados sequencial ou por indice. Aplicar funcao a cada registro encontrado
   * @param ENTRADA* entrada. Informacoes de input
   * @param void (*ftnPorBusca). Funcao a ser aplicada por registro encontrado
   * @return bool. true caso bem-sucedido. false, caso contrario
  */
  bool dadosBuscarPorCampos(ENTRADA* entrada, void (*ftnPorBusca));
  
  /**
   * @brief Comparar campo do registro atual com uma dada chave de busca
   * @param ARGS* args. TAD contendo informacoes do registro a ser comparado
   * @return int. -1, caso chave de busca menor que registro
   *               0, caso igual
   *              +1, caso chave de busca maior que registro
  */
  int dadosCompararRegistroComChaveBusca(ARGS* args);
  
  /**
   * @brief Copiar os dados de um registro para outra posicao de memoria
   * @param REGISTRO* regOrig. Registro original
   * @param REGISTRO* regDest. Novo registro copiado
  */
  void copiarRegistro(REGISTRO* regOrig, REGISTRO* regDest);
  
  /**
   * @brief Inicializa TAD contendo informacoes relevantes para a execucao do programa
   * @return ARGS*. Ponteiro para o TAD args criado
  */
  ARGS* argsInit();
  
  /**
   * @brief Remove um determinado registro logicamente (status 1)
   * @param ARGS* args. TAD contendo informacoes do registro a ser removido
   * @return bool. true caso bem-sucedido. false, caso contrario
  */
  bool dadosRemoverRegistroLogicamente(ARGS* args);
  
  /**
   * @brief Obter campo idCrime de um dado registro
   * @param REGISTRO* registro. TAD registro de dados para consulta
   * @return int. Valor do campo idCrime
  */
  int dadosObterIdCrime(REGISTRO* registro);
  
  /**
   * @brief Verificar se um dado registro apresenta match com todos os campos de busca. Se sim, aplicar a funcao desejada
   * @param ARGS* args. TAD args contendo informacoes do registro a ser buscado
   * @param void (*ftnPorRegistro) funcao a ser aplicada sobre o registro, caso corresponda aos valores de busca
  */
  void dadosEncontrar(ARGS* args, void (*ftnPorRegistro)());
  
  /**
   * @brief Imprime um determinado registro do arquivo binario de dados
   * @param ARGS* args. TAD args contendo informacoes do registro a ser impresso
  */
  void dadosImprimirRegistro(ARGS* args);
  
  /**
   * @brief Atualizar byteoffset da posicao em que se localiza um dado registro no arquivo binario de dados
   * @param REGISTRO* registro. TAD registro contendo o registro do arquivo binario de dados
   * @param int64_t byteOffset. Novo valor de byteoffset
  */
  void dadosAtualizarByteoffset(REGISTRO* registro, int64_t byteOffset);
  
  /**
   * @brief Obter status 'removido' de um registro do arquivo binario de dados
   * @param REGISTRO* registro. TAD registro, contendo o registro de dados a ser verificado
   * @return int64_t. Status do registro
  */
  int64_t dadosObterRemovido(REGISTRO* registro);
  
  /**
   * @brief Insere multiplos registros de dados ao fim do arquivo binario de dados
   * @param ENTRADA* entrada. TAD entrada, contendo informacoes de input
   * @return bool. true caso bem-sucedido. false, caso contrario
  */
  bool dadosInserirNovosRegistros(ENTRADA* entrada);
  
  /**
   * @brief Adiciona novo registro ao final do arquivo binario de dados
   * @param ARGS* args. TAD contendo informacoes necessarias para a operacao, assim como o registro a ser adicionado
   * @return bool. true caso bem-sucedido. false, caso contrario
  */
  bool dadosAtualizarRegistros(ARGS* args);
  
  /**
   * @brief Percorre sequencialmente o arquivo binario de dados e executa, por registro lido, a funcao desejada
   * @param ENTRADA* entrada. TAD entrada contendo as entradas informadas
   * @param void (*ftnPorRegistro). Ponteiro de funcao a ser utilizada por registro
   * @return ARGS*. Ponteiro para o TAD args*, contendo todas as informacoes coletadas
  */
  ARGS* dadosVarreduraSequencialArquivoBinario(ENTRADA* entrada, void (*ftnPorRegistro)());

  /**
   * @brief Inicializa o TAD cabecalho contendo informacoes do cabecalho do arquivo binario de dados
   * @return CABECALHO*. Ponteiro para o TAD cabecalho de dados
  */
  CABECALHO* dadosCabecalhoInit();
  
  /**
   * @brief Inicializa o TAD registro contendo informacoes do registro do arquivo binario de dados
   * @return REGISTRO*. ponteiro para o TAD registro de dados
  */
  REGISTRO* dadosRegistroInit();

#endif