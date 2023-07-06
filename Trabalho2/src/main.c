#include "utilitarios/entrada.h"
#include "utilitarios/erros.h"
#include "dados/dados.h"
#include "indice/indice.h"

int main() {
  ENTRADA* entrada = lerEntradas();
  
  int funcionalidade = entradaObterFuncionalidade(entrada);
  bool sucesso = true;

  if (funcionalidade == 1) {
    dadosCriarArquivoBinario(entrada);
  } else 
  if (funcionalidade == 2) {
    dadosImprimirArquivoBinario(entrada);
  } else 
  if (funcionalidade == 3) {
    indiceCriarArquivoBinario(entrada);
  } else 
  if (funcionalidade == 4) {
    dadosBuscarPorCampos(entrada, dadosImprimirRegistro);
  } else 
  if (funcionalidade == 5) {
    sucesso = dadosBuscarPorCampos(entrada, dadosRemoverRegistroLogicamente);
    if (sucesso) {
      binarioNaTela(entradaObterArquivoEntrada(entrada));
      indiceCriarArquivoBinario(entrada);
    }
  } else 
  if (funcionalidade == 6) {
    sucesso = dadosInserirNovosRegistros(entrada);
    if (sucesso) {
      binarioNaTela(entradaObterArquivoEntrada(entrada));
      indiceCriarArquivoBinario(entrada);
    }
  } else 
  if (funcionalidade == 7) {
    sucesso = dadosBuscarPorCampos(entrada, dadosAtualizarRegistros);
    if (sucesso) {
      binarioNaTela(entradaObterArquivoEntrada(entrada));
      indiceCriarArquivoBinario(entrada);
    }
  } else {
    erroModo();
  }

  entradaApagar(&entrada);
  return 0;
}