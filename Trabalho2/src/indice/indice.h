#ifndef INDICE_H
  #define INDICE_H

  #define TAM_MAX_CABECALHO_INDICEB 76
  #define ORDEM_ARVOREB 5

  #include "../utilitarios/entrada.h"
  #include "../utilitarios/funcoesFornecidas.h"
  #include "../utilitarios/erros.h"
  #include "../dados/dados.h"

  typedef struct cabecalho_indice_ CABECALHO_INDICE;
  typedef struct chave_arvore_B_estrela_ CHAVE;
  typedef struct cabecalho_arvore_B_estrela_ CABECALHOBESTRELA;
  typedef struct pagina_arvore_B_estrela_ BTPAGE;

  /**
   * @brief Criar arquivo binario de indices linear
   * @param ENTRADA* entrada. TAD entrada contendo as entradas informadas
   * @return bool. true caso bem-sucedido. false caso contrario
  */
  bool indiceCriarArquivoBinario(ENTRADA* entrada);
  
  /**
   * @brief Executar varredura por todo arquivo de indice linear sequencialmente e aplicar funcao desejada sobre cada registro de dados
   * @param ENTRADA* entrada. TAD entrada contendo informacoes de input
   * @param void (*ftnPorRegistro). Funcao a ser aplicada sobre o registro encontrado
   * @return ARGS*. ponteiro para TAD args com informacoes atuais da verredura
  */
  ARGS* indiceVarreduraSequencialArquivoBinario(ENTRADA* entrada, void (*ftnPorRegistro)());
  
  /**
   * @brief Executar busca de registro por meio de busca binaria utilizando arquivo de indices linear e aplicar funcao desejada
   * @param ENTRADA* entrada. TAD entrada contendo informacoes de input
   * @param ARGS* args. TAD com informacoes sobre registro a ser buscado
   * @param void (*ftnPorRegistro). Funcao a ser aplicada sobre o registro encontrado
   * @return bool. true caso bem-sucedido. false caso contrario
  */
  bool indiceBuscaBinariaArquivoBinario(ENTRADA* entrada, ARGS* args, void (*ftnPorRegistro)());
  
  /**
   * @brief Criar arquivo binario de indices em modo arvore B
   * @param ENTRADA* entrada. TAD entrada contendo informacoes de input
   * @return bool. true caso bem-sucedido. false caso contrario
  */
  bool indiceArvoreBsCriarArquivoBinario(ENTRADA* entrada);
  
  /**
   * @brief Liberar memoria reservada para registro de indices
   * @param REGISTRO_INDICE** registroIndice. Endereco do ponteiro para registro alocado
   * @return bool. true caso bem-sucedido. false caso contrario
  */
  bool indiceRegistroApagar(REGISTRO_INDICE** registroIndice, char* tipoDado);
  
  /**
   * @brief Liberar memoria reservada para cabecalho de indices
   * @param CABECALHO_INDICE** cabecalho_indice. Endereco do ponteiro para cabecalho alocado
   * @return bool. true caso bem-sucedido. false caso contrario
  */
  bool indiceCabecalhoApagar(CABECALHO_INDICE** cabecalho_indice);
  
  /**
   * @brief Criar cabecalho do arquivo de indices Linear ou arvore B
   * @return CABECALHO_INDICE*. Ponteiro para TAD contendo informacoes de cabecalho
  */
  CABECALHO_INDICE* indiceCabecalhoInit() ;
  
  /**
   * @brief Inicializar informacoes relativas a arvore B e armazenar no TAD args
   * @param ARGS* args. TAD args para insercao das informacoes da arvore B
   * @return bool. true caso bem-sucedido. false caso contrario
  */
  bool argsInitInformativoParaArvoreB(ARGS* args);
  
  /**
   * @brief Buscar registro no arquivo de dados por meio do arquivo de indices em arvore B e aplicar funcao desejada
   * @param ENTRADA* entrada. TAD contendo informacoes de input
   * @param ARGS* args. TAD contendo informacoes do registro a ser buscado
   * @param void (*ftnPorBusca). Funcao a ser aplicada sobre o registro encontrado no arquivo de indices
   * @return bool. true caso bem-sucedido. false caso contrario
  */
  bool indiceBuscaPorIndiceArvoreB(ENTRADA* entrada, ARGS* args, void (*ftnPorBusca)());

  CABECALHOBESTRELA *ArvoreBCriarCabecalho();
  void imprimirCabecalhoArvoreB(CABECALHOBESTRELA *cabecalho);
  void removerCabecalhoArvoreB(CABECALHOBESTRELA** cabecalho);
  CABECALHOBESTRELA *lerCabecalhoArvoreB(FILE *indiceBinario);
  BTPAGE *criarPaginaArvoreB();
  BTPAGE *preencherPaginaArvoreB(int nivel, int contadorDeOcupacao, CHAVE chaves[], int Pfinal);
  void removerPaginaArvoreB(BTPAGE** pagina);
  int arvoreBObterNivel(BTPAGE* pagina);
  int arvoreBObterOcupacao(BTPAGE* pagina);
  int *arvoreBObterP(BTPAGE* pagina);
  CHAVE *arvoreBObterChaves(BTPAGE* pagina);
  void acessarPaginaArvoreB(FILE *indiceBinario, BTPAGE* pagAux, int32_t RRN);
  void imprimirPaginaArvoreB(BTPAGE *pagina) ;
  FILE *abrirIndiceArvoreB(char *nomeIndice);

  void fecharIndiceArvoreB(FILE* indiceBinario, bool error);
  void atualizarPagina(FILE* indiceBinario, BTPAGE* pagina, int32_t RRN);
  int novaPagina(FILE* indiceBinario, CABECALHOBESTRELA* cabecalho, int32_t nivel, int32_t byteOffsetAtual) ;
  int64_t ArvoreBBuscar(FILE* indiceBinario, int RRN, int64_t chave);
  void ordenarChaves(CHAVE chaves[], int size);
  void shiftPagina(FILE* indiceBinario, int RRN, CHAVE chave, int32_t nivel, int32_t taxaDeOcupacao);
  CHAVE* split1para2(FILE* indiceBinario, CABECALHOBESTRELA* cabecalho, int RRN, CHAVE chave, int32_t nivel);
  CHAVE* split(FILE* indiceBinario, CABECALHOBESTRELA* cabecalho, int RRN, CHAVE chave, int32_t nivel);
  CHAVE* inserirArvoreB(FILE* indiceBinario, CABECALHOBESTRELA* cabecalho, int RRN, CHAVE chave);

#endif