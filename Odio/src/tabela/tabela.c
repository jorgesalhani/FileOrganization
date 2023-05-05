#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tabela.h"
#include "../utilitarios/erros.h"
#include "../indice/indice.h"

/********************
 * ESTRUTURA DE DADOS
 * ******************
*/

struct tabela_ {
    char* nome;
    FILE* arquivoBinario;
};

/*********************
 * FUNCOES UTILITARIAS
 * *******************
*/

bool nomeValido(char* nome) {
    return strlen(nome) > 0 ? true : false;
}

bool arquivoExiste(FILE* arquivo) {
    return arquivo != NULL ? true : false;
}

bool tabelaExiste(TABELA* tabela) {
    return tabela != NULL ? true : false;
}

bool delimitadorValido(char delimitadorCampos) {
    return delimitadorCampos == '|' ? true : false;
}

CABECALHO* tabelaLerArmazenarCabecalho(TABELA* tabela) {
  if (!tabelaExiste(tabela) || !arquivoExiste(tabela->arquivoBinario)) return NULL;

  FILE* binarioDados = tabela->arquivoBinario;

  char statusAux = '0';
  int32_t nroRegRem = 0;
  int32_t nroRegArq = 1; 
  int64_t proxByteOffset = 0;
  int32_t idAux, numArtAux, i = 0;
  
  fread(&statusAux, sizeof(char), 1, binarioDados);
  fread(&proxByteOffset, sizeof(int64_t), 1, binarioDados);
  fread(&nroRegArq, sizeof(int32_t), 1, binarioDados);
  fread(&nroRegRem, sizeof(int32_t), 1, binarioDados);

  if (statusAux == '0') {
    erroGenerico();
    return NULL;
  }
  
  if(nroRegArq == 0) {
    erroSemRegistros();
    return NULL;
  }

  CABECALHO* cabecalho = cabecalhoCriar(statusAux, proxByteOffset, nroRegArq, nroRegRem);
  if (!cabecalhoExiste(cabecalho)) return NULL;
  return cabecalho;
}

bool verificarSeRemovido(TABELA* tabela, char removido) {
  FILE* binarioDados = tabela->arquivoBinario;

  char lixo[256];
  char charAux = '0';
  if(removido == '1') {
    fread(lixo, sizeof(char), 30, binarioDados);
    while(charAux != '#') {
      fread(&charAux, sizeof(char), 1, binarioDados);
    }
    charAux = '\0';
    return true;
  }
  return false;
}

DADOS* tabelaLerArmazenarDado(TABELA* tabela) {
  if (!tabelaExiste(tabela) || !arquivoExiste(tabela->arquivoBinario)) return NULL;

  FILE* binarioDados = tabela->arquivoBinario;

  char removidoAux = '0';
  fread(&removidoAux, sizeof(char), 1, binarioDados);
  if (!removidoValido(removidoAux)) return NULL;
  bool registroRemovido = verificarSeRemovido(tabela, removidoAux);
  if (registroRemovido) return NULL;

  int32_t idAux, numArtAux;
  char charAux = '0';
  char dataAux[TAMANHO_DATA_CRIME+1] = ""; 
  char marcaAux[TAMANHO_MARCA_CELULAR+1] = "";
  char descricaoAux[256] = "";
  char lugarAux[256] = ""; 

  fread(&idAux, sizeof(int32_t), 1, binarioDados);
  fread(dataAux, sizeof(char), TAMANHO_DATA_CRIME , binarioDados);
  fread(&numArtAux, sizeof(int32_t), 1, binarioDados);
  fread(marcaAux, sizeof(char), TAMANHO_MARCA_CELULAR, binarioDados);

  int lugarAuxIndice = 0;
  while(1) {
    fread(&lugarAux[lugarAuxIndice], sizeof(char), 1, binarioDados);
    if(lugarAux[lugarAuxIndice] == '|') {
      lugarAux[lugarAuxIndice] = '\0';
      break;
    }
    lugarAuxIndice++;
  }

  int descricaoAuxIndice = 0;
  while(1) {
    fread(&descricaoAux[descricaoAuxIndice], sizeof(char), 1, binarioDados);
    if(descricaoAux[descricaoAuxIndice] == '|') {
      descricaoAux[descricaoAuxIndice] = '\0';
      break;
    }
    descricaoAuxIndice++;
  }
  fread(&charAux, sizeof(char), 1, binarioDados);

  DADOS* dados = dadosCriar(idAux, dataAux, numArtAux, marcaAux, lugarAux, descricaoAux, removidoAux);
  return dados;
}

