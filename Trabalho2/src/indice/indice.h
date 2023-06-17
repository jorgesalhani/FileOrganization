#ifndef INDICE_H
  #define INDICE_H

  #include "../utilitarios/entrada.h"
  #include "../utilitarios/funcoesFornecidas.h"
  #include "../utilitarios/erros.h"
  #include "../dados/dados.h"

  typedef struct registro_indice_ REGISTRO_INDICE;
  typedef struct cabecalho_indice_ CABECALHO_INDICE;

  bool indiceCriarArquivoBinario(ENTRADA* entrada);
  ARGS* indiceVarreduraSequencialArquivoBinario(ENTRADA* entrada, void (*ftnPorRegistro)());
  bool indiceBuscaBinariaArquivoBinario(ENTRADA* entrada, ARGS* args, void (*ftnPorRegistro)());
  bool indiceArvoreBsCriarArquivoBinario(ENTRADA* entrada);

#endif