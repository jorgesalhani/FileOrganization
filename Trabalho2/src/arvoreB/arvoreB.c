#include "arvoreB.h"

struct chave_ {
  int32_t C;
  int64_t Pr;
};

struct pagina_ {
  int contadorDeOcupacao;
  int32_t* P;
  CHAVE** chaves;
  int32_t nivel;
  int32_t n;
};

struct cabecalho_indiceB_ {
  char status;
  int32_t noRaiz;
  int32_t RNNproxNo;
  int32_t nroNiveis;
  int32_t nroChaves;
};

PAGINA* arvoreBPaginaInit() {
  PAGINA* pagina = (PAGINA*) malloc(sizeof(PAGINA));
  if (pagina == NULL) return NULL;
  pagina->contadorDeOcupacao = 0;
  pagina->n = 0;
  pagina->nivel = 1;
  pagina->P = (int32_t*) malloc(sizeof(int32_t)*(ORDEM_ARVOREB-1));

  int numChaves = ORDEM_ARVOREB-2;
  pagina->chaves = (CHAVE**) malloc(sizeof(CHAVE*)*numChaves);
  for (int i = 0; i < numChaves; i++) {
    (pagina->chaves)[i] = (CHAVE*) malloc(sizeof(CHAVE));
  }

  return pagina;
}

CHAVE* arvoreBchaveInit() {
  CHAVE* chave = (CHAVE*) malloc(sizeof(CHAVE));
  chave->C = -1;
  chave->Pr = -1;
  return chave;
}

CABECALHO_INDICEB* cabecalhoIndiceBInit() {
  CABECALHO_INDICEB* cabecalho = (CABECALHO_INDICEB*) malloc(sizeof(CABECALHO_INDICEB));
  if (cabecalho == NULL) return NULL;
  cabecalho->noRaiz = -1;
  cabecalho->nroChaves = 0;
  cabecalho->nroNiveis = 0;
  cabecalho->status = '0';
  return cabecalho;
}

bool arvoreBCabecalhoApagar(CABECALHO_INDICEB** cabecalho) {
  if (cabecalho == NULL || *cabecalho == NULL) return false;
  free(*cabecalho);
  *cabecalho = NULL;
  cabecalho = NULL;
}

bool arvoreBPaginaApagar(PAGINA** pagina) {
  if (pagina == NULL || *pagina == NULL) return false;
  int numChaves = ORDEM_ARVOREB-2;
  for (int i = 0; i < numChaves; i++) {
    free(&((*pagina)->chaves)[i]);
    (*pagina)->chaves[i] = NULL;
  }
  free(*pagina);
  *pagina = NULL;
  pagina = NULL;
  return true;
}

bool arvoreBArmazenarPagina(PAGINA* pagina, FILE* arquivoBinarioIndice) {
  if (arquivoBinarioIndice == NULL || pagina == NULL) return false;
  fwrite(&pagina->nivel, sizeof(int32_t), 1, arquivoBinarioIndice);
  fwrite(&pagina->n, sizeof(int32_t), 1, arquivoBinarioIndice);
  fwrite(&(pagina->P)[0], sizeof(int64_t), 1, arquivoBinarioIndice);
  for (int i = 0; i < ORDEM_ARVOREB-1; i++) {
    fwrite(&(pagina->chaves)[i]->C, sizeof(int32_t), 1, arquivoBinarioIndice);
    fwrite(&(pagina->chaves)[i]->Pr, sizeof(int64_t), 1, arquivoBinarioIndice);
    fwrite(&(pagina->P)[i+1], sizeof(int64_t), 1, arquivoBinarioIndice);
  }
  return true;
}

bool arvoreBArmazenarCabecalho(CABECALHO_INDICEB* cabecalho, FILE* arquivoBinarioIndice) {
  if (cabecalho == NULL || arquivoBinarioIndice == NULL) return false;
  resetLeituraDeArquivo(arquivoBinarioIndice, 0);
  fwrite(&cabecalho->status, sizeof(char), 1, arquivoBinarioIndice);
  fwrite(&cabecalho->noRaiz, sizeof(int32_t), 1, arquivoBinarioIndice);
  fwrite(&cabecalho->RNNproxNo, sizeof(int32_t), 1, arquivoBinarioIndice);
  fwrite(&cabecalho->nroNiveis, sizeof(int32_t), 1, arquivoBinarioIndice);
  fwrite(&cabecalho->nroChaves, sizeof(int32_t), 1, arquivoBinarioIndice);

  int32_t tamCab = sizeof(cabecalho->status) + sizeof(cabecalho->noRaiz) +
    sizeof(cabecalho->RNNproxNo) + sizeof(cabecalho->nroNiveis) + sizeof(cabecalho->nroChaves);
  
  int32_t tamLixo = TAM_MAX_CABECALHO_INDICEB - tamCab;
  char lixo = '$';
  fwrite(&lixo, sizeof(char), tamLixo, arquivoBinarioIndice);
  fflush(arquivoBinarioIndice);
  return true;
}

bool indiceArvoreBLerCabecalhoDoArquivoBinario(FILE* arquivoBinarioIndice, CABECALHO_INDICEB* cabecalho) {
  if (cabecalho == NULL || arquivoBinarioIndice == NULL) return false;

  resetLeituraDeArquivo(arquivoBinarioIndice, 0);
  fread(&cabecalho->status, sizeof(char), 1, arquivoBinarioIndice);

  // char statusEmProgresso = '0';
  // resetLeituraDeArquivo(arquivoBinarioIndice, 0);
  // fwrite(&statusEmProgresso, sizeof(char), 1, arquivoBinarioIndice);
  fread(&cabecalho->noRaiz, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&cabecalho->RNNproxNo, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&cabecalho->nroNiveis, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&cabecalho->nroChaves, sizeof(int32_t), 1, arquivoBinarioIndice);
  return true;
}

bool indiceArvoreBLerPaginaDoArquivoBinario(FILE* arquivoBinarioIndice, PAGINA* pagina) {
  if (arquivoBinarioIndice == NULL || pagina == NULL) return false;

  fread(&pagina->nivel, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&pagina->n, sizeof(int32_t), 1, arquivoBinarioIndice);
  fread(&(pagina->P)[0], sizeof(int64_t), 1, arquivoBinarioIndice);
  for (int i = 0; i < ORDEM_ARVOREB-1; i++) {
    fread(&(pagina->chaves)[i]->C, sizeof(int32_t), 1, arquivoBinarioIndice);
    fread(&(pagina->chaves)[i]->Pr, sizeof(int64_t), 1, arquivoBinarioIndice);
    fread(&(pagina->P)[i+1], sizeof(int64_t), 1, arquivoBinarioIndice);
  }
  return true;
}

void indiceArvoreBVarreduraDeBusca(ARGS* args) {
  if (args == NULL) return;

}


bool indiceArvoreBCriarArquivoBinario(ENTRADA* entrada) {
  if (entrada == NULL) return false;

  CABECALHO_INDICEB* cabecalhoIndice = cabecalhoIndiceBInit();
  if (cabecalhoIndice == NULL) {
    return false;
  }

  char* arquivoIndice = entradaObterArquivoIndice(entrada);
  FILE* arquivoBinarioIndice = fopen(arquivoIndice, MODO_ESCRITA_ARQUIVO);
  if (arquivoBinarioIndice == NULL) {
    erroGenerico();
    return false;
  }

  ARGS* args = dadosVarreduraSequencialArquivoBinario(entrada, indiceArvoreBVarreduraDeBusca);
  args->arquivoIndiceBin = arquivoBinarioIndice;
  args->cabecalhoIndiceArvoreB = cabecalhoIndice;
  

  // args->arquivoIndiceBin = arquivoBinarioIndice;

  // cabecalhoIndice->qtdReg = args->arvoreBinaria->totalRegistros;
  // indiceArmazenarCabecalho(cabecalhoIndice, arquivoBinarioIndice);

  // args->arvoreBinaria->ordem = emOrdem;
  // dadosVarreduraCompletaArvoreBinaria(args, args->arvoreBinaria->raiz, indiceArmazenarRegistro);


  // cabecalhoIndice->status = '1';
  // indiceArmazenarCabecalho(cabecalhoIndice, arquivoBinarioIndice);

  // fclose(arquivoBinarioIndice);

  // indiceCabecalhoApagar(&cabecalhoIndice);
  // argsApagar(&args);
  
  // binarioNaTela(arquivoIndice);

  return true;
}