METADADOS* tabelaLerArmazenarMetadado(DADOS* dados) {
  if (!dadosExiste(dados)) return NULL;
  char* descricaoCrime = dadosObterDescricaoCrime(dados);
  char* lugarCrime = dadosObterLugarCrime(dados);
  METADADOS* metadados = dadosCriarMetadados((int)strlen(descricaoCrime), (int)(strlen(lugarCrime)));
  if (!metadadosExiste(metadados)) return NULL;
  return metadados;
}

/********************
 * FUNCOES PRINCIPAIS
 * ******************
*/

bool modoAbrirArquivoValido(char* modoAberturaArquivo) {
  return (
    strcmp(modoAberturaArquivo, "wb+") == 0 ||
    strcmp(modoAberturaArquivo, "rb") == 0
  ) ? true : false;
}
    
TABELA* tabelaCriar(char* nome, char* modoAberturaArquivo) {
    if (!nomeValido(nome) || !modoAbrirArquivoValido(modoAberturaArquivo)) return NULL;
    FILE* arquivoBinario = fopen(nome, modoAberturaArquivo);
    if (!arquivoExiste(arquivoBinario)) return NULL;

    TABELA* tabela = (TABELA*) malloc(sizeof(TABELA));
    if (!tabelaExiste(tabela)) return NULL;

    tabela->arquivoBinario = arquivoBinario;
    tabela->nome = nome;
    return tabela;
}

bool tabelaAtualizarCabecalho(TABELA* tabela, CABECALHO* cabecalho) {
    if (!tabelaExiste(tabela) || !arquivoExiste(tabela->arquivoBinario) || 
        !cabecalhoExiste(cabecalho)) return false;

    FILE* arquivo = tabela->arquivoBinario;

    char status = cabecalhoObterStatus(cabecalho);
    int64_t byteOffset = cabecalhoObterProxByteOffset(cabecalho);
    int32_t nroRegArq = cabecalhoObterNroRegArq(cabecalho);
    int32_t nroRegRem = cabecalhoObterNroRegRem(cabecalho);

    fwrite(&status, sizeof(char), 1, arquivo);
    fwrite(&byteOffset, sizeof(int64_t), 1, arquivo);
    fwrite(&nroRegArq, sizeof(int32_t), 1, arquivo);
    fwrite(&nroRegRem, sizeof(int32_t), 1, arquivo);

    return true;
}

bool tabelaAtualizarDados(TABELA* tabela, DADOS* dados, METADADOS* metadados, 
                          char delimitadorCampos) {
    if (!delimitadorValido(delimitadorCampos)) return false;
    if (!tabelaExiste(tabela) || !arquivoExiste(tabela->arquivoBinario) || 
        !dadosExiste(dados)) return false;

    FILE* arquivo = tabela->arquivoBinario;

    char removido = dadosObterRemovido(dados);
    int32_t idCrime = dadosObterIdCrime(dados);
    char* dataCrime = dadosObterDataCrime(dados);
    int32_t numeroArtigo = dadosObterNumeroArtigo(dados);
    char* marcaCelular = dadosObterMarcaCelular(dados);
    char delimitador = dadosObterDelimitadorRegistro(dados);

    int64_t tamanhoLugarCrime = dadosMetadadosObterTamanhoLugarCrime(metadados);
    char lugarCrime[tamanhoLugarCrime];
    for (int64_t i = 0; i < tamanhoLugarCrime; i++) lugarCrime[i] = dadosObterLugarCrime(dados)[i];

    int64_t tamanhoDescricaoCrime = dadosMetadadosObterTamanhoDescricaoCrime(metadados);
    char descricaoCrime[tamanhoDescricaoCrime];
    for (int64_t i = 0; i < tamanhoDescricaoCrime; i++) descricaoCrime[i] = dadosObterDescricaoCrime(dados)[i];

    fwrite(&removido, sizeof(char), 1, arquivo);
    
    fwrite(&idCrime, sizeof(int32_t), 1, arquivo);

    fwrite(dataCrime, sizeof(char), TAMANHO_DATA_CRIME, arquivo);

    fwrite(&numeroArtigo, sizeof(int32_t), 1, arquivo);

    fwrite(marcaCelular, sizeof(char), TAMANHO_MARCA_CELULAR, arquivo);

    fwrite(lugarCrime, sizeof(lugarCrime), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);

    fwrite(descricaoCrime, sizeof(descricaoCrime), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);    
    
    fwrite(&delimitador, sizeof(char), 1, arquivo);
    return true;

}

char* tabelaObterNomeArquivo(TABELA* tabela) {
    if (!tabelaExiste(tabela)) return NULL;
    return tabela->nome;
}

bool tabelaFecharArquivo(TABELA* tabela) {
    if (!tabelaExiste(tabela) || !arquivoExiste(tabela->arquivoBinario)) return false;
    fclose(tabela->arquivoBinario);
    tabela->arquivoBinario = NULL;
    return true;
}

bool tabelaDeletar(TABELA** tabela, bool manterArquivo) {
    if (tabela == NULL || !tabelaExiste(*tabela)) return false;
    
    if (arquivoExiste((*tabela)->arquivoBinario)) fclose((*tabela)->arquivoBinario);
    
    if (!manterArquivo) remove((*tabela)->nome);
    free(*tabela);
    *tabela = NULL;
    tabela = NULL;
    return true;
}

TABELA* tabelaCriarBinario(char* nomeEntrada, char* nomeSaida) {
    FILE *crimesDados = fopen(nomeEntrada, "r");
    if (crimesDados == NULL) {
        erroGenerico();
        return NULL;
    }

    char str[256] = "";
    TABELA* tabela = tabelaCriar(nomeSaida, "wb+");

    CABECALHO* cabecalho = cabecalhoCriar('0', 1, 2, 3);
    DADOS* dados = dadosCriar(0, "$$$$$$$$$$", 0, "$$$$$$$$$$$$", "", "", '0');
    METADADOS* metadados = dadosCriarMetadados(0, 0);

    int32_t tamanhoRegistroDados = 0;
    int64_t tamanhoRegistroCabecalho = 0;
    int32_t novoNroRegArq = 0;
    int32_t novoNroRegRem = 0;
    

    tabelaAtualizarCabecalho(tabela, cabecalho);
    
    fgets(str, 256, crimesDados);
    while(fgets(str, 256, crimesDados)) {
        if(str[0] == '\n')
        continue;

        int j = 0, k = 0;
        char token[256] = "";
        for(int i = 0; i < strlen(str); i++) {
        if(str[i] == '\n' || str[i] == ',' || (i == strlen(str)-1 && feof(crimesDados))) {
            // printf("%s\n", str);
            
            if(i == strlen(str)-1 && feof(crimesDados)) {
            token[j] = str[i];
            }
            
            switch(k) {
            case 0:
                if(strcmp(token, "") == 0) {
                    erroGenerico(); 
                    return NULL;
                }
                // printf("%d\n", atol(token));
                dadosAtualizarIdCrime(dados, atoi(token));
                break;
            case 1:
                dadosAtualizarDataCrime(dados, token);
                break;
            case 2:
                if(strlen(token) == 0)
                dadosAtualizarNumeroArtigo(dados, -1);
                else
                dadosAtualizarNumeroArtigo(dados, atoi(token));
                break;
            case 3:
                dadosAtualizarLugarCrime(dados, token, metadados);
                break;
            case 4:
                dadosAtualizarDescricaoCrime(dados, token, metadados);
                break;
            case 5:
                dadosAtualizarMarcaCelular(dados, token);
            }
            k++;
            memset(token, 0, strlen(token));
            j = 0;
            continue;
        }
        token[j] = str[i];
        //printf("%c\n", token[j]);
        j++;
        }
    
        tabelaAtualizarDados(tabela, dados, metadados, '|');

        novoNroRegArq++;
        
        tamanhoRegistroDados += dadosMetadadosObterTamanhoRegistro(dados, metadados);

        if (dadosObterRemovido(dados) == '1') {
            novoNroRegRem = (int32_t)(novoNroRegRem + 1);
        }

        // dadosImprimir(dados);
        // printf("=====\n");
    }

    tamanhoRegistroCabecalho = cabecalhoObterTamanhoRegistro(cabecalho);
    int64_t novoProxByteOffset = (int64_t)(tamanhoRegistroDados + tamanhoRegistroCabecalho);

    fseek(tabela->arquivoBinario, 0, SEEK_SET);

    cabecalhoAtualizarStatus(cabecalho, '1');
    cabecalhoAtualizarProxByteOffset(cabecalho, novoProxByteOffset);
    cabecalhoAtualizarNroRegArq(cabecalho, novoNroRegArq);
    cabecalhoAtualizarNroRegRem(cabecalho, novoNroRegRem);

    tabelaAtualizarCabecalho(tabela, cabecalho);

    fclose(crimesDados);

    dadosMetadadosDeletar(&metadados);
    dadosDeletar(&dados);
    cabecalhoDeletar(&cabecalho);

    return tabela;
}

TABELA* tabelaLerImprimirBinario(char* entrada) {
  TABELA* tabela = tabelaCriar(entrada, "rb");
  if(!tabelaExiste(tabela)) {
    erroGenerico();
    return tabela;
  }
    
  CABECALHO* cabecalho = tabelaLerArmazenarCabecalho(tabela);
  if (!cabecalhoExiste(cabecalho)) return tabela;

  int32_t nroRegArq = cabecalhoObterNroRegArq(cabecalho);

  while(nroRegArq--) {

    DADOS* dados = tabelaLerArmazenarDado(tabela);
    if (!dadosExiste(dados)) continue;

    METADADOS* metadados = tabelaLerArmazenarMetadado(dados);
    
    dadosImprimir(dados, metadados);

    dadosDeletar(&dados);
    dadosMetadadosDeletar(&metadados);
  }

  cabecalhoDeletar(&cabecalho);
  return tabela;
}

void lerEntradaNumeroParCriterio(int* numeroParCriterio) {
  scanf("%d", numeroParCriterio);
}

void lerEntradasBuscaPorCampos(char** listaCamposDeBusca, void** listaValoresDeBusca, int numeroParesCampoValor) {
  for (int campo_i = 0; campo_i < numeroParesCampoValor; campo_i++) {
    listaCamposDeBusca[campo_i] = (char*) malloc(sizeof(char)*50);
    scanf("%s", listaCamposDeBusca[campo_i]);
    int indiceCampoEscolhido = dadosObterNumeroCampoIndexado(listaCamposDeBusca[campo_i]);
    switch (indiceCampoEscolhido) {
      case 0:
        listaValoresDeBusca[campo_i] = (int32_t*) malloc(sizeof(int32_t));
        scanf("%d", (int32_t*) listaValoresDeBusca[campo_i]);
        break;
      case 1:
        listaValoresDeBusca[campo_i] = (char*) malloc(sizeof(char)*50);
        scan_quote_string((char*) listaValoresDeBusca[campo_i]);
        break;
      default:
        break;
    }
  }
}

bool verificarSeCriterioDeBuscaIndexado(char** listaCamposDeBusca, int numeroParesCampoValor, char* campoIndexado) {
  for (int i = 0; i < numeroParesCampoValor; i++) {
    if (strcmp(listaCamposDeBusca[i], campoIndexado) == 0) return true;
  }
  return false;
}

bool tabelaLerImprimirBuscaPorCampos(
    TABELA* tabela, char* campoIndexado, 
    char* tipoDado, char* nomeArquivoIndice, int numeroCamposBuscados
) {
    if (!tabelaExiste(tabela) || (!tipoDadoInteiroValido(tipoDado) && !tipoDadoStringValido(tipoDado))) {
        erroGenerico();
        return false;
    }
    
    if (!dadosCampoIndexadoValido(campoIndexado)) {
        erroGenerico();
        return false;
    }

    INDICE* indice = indiceCriar(nomeArquivoIndice, "rb");
    if (!indiceExiste(indice)) {
        erroGenerico();
        return false;
    }

    CABECALHO_INDICE* cabecalhoIndice = indiceLerArmazenarCabecalho(indice);
    if (!cabecalhoIndiceExiste(cabecalhoIndice)) {
        indiceDeletar(&indice, true);
        erroGenerico();
        return false;
    }

    int32_t indiceTotalRegistros = cabecalhoIndiceObterQtdReg(cabecalhoIndice);
    int64_t byteOffset = 0;

    ARVORE_BINARIA* arvoreBinaria = arvoreBinariaCriar(campoIndexado);
    if (!arvoreBinariaExiste(arvoreBinaria)) {
      indiceDeletar(&indice, true);
      cabecalhoIndiceDeletar(&cabecalhoIndice);
      erroGenerico();
      return NULL;
    }

    int32_t chave = 0;
    while (indiceTotalRegistros--) {
        DADOS_INDICE_INTEIRO* dadosIndiceInteiro = NULL;
        DADOS_INDICE_STRING* dadosIndiceString = NULL;
        
        if (tipoDadoInteiroValido(tipoDado)) {
            dadosIndiceInteiro = indiceLerArmazenarDadosInteiro(indice, tipoDado);
            byteOffset = dadosIndiceInteiroObterByteOffset(dadosIndiceInteiro);
        } else {
            dadosIndiceString = indiceLerArmazenarDadosString(indice, tipoDado);
            byteOffset = dadosIndiceStringObterByteOffset(dadosIndiceString);
        }

        fseek(tabela->arquivoBinario, byteOffset, SEEK_SET);
        DADOS* dados = tabelaLerArmazenarDado(tabela);
        METADADOS* metadados = tabelaLerArmazenarMetadado(dados);
        if (!dadosExiste(dados)) continue;        
        
        
        arvoreBinariaAdicionar(arvoreBinaria, dados, metadados, chave, byteOffset, campoIndexado);

        dadosIndiceInteiroDeletar(&dadosIndiceInteiro);
        dadosIndiceStringDeletar(&dadosIndiceString);
        chave++;
    }

    for (int i = 0; i < numeroCamposBuscados; i++) {
      int numeroParesCampoValor;
      lerEntradaNumeroParCriterio(&numeroParesCampoValor);

      char** listaCamposDeBusca = (char**) malloc(sizeof(char*)*numeroParesCampoValor);
      void** listaValoresDeBusca = (void**) malloc(sizeof(void*)*numeroParesCampoValor);

      lerEntradasBuscaPorCampos(listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValor);

      bool buscaIndexada = verificarSeCriterioDeBuscaIndexado(listaCamposDeBusca, numeroParesCampoValor, campoIndexado);
      if (buscaIndexada) {
        indiceImprimirBuscaBinaria(arvoreBinaria, listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValor);
      }
      else tabelaImprimirBuscaSequencial(tabela, listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValor);

      for (int i = 0; i < numeroParesCampoValor; i++) {
        free(listaCamposDeBusca[i]);
        listaCamposDeBusca[i] = NULL;
        free(listaValoresDeBusca[i]);
        listaValoresDeBusca[i] = NULL;
      }
      listaCamposDeBusca = NULL;
      listaValoresDeBusca = NULL;
    }

    arvoreBinariaDeletar(&arvoreBinaria);

    cabecalhoIndiceDeletar(&cabecalhoIndice);

    indiceDeletar(&indice, true);
    return true;
}

TABELA* tabelaLerImprimirBusca(
  char* nomeArquivoEntrada, char* campoIndexado, char* tipoDado, 
  char* nomeArquivoIndice, int numeroCamposBuscados
) {
  TABELA* tabela = tabelaCriar(nomeArquivoEntrada, "rb");
  if (!tabelaExiste(tabela)) {
    erroGenerico();
    return tabela;
  }

  tabelaLerImprimirBuscaPorCampos(tabela, campoIndexado, tipoDado, nomeArquivoIndice, numeroCamposBuscados);

  return tabela;
}

bool tabelaImprimirBuscaSequencial(TABELA* tabela, char** listaCamposDeBusca, void** listaValoresDeBusca, int numeroParesCampoValor) {

}