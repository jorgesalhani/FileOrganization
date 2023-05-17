#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tabela.h"
#include "../utilitarios/funcoesFornecidas.h"
#include "../busca/arvoreBinaria.h"

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

  fseek(tabelaObterPonteiroArquivo(tabela), 0, SEEK_SET);
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
    fread(lixo, sizeof(char), 1, binarioDados);
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
  // if (!removidoValido(removidoAux)) return NULL;
  // bool registroRemovido = verificarSeRemovido(tabela, removidoAux);
  // if (registroRemovido) return NULL;

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

FILE* tabelaObterPonteiroArquivo(TABELA* tabela) {
    if (!tabelaExiste(tabela)) return NULL;
    return tabela->arquivoBinario;
}

/********************
 * FUNCOES PRINCIPAIS
 * ******************
*/

bool modoAbrirArquivoValido(char* modoAberturaArquivo) {
  return (
    strcmp(modoAberturaArquivo, "wb+") == 0 ||
    strcmp(modoAberturaArquivo, "rb") == 0 ||
    strcmp(modoAberturaArquivo, "rb+") == 0
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

    fseek(arquivo, 0, SEEK_SET);
    fwrite(&status, sizeof(char), 1, arquivo);
    fwrite(&byteOffset, sizeof(int64_t), 1, arquivo);
    fwrite(&nroRegArq, sizeof(int32_t), 1, arquivo);
    fwrite(&nroRegRem, sizeof(int32_t), 1, arquivo);
    fflush(tabela->arquivoBinario);
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

    char marcaCelularAux[TAMANHO_MARCA_CELULAR+5];

    strcpy(marcaCelularAux, marcaCelular);

    dadosAtualizarMarcaCelular(dados, marcaCelularAux);
    char* marcaCelularPreenchida = dadosObterMarcaCelular(dados);

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

    fwrite(marcaCelularPreenchida, sizeof(char), TAMANHO_MARCA_CELULAR, arquivo);

    fwrite(lugarCrime, sizeof(lugarCrime), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);

    fwrite(descricaoCrime, sizeof(descricaoCrime), 1, arquivo);
    fwrite(&delimitadorCampos, sizeof(char), 1, arquivo);    
    
    fwrite(&delimitador, sizeof(char), 1, arquivo);
    fflush(tabela->arquivoBinario);
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
    if (!dadosExiste(dados) || dadosRemovido(dados)) continue;

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

void lerEntradasCampoValor(char** listaCampos, void** listaValores, int numeroPares) {
  for (int campo_i = 0; campo_i < numeroPares; campo_i++) {
    listaCampos[campo_i] = (char*) malloc(sizeof(char)*50);
    scanf("%s", listaCampos[campo_i]);
    int indiceCampoEscolhido = dadosObterNumeroCampoIndexado(listaCampos[campo_i]);
    switch (indiceCampoEscolhido) {
      case 0:
        listaValores[campo_i] = (int32_t*) malloc(sizeof(int32_t));
        scanf("%d", (int32_t*) listaValores[campo_i]);
        break;
      case 1:
        listaValores[campo_i] = (char*) malloc(sizeof(char)*200);
        scan_quote_string((char*) listaValores[campo_i]);
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

ARVORE_BINARIA* obterArvoreBinariaIndices(
  TABELA* tabela, char* nomeArquivoIndice, char** listaCamposDeBusca, void** listaValoresDeBusca, 
  char* campoIndexado, char* tipoDado, int numeroParesCampoValor
) {
  if (listaCamposDeBusca == NULL || listaValoresDeBusca == NULL) return NULL;

  INDICE* indice = indiceCriar(nomeArquivoIndice, "rb");
  if (!indiceExiste(indice)) {
    erroGenerico();
    return NULL;
  }

  CABECALHO_INDICE* cabecalhoIndice = indiceLerArmazenarCabecalho(indice);
  if (!cabecalhoIndiceExiste(cabecalhoIndice)) {
    indiceDeletar(&indice, true);
    erroGenerico();
    return NULL;
  }

  ARVORE_BINARIA* arvoreBinaria = arvoreBinariaCriar(campoIndexado);
  if (!arvoreBinariaExiste(arvoreBinaria)) {
    indiceDeletar(&indice, true);
    cabecalhoIndiceDeletar(&cabecalhoIndice);
    erroGenerico();
    return NULL;
  }

  int32_t indiceTotalRegistros = cabecalhoIndiceObterQtdReg(cabecalhoIndice);
  int64_t byteOffset = 0;

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
    if (!dadosExiste(dados) || dadosRemovido(dados)) {
      dadosDeletar(&dados);
      dadosMetadadosDeletar(&metadados);
      dadosIndiceInteiroDeletar(&dadosIndiceInteiro);
      dadosIndiceStringDeletar(&dadosIndiceString);
      continue;        
    }
    
    
    arvoreBinariaAdicionar(arvoreBinaria, dados, metadados, chave, byteOffset, campoIndexado);

    dadosIndiceInteiroDeletar(&dadosIndiceInteiro);
    dadosIndiceStringDeletar(&dadosIndiceString);
    chave++;
  }

  cabecalhoIndiceDeletar(&cabecalhoIndice);

  indiceDeletar(&indice, true);

  return arvoreBinaria;
}

void tabelaRemoverRegistroLogicamente(TABELA* tabela, DADOS* dados, METADADOS* metadados) {
  FILE* arquivoBinario = tabelaObterPonteiroArquivo(tabela);
  fseek(arquivoBinario, (-1)*dadosMetadadosObterTamanhoRegistro(dados, metadados), SEEK_CUR);
  char removido = '1';
  fwrite(&removido, sizeof(char), 1, arquivoBinario);  
}

int tabelaBuscaImprimir(TABELA* tabela, char* campoIndexado, char** listaCamposDeBusca, void** listaValoresDeBusca, int numeroParesCampoValor, int32_t nroRegArq, int funcionalidade) {
  if (!tabelaExiste(tabela) || listaCamposDeBusca == NULL || listaValoresDeBusca == NULL) return 0;

  int contadorRegistros = nroRegArq;

  int totalRegistrosEncontrados = 0;
  CABECALHO* cabecalho = tabelaLerArmazenarCabecalho(tabela);
  while (contadorRegistros--) {

    DADOS* dados = tabelaLerArmazenarDado(tabela);
    if (!dadosExiste(dados)) continue;
    if (dadosRemovido(dados)) {
      dadosDeletar(&dados);
      continue;
    }

    METADADOS* metadados = tabelaLerArmazenarMetadado(dados);
    if (!metadadosExiste(metadados)) continue;
    
    bool correspondenciaCompleta = dadosBuscaCorrespondenciaCompleta(dados, listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValor);
    if (correspondenciaCompleta) {
      switch(funcionalidade) {
        case 4:
          dadosImprimir(dados, metadados);
          break;
        case 5: //FUNCIONALIDADE 5
          tabelaRemoverRegistroLogicamente(tabela, dados, metadados);
          break;
        default:
          erroGenerico();
          exit(1);
      }
      totalRegistrosEncontrados++;
    }

    dadosDeletar(&dados);
    dadosMetadadosDeletar(&metadados);
  }

  cabecalhoDeletar(&cabecalho);

  return totalRegistrosEncontrados;
}

bool tabelaResetLeituraArquivoBinario(TABELA* tabela, int64_t byteOffset) {
  if (!tabelaExiste(tabela) || !arquivoExiste(tabela->arquivoBinario)) return false;
  fseek(tabela->arquivoBinario, byteOffset, SEEK_SET);
  return true;
}

bool tabelaAtualizarDadoComoRemovido(TABELA* tabela, int64_t byteOffset) {
  if (!tabelaExiste(tabela) || !arquivoExiste(tabela->arquivoBinario)) return false;
  tabelaResetLeituraArquivoBinario(tabela, byteOffset);
  char removido = '1';
  fwrite(&removido, sizeof(char), 1, tabela->arquivoBinario);
  return true;
}

int tabelaLerImprimirBuscaCampo(TABELA* tabela, CABECALHO* cabecalho, ARVORE_BINARIA** arvoreBinaria, char* campoIndexado, 
  char* tipoDado, char* nomeArquivoIndice, int32_t nroRegArq, int numeroBuscaAtual, int funcionalidade
) {
  int numeroParesCampoValor;
  lerEntradaNumeroParCriterio(&numeroParesCampoValor);
  if(funcionalidade == 4) {
    printf("Resposta para a busca %d\n", numeroBuscaAtual);
  }

  char** listaCamposDeBusca = (char**) malloc(sizeof(char*)*numeroParesCampoValor);
  void** listaValoresDeBusca = (void**) malloc(sizeof(void*)*numeroParesCampoValor);

  lerEntradasCampoValor(listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValor);

  bool buscaIndexada = verificarSeCriterioDeBuscaIndexado(listaCamposDeBusca, numeroParesCampoValor, campoIndexado);
  int totalRegistros = 0;
  if (buscaIndexada) {
    if (!arvoreBinariaExiste(*arvoreBinaria)) {
      *arvoreBinaria = obterArvoreBinariaIndices(
        tabela, nomeArquivoIndice, listaCamposDeBusca, listaValoresDeBusca, 
        campoIndexado, tipoDado, numeroParesCampoValor
      );
    }

    totalRegistros = arvoreBinariaBusca(
      *arvoreBinaria, tabela, cabecalho, campoIndexado, 
      listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValor,
      NULL, NULL, 0, funcionalidade
      );
  }
  else totalRegistros = tabelaBuscaImprimir(tabela, campoIndexado, listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValor, nroRegArq, funcionalidade);

  if(funcionalidade == 5) {
    cabecalhoAtualizarNroRegRem(cabecalho, cabecalhoObterNroRegRem(cabecalho)+totalRegistros);
    tabelaAtualizarCabecalho(tabela, cabecalho);  
  }

  tabelaResetLeituraArquivoBinario(tabela, 0);

  for (int i = 0; i < numeroParesCampoValor; i++) {
    free(listaCamposDeBusca[i]);
    listaCamposDeBusca[i] = NULL;
    free(listaValoresDeBusca[i]);
    listaValoresDeBusca[i] = NULL;
  }
  free(listaCamposDeBusca);
  listaCamposDeBusca = NULL;
  free(listaValoresDeBusca);
  listaValoresDeBusca = NULL;

  return totalRegistros;
}

TABELA* tabelaBusca(
  char* nomeArquivoEntrada, char* campoIndexado, char* tipoDado, 
  char* nomeArquivoIndice, int numeroCamposBuscados, int funcionalidade
) {
  TABELA* tabela = tabelaCriar(nomeArquivoEntrada, "rb");
  if (!tabelaExiste(tabela)) {
    erroGenerico();
    return tabela;
  }

  if (!tabelaExiste(tabela) || (!tipoDadoInteiroValido(tipoDado) && !tipoDadoStringValido(tipoDado))) {
        erroGenerico();
        return false;
  }
    
  if (!dadosCampoIndexadoValido(campoIndexado)) {
      erroGenerico();
      return false;
  }

  CABECALHO* cabecalho = tabelaLerArmazenarCabecalho(tabela);
  if (!cabecalhoExiste(cabecalho)) return tabela;

  int32_t nroRegArq = cabecalhoObterNroRegArq(cabecalho);

  ARVORE_BINARIA* arvoreBinaria = NULL;
  for (int i = 0; i < numeroCamposBuscados; i++) {
    int numeroBuscaAtual = i+1;
    int totalRegistrosEncontrados = 0;
    totalRegistrosEncontrados = tabelaLerImprimirBuscaCampo(tabela, cabecalho, &arvoreBinaria, campoIndexado, tipoDado, nomeArquivoIndice, nroRegArq, numeroBuscaAtual, funcionalidade);
    if (totalRegistrosEncontrados <= 0 && funcionalidade == 4) erroSemRegistros();
  }

  arvoreBinariaDeletar(&arvoreBinaria);

  cabecalhoDeletar(&cabecalho);

  return tabela;
}

bool liberarParesCampoValor(char*** listaCamposDeBusca, void*** listaValoresDeBusca, int total) {
  if (listaCamposDeBusca == NULL || listaValoresDeBusca == NULL) return false;

  char** listaCampos = *listaCamposDeBusca;
  void** listaValores = *listaValoresDeBusca;

  for (int i = 0; i < total; i++) {
    free(listaCampos[i]);
    listaCampos[i] = NULL;
    free(listaValores[i]);
    listaValores[i] = NULL;
  }

  free(listaCampos);
  listaCampos = NULL;
  listaCamposDeBusca = NULL;

  free(listaValores);
  listaValores = NULL;
  listaValoresDeBusca = NULL;

  return true;
}

bool tabelaEscreverChar(TABELA* tabela, char* valor) {
  if (!tabelaExiste(tabela)) return false;
  fwrite(valor, sizeof(char), 1, tabela->arquivoBinario);
  fflush(tabela->arquivoBinario);
  return true;
}

TABELA* tabelaLerAtualizar(
  char* nomeArquivoEntrada, char* campoIndexado, char* tipoDado, 
  char* nomeArquivoIndice, int numeroAtualizacoes
) {
  /**
   * Inicializar tabela (arquivo binario de dados) em modo leitura
   * e cabecalho do arquivo de dados
  */
  TABELA* tabela = tabelaCriar(nomeArquivoEntrada, "rb+");
  if (!tabelaExiste(tabela)) {
    erroGenerico();
    return tabela;
  }

  INDICE* indice = indiceCriar(nomeArquivoIndice, "rb+");
  if (!indiceExiste(indice)){
    erroGenerico();
    return tabela;
  }

  CABECALHO_INDICE* cabecalhoIndice = indiceLerArmazenarCabecalho(indice);
  if (!cabecalhoIndiceExiste(cabecalhoIndice)) {
    erroGenerico();
    indiceDeletar(&indice, false);
    return tabela;
  }

  if (!cabecalhoIndiceConsistente(cabecalhoIndice)) {
    erroGenerico();
    indiceDeletar(&indice, false);
    cabecalhoIndiceDeletar(&cabecalhoIndice);
    tabelaDeletar(&tabela, false);
    return NULL;
  }


  if (!tabelaExiste(tabela) || (!tipoDadoInteiroValido(tipoDado) && !tipoDadoStringValido(tipoDado))) {
        erroGenerico();
        return false;
    }
    
  if (!dadosCampoIndexadoValido(campoIndexado)) {
      erroGenerico();
      return false;
  }

  CABECALHO* cabecalho = tabelaLerArmazenarCabecalho(tabela);
  if (!cabecalhoExiste(cabecalho)) return tabela;

  int32_t nroRegArq = cabecalhoObterNroRegArq(cabecalho);

  ARVORE_BINARIA* arvoreBinaria = NULL;

  /**
   * Inicializar busca para cada uma das n (= numeroAtualizacoes) 
   * atualizacoes a serem realizadas
  */
  for (int i = 0; i < numeroAtualizacoes; i++) {
    int totalRegistrosEncontrados = 0;

    /**
     * Para cada atualizacao serao informados 
     * m (= numeroParesCampoValorBusca) pares campo:valor para busca
     * p (= numeroParesCampoValorAtualizacao) pares campo:valor para atualizar 
     * 
     * Lendo os pares para os criterios de busca
    */
    int numeroParesCampoValorBusca;
    scanf("%d", &numeroParesCampoValorBusca);

    char** listaCamposDeBusca = (char**) malloc(sizeof(char*)*numeroParesCampoValorBusca);
    void** listaValoresDeBusca = (void**) malloc(sizeof(void*)*numeroParesCampoValorBusca);

    lerEntradasCampoValor(listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValorBusca);

    /**
     * Lendo os pares para atualizacao
    */
    int numeroParesCampoValorAtualizacao;
    scanf("%d", &numeroParesCampoValorAtualizacao);

    char** listaCamposDeAtualizacao = (char**) malloc(sizeof(char*)*numeroParesCampoValorAtualizacao);
    void** listaValoresDeAtualizacao = (void**) malloc(sizeof(void*)*numeroParesCampoValorAtualizacao);

    lerEntradasCampoValor(listaCamposDeAtualizacao, listaValoresDeAtualizacao, numeroParesCampoValorAtualizacao);


    /**
     * Verificar caso algum dos campos de busca seja o mesmo que
     * o campoIndexado, utilizado para construir o arquivo de indice
     * 
     * Se verdadeiro, executar busca binaria
    */
    bool buscaIndexada = verificarSeCriterioDeBuscaIndexado(listaCamposDeBusca, numeroParesCampoValorBusca, campoIndexado);
    if (buscaIndexada) {
      if (!arvoreBinariaExiste(arvoreBinaria)) {
          arvoreBinaria = obterArvoreBinariaIndices(
          tabela, nomeArquivoIndice, listaCamposDeBusca, listaValoresDeBusca, 
          campoIndexado, tipoDado, numeroParesCampoValorBusca
        );
      }

      totalRegistrosEncontrados = arvoreBinariaBusca(
        arvoreBinaria, tabela, cabecalho, campoIndexado, 
        listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValorBusca,
        listaCamposDeAtualizacao, listaValoresDeAtualizacao, numeroParesCampoValorAtualizacao, 7
        );
    }

    /**
     * Caso contrario, executar busca sequencial no arquivo binario de dados
    */
    else {
      int contadorRegistros = nroRegArq;

      int64_t byteOffset = cabecalhoObterTamanhoRegistro(cabecalho);
      tabelaResetLeituraArquivoBinario(tabela, byteOffset);
      while (contadorRegistros--) {
        DADOS* dados = tabelaLerArmazenarDado(tabela);
        if (!dadosExiste(dados)) continue;

        METADADOS* metadados = tabelaLerArmazenarMetadado(dados);
        if (!metadadosExiste(metadados)) continue;

        int64_t proxByteOffset = dadosMetadadosObterTamanhoRegistro(dados, metadados);
        if (!dadosRemovido(dados)) {
          bool correspondenciaCompleta = dadosBuscaCorrespondenciaCompleta(dados, listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValorBusca);
        
          /**
           * Caso registro de dados tenha correspondencia com os campos buscados
           * atualizar registro de dados (RAM)
           * atualizar arquivo de dados (append ou inplace)
           * atualizar arquivo de indice (criar novo indice)
          */
          if (correspondenciaCompleta) {
            int64_t tamRegistro = dadosMetadadosObterTamanhoRegistro(dados, metadados);
            dadosAtualizaCamposEspecificados(dados, metadados, listaCamposDeAtualizacao, listaValoresDeAtualizacao, numeroParesCampoValorAtualizacao);
            METADADOS* metadadosAtualizados = tabelaLerArmazenarMetadado(dados);
            
            int64_t tamRegistroAtualizado = dadosMetadadosObterTamanhoRegistro(dados, metadadosAtualizados);

            fseek(tabela->arquivoBinario, byteOffset, SEEK_SET);
            if (tamRegistro == tamRegistroAtualizado) {
              tabelaAtualizarDados(tabela, dados, metadadosAtualizados, '|');
            }
            if (tamRegistro > tamRegistroAtualizado) {
              tabelaAtualizarDados(tabela, dados, metadadosAtualizados, '|');
              tabelaResetLeituraArquivoBinario(tabela, byteOffset+tamRegistroAtualizado-1);
              char preenchimentoReg = '$';
              for (int k = tamRegistroAtualizado; k < tamRegistro; k++) {
                fwrite(&preenchimentoReg, sizeof(char), 1, tabela->arquivoBinario);
              }
              fflush(tabela->arquivoBinario);
            } 
            if (tamRegistro < tamRegistroAtualizado) {
              char removido = '1';
              fwrite(&removido, sizeof(char), 1, tabela->arquivoBinario);
              fflush(tabela->arquivoBinario);

              int64_t byteOffsetFinal = cabecalhoObterProxByteOffset(cabecalho);
              fseek(tabela->arquivoBinario, byteOffsetFinal, SEEK_SET);

              tabelaAtualizarDados(tabela, dados, metadadosAtualizados, '|');
              tabelaResetLeituraArquivoBinario(tabela, 0);

              byteOffsetFinal += dadosMetadadosObterTamanhoRegistro(dados, metadadosAtualizados);

              int32_t nroRem = cabecalhoObterNroRegRem(cabecalho);
              int32_t nroReg = cabecalhoObterNroRegArq(cabecalho);
              cabecalhoAtualizarProxByteOffset(cabecalho, byteOffsetFinal);
              cabecalhoAtualizarNroRegRem(cabecalho, nroRem+1);
              cabecalhoAtualizarNroRegArq(cabecalho, nroReg+1);

              tabelaAtualizarCabecalho(tabela, cabecalho);
            }

            totalRegistrosEncontrados++;
            dadosMetadadosDeletar(&metadadosAtualizados);
          }
        }

        byteOffset += proxByteOffset;
        tabelaResetLeituraArquivoBinario(tabela, byteOffset);

        dadosDeletar(&dados);
        dadosMetadadosDeletar(&metadados);
      }

    }

    tabelaResetLeituraArquivoBinario(tabela, 0);

    liberarParesCampoValor(&listaCamposDeBusca, &listaValoresDeBusca, numeroParesCampoValorBusca);
    liberarParesCampoValor(&listaCamposDeAtualizacao, &listaValoresDeAtualizacao, numeroParesCampoValorAtualizacao);

  }

  indiceDeletar(&indice, true);
  cabecalhoIndiceDeletar(&cabecalhoIndice);

  arvoreBinariaDeletar(&arvoreBinaria);

  cabecalhoDeletar(&cabecalho);

  return tabela;
}

bool tabelaArquivoIndisponivel(TABELA* tabela, CABECALHO* cabecalho) {
  FILE* arquivoBinario = tabelaObterPonteiroArquivo(tabela);
  char indisponivel = '0';
  char disponivelAux;
  if(cabecalhoObterStatus(cabecalho) == '0') {
    return false;
  }
  
  cabecalhoAtualizarStatus(cabecalho, indisponivel);
  fwrite(&indisponivel, sizeof(char), 1, arquivoBinario);
  tabelaResetLeituraArquivoBinario(tabela, 0);

  return true;
}