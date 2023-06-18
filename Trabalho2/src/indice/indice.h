#ifndef INDICE_H
  #define INDICE_H

  #define TAM_MAX_CABECALHO_INDICEB 76
  #define ORDEM_ARVOREB 5

  #include "../utilitarios/entrada.h"
  #include "../utilitarios/funcoesFornecidas.h"
  #include "../utilitarios/erros.h"
  #include "../dados/dados.h"

  typedef struct cabecalho_indice_ CABECALHO_INDICE;

  bool indiceCriarArquivoBinario(ENTRADA* entrada);
  ARGS* indiceVarreduraSequencialArquivoBinario(ENTRADA* entrada, void (*ftnPorRegistro)());
  bool indiceBuscaBinariaArquivoBinario(ENTRADA* entrada, ARGS* args, void (*ftnPorRegistro)());
  bool indiceArvoreBsCriarArquivoBinario(ENTRADA* entrada);
  bool indiceRegistroApagar(REGISTRO_INDICE** registroIndice, char* tipoDado);
  bool indiceCabecalhoApagar(CABECALHO_INDICE** cabecalho_indice);
  CABECALHO_INDICE* indiceCabecalhoInit() ;

#endif