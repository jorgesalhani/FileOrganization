#include <stdio.h>
#include <stdlib.h>
#include "arvoreBinaria.h"
#include "item.h"

typedef struct no_ NO;

struct no_ {
    ITEM* item;
    NO* esquerda;
    NO* direita;
};

struct arvore_binaria_ {
    char* campoIndexado;
    int32_t qtdReg;
    NO* raiz;
};

bool arvoreBinariaExiste(ARVORE_BINARIA* arvoreBinaria) {
    return arvoreBinaria != NULL ? true : false;
}

bool arvoreBinariaNoExiste(NO* no) {
    return no != NULL ? true : false;
}

bool arvoreBinariaNoDeletar(NO** no) {
    if (no == NULL || !arvoreBinariaNoExiste(*no)) return false;
    if (itemExiste((*no)->item)) itemDeletar(&((*no)->item));
    (*no)->direita = NULL;
    (*no)->esquerda = NULL;
    free(*no);
    *no = NULL;
    no = NULL;
    return true;
}

bool arvoreBinariaDeletarAux(NO* no) {
    if (!arvoreBinariaNoExiste(no)) return false;

    arvoreBinariaDeletarAux(no->esquerda);
    arvoreBinariaDeletarAux(no->direita);
    
    arvoreBinariaNoDeletar(&no);
    return true;
}

NO* arvoreBinariaNoCriar(ITEM* item) {
    if (!itemExiste(item)) return NULL;

    NO* no = (NO*) malloc(sizeof(NO));
    if (!arvoreBinariaNoExiste(no)) return NULL;

    no->direita = NULL;
    no->esquerda = NULL;
    no->item = item;
    return no;
}

ARVORE_BINARIA* arvoreBinariaCriar(char* campoIndexado) {
    if (!dadosCampoIndexadoValido(campoIndexado)) return NULL;

    ARVORE_BINARIA* arvoreBinaria = (ARVORE_BINARIA*) malloc(sizeof(ARVORE_BINARIA));
    if (!arvoreBinariaExiste(arvoreBinaria)) return NULL;

    arvoreBinaria->raiz = NULL;
    arvoreBinaria->campoIndexado = campoIndexado;

    return arvoreBinaria;
}


int32_t arvoreBinariaObterQtdReg(ARVORE_BINARIA* arvoreBinaria) {
    if (!arvoreBinariaExiste(arvoreBinaria)) return -1;
    return arvoreBinaria->qtdReg;
}

char* arvoreBinariaObterCampoIndexado(ARVORE_BINARIA* arvoreBinaria) {
    if (!arvoreBinariaExiste(arvoreBinaria)) return NULL;
    return arvoreBinaria->campoIndexado;
}

int stringCompareMarcaCelular(char* str1, char* str2) {
    char str1c[TAMANHO_MARCA_CELULAR+5];
    char str2c[TAMANHO_MARCA_CELULAR+5];

    strcpy(str1c, str1);
    strcpy(str2c, str2);

    str1c[TAMANHO_MARCA_CELULAR] = '\0';
    str2c[TAMANHO_MARCA_CELULAR] = '\0';

    return strcmp(str1c, str2c);
}

bool arvoreBinariaOrdenarPorCampo(
    ARVORE_BINARIA* arvoreBinaria, NO* raiz, NO* anterior, NO* novoNo, 
    char* campoIndexado, int indiceCampoEscolhido
) {
    if (!arvoreBinariaNoExiste(raiz) || !arvoreBinariaNoExiste(novoNo) || !arvoreBinariaNoExiste(anterior)) return false;

    DADOS* dadosNoRaiz = itemObterDados(raiz->item);
    DADOS* dadosNo = itemObterDados(novoNo->item);
    DADOS* dadosNoAnterior = itemObterDados(anterior->item);

    void* valorCampoEscolhidoRaiz = dadosObterCampoIndexado(dadosNoRaiz, campoIndexado);
    void* valorCampoEscolhido = dadosObterCampoIndexado(dadosNo, campoIndexado);
    void* valorCampoEscolhidoAnterior = dadosObterCampoIndexado(dadosNoAnterior, campoIndexado);

    switch (indiceCampoEscolhido) {
        case 0:;
            int32_t* campoIntRaiz = (int32_t*) valorCampoEscolhidoRaiz;
            int32_t* campoInt = (int32_t*) valorCampoEscolhido;
            int32_t* campoIntAnterior = (int32_t*) valorCampoEscolhidoAnterior;
            if (*campoInt > *campoIntRaiz) {
                if (!arvoreBinariaNoExiste(raiz->direita)) {
                    raiz->direita = novoNo;
                    return true;
                } else {
                    return arvoreBinariaOrdenarPorCampo(arvoreBinaria, raiz->direita, raiz, novoNo, campoIndexado, indiceCampoEscolhido);
                }
            } else 
            if (*campoInt < *campoIntRaiz) {
                if (!arvoreBinariaNoExiste(raiz->esquerda)) {
                        raiz->esquerda = novoNo;
                        return true;
                } else {
                    return arvoreBinariaOrdenarPorCampo(arvoreBinaria, raiz->esquerda, raiz, novoNo, campoIndexado, indiceCampoEscolhido);
                }
            } else {
                int64_t novoByteOffset = itemObterByteOffset(novoNo->item);
                int64_t byteOffset = itemObterByteOffset(raiz->item);
                if (novoByteOffset <= byteOffset) {
                    novoNo->esquerda = raiz->esquerda;
                    raiz->esquerda = novoNo;

                    if (raiz == anterior) {
                        novoNo->esquerda = raiz->esquerda;
                        raiz->esquerda = novoNo;
                    }

                    return true;
                } else {
                    novoNo->direita = raiz->direita;
                    novoNo->esquerda = raiz;

                    if (raiz == anterior) {
                        raiz->direita = NULL;
                        arvoreBinaria->raiz = novoNo;
                        return true;
                    }

                    if (*campoIntAnterior > *campoIntRaiz) anterior->esquerda = novoNo;
                    else anterior->direita = novoNo;
                    raiz->direita = NULL;
                    return true;
                }
            }
            break;

        case 1:;
            char* campoStrRaiz = (char*) valorCampoEscolhidoRaiz;
            char* campoStr = (char*) valorCampoEscolhido;
            char* campoStrAnterior = (char*) valorCampoEscolhidoAnterior;

            // if (dadosObterEnumCampo(campoIndexado) == 3) {
            //     char campoStrRaizCp[TAMANHO_MARCA_CELULAR+5] = "";
            //     char campoStrCp[TAMANHO_MARCA_CELULAR+5] = "";
            //     char campoStrAnteriorCp[TAMANHO_MARCA_CELULAR+5] = "";

            //     strcpy(campoStrRaizCp, campoStrRaiz);
            //     strcpy(campoStrCp, campoStr);
            //     strcpy(campoStrAnteriorCp, campoStrAnterior);
                
            //     campoStrRaizCp[TAMANHO_MARCA_CELULAR] = '\0';
            //     campoStrCp[TAMANHO_MARCA_CELULAR] = '\0';
            //     campoStrAnteriorCp[TAMANHO_MARCA_CELULAR] = '\0';

            //     campoStrRaiz = campoStrRaizCp;
            //     campoStr = campoStrCp;
            //     campoStrAnterior = campoStrAnteriorCp;
            // }

            if (strcmp(campoStr, campoStrRaiz) > 0) {
                if (!arvoreBinariaNoExiste(raiz->direita)) {
                    raiz->direita = novoNo;
                    return true;
                } else {
                    return arvoreBinariaOrdenarPorCampo(arvoreBinaria, raiz->direita, raiz, novoNo, campoIndexado, indiceCampoEscolhido);
                }
            } else 
            if (strcmp(campoStr, campoStrRaiz) < 0) {
                if (!arvoreBinariaNoExiste(raiz->esquerda)) {
                        raiz->esquerda = novoNo;
                        return true;
                } else {
                    return arvoreBinariaOrdenarPorCampo(arvoreBinaria, raiz->esquerda, raiz, novoNo, campoIndexado, indiceCampoEscolhido);
                }
            } else {
                int64_t novoByteOffset = itemObterByteOffset(novoNo->item);
                int64_t byteOffset = itemObterByteOffset(raiz->item);
                if (novoByteOffset <= byteOffset) {
                    novoNo->esquerda = raiz->esquerda;
                    raiz->esquerda = novoNo;

                    if (raiz == anterior) {
                        novoNo->esquerda = raiz->esquerda;
                        raiz->esquerda = novoNo;
                    }

                    return true;
                } else {
                    novoNo->direita = raiz->direita;
                    novoNo->esquerda = raiz;

                    if (raiz == anterior) {
                        raiz->direita = NULL;
                        arvoreBinaria->raiz = novoNo;
                        return true;
                    }

                    if (strcmp(campoStrRaiz, campoStrAnterior) < 0) anterior->esquerda = novoNo;
                    else anterior->direita = novoNo;
                    raiz->direita = NULL;
                    return true;
                }
            }
            break;

        default:
            break;
    }
}

bool arvoreBinariaAdicionar(ARVORE_BINARIA* arvoreBinaria, DADOS* dados, METADADOS* metadados, int32_t chave, int64_t byteOffset, char* campoIndexado) {
    if (!arvoreBinariaExiste(arvoreBinaria) || !dadosExiste(dados) || !metadadosExiste(metadados)) return false;

    int indiceCampoEscolhido = dadosObterNumeroCampoIndexado(campoIndexado);
    void* valorCampoEscolhido = dadosObterCampoIndexado(dados, campoIndexado);

    ITEM* item = itemCriar(chave, byteOffset, valorCampoEscolhido, dados, metadados);
    if (!itemExiste(item)) return false;
    
    NO* novoNo = arvoreBinariaNoCriar(item);
    if (!arvoreBinariaNoExiste(novoNo)) return false;

    if (!arvoreBinariaNoExiste(arvoreBinaria->raiz)) {
        arvoreBinaria->raiz = novoNo;
        return true;
    }

    arvoreBinariaOrdenarPorCampo(arvoreBinaria, arvoreBinaria->raiz, arvoreBinaria->raiz, novoNo, campoIndexado, indiceCampoEscolhido);

    return true;
}

bool arvoreBinariaDeletar(ARVORE_BINARIA** arvoreBinaria) {
    if (arvoreBinaria == NULL || !arvoreBinariaExiste(*arvoreBinaria)) return false;

    arvoreBinariaDeletarAux((*arvoreBinaria)->raiz);
    free(*arvoreBinaria);
    *arvoreBinaria = NULL;
    arvoreBinaria = NULL;
    return true;
}

bool arvoreBinariaArmazenarRegistrosOrdenados(ARVORE_BINARIA* arvoreBinaria, TABELA* tabela, CABECALHO* cabecalho, char* tipoDado) {
    if (!arvoreBinariaExiste(arvoreBinaria) || !tabelaExiste(tabela) || !cabecalhoExiste(cabecalho)) return false;

    char* campoIndexado = arvoreBinariaObterCampoIndexado(arvoreBinaria);

    int32_t nroRegArq = cabecalhoObterNroRegArq(cabecalho);
    int64_t byteOffset = cabecalhoObterTamanhoRegistro(cabecalho);
    int32_t chave = 0;
    while(nroRegArq--) {
        DADOS* dados = tabelaLerArmazenarDado(tabela);
        METADADOS* metadados = tabelaLerArmazenarMetadado(dados);
        if (!dadosExiste(dados) || !metadadosExiste(metadados)) {
            continue;
        }
        
        char charAux = tabelaLerChar(tabela);
        int numPreenchimento = 0;
        if (charAux == '$') numPreenchimento = 1;
        while (charAux == '$') {
            charAux = tabelaLerChar(tabela);
            numPreenchimento += 1;
        };
        dadosMetadadosAtualizarTamanhoPreenchimento(metadados, numPreenchimento);

        int64_t proxByteOffset = dadosMetadadosObterTamanhoRegistro(dados, metadados);
        if (!dadosValorIndexadoValido(dados, campoIndexado, tipoDado) || dadosRemovido(dados)) {
            dadosDeletar(&dados);
            dadosMetadadosDeletar(&metadados);
        } else {
            arvoreBinariaAdicionar(arvoreBinaria, dados, metadados, chave, byteOffset, arvoreBinaria->campoIndexado);
            chave++;
        }
        byteOffset += proxByteOffset;
        tabelaResetLeituraArquivoBinario(tabela, byteOffset);
    }

    arvoreBinaria->qtdReg = chave;

    return true;
}


bool armazenarRegistroOrdemCrescente(
    ITEM* item, INDICE* indice, ARVORE_BINARIA* arvoreBinaria, char* tipoDado
) {
    if (!indiceExiste(indice) || !arvoreBinariaExiste(arvoreBinaria)) return false;

    DADOS_INDICE_INTEIRO* dadosIndiceInteiro = NULL;
    DADOS_INDICE_STRING* dadosIndiceString = NULL;

    char* campoIndexado = arvoreBinariaObterCampoIndexado(arvoreBinaria);
    int indiceCampoEscolhido = dadosObterNumeroCampoIndexado(campoIndexado);

    DADOS* dados = itemObterDados(item);
    METADADOS* metadados = itemObterMetadados(item);

    void* valorCampoEscolhido = dadosObterCampoIndexado(dados, campoIndexado);

    switch (indiceCampoEscolhido) {
        case 0:;
            int32_t* campoInt = (int32_t*) valorCampoEscolhido;
            dadosIndiceInteiro = dadosIndiceInteiroCriar(tipoDado, *campoInt, itemObterByteOffset(item));
            
            if (*campoInt != -1) indiceInteiroAtualizarDados(indice, dadosIndiceInteiro);
            
            dadosIndiceInteiroDeletar(&dadosIndiceInteiro);
            break;
        case 1:;
            char* campoStr = (char*) valorCampoEscolhido;
            if (strlen(campoStr) == 0) break;
            char* campoTruncado = dadosIndiceTruncarString(campoStr);
            dadosIndiceString = dadosIndiceStringCriar(tipoDado, campoTruncado, itemObterByteOffset(item));

            if (campoTruncado[0] != '$') indiceStringAtualizarDados(indice, dadosIndiceString);

            dadosIndiceStringDeletar(&dadosIndiceString);
            free(campoTruncado);
            break;
        default:;
            break;
    }

    return true;
}

DADOS* noObterDados(NO* no) {
    if (!arvoreBinariaNoExiste(no)) return NULL;
    return itemObterDados(no->item);
}

METADADOS* noObterMetadados(NO* no) {
    if (!arvoreBinariaNoExiste(no)) return NULL;
    return itemObterMetadados(no->item);
}

bool arvoreBinariaArmazenarOrdemCrescenteAux(
    NO* no, INDICE* indice, ARVORE_BINARIA* arvoreBinaria, char* tipoDado
) {
    if (!arvoreBinariaNoExiste(no)) return false;
    arvoreBinariaArmazenarOrdemCrescenteAux(no->esquerda, indice, arvoreBinaria, tipoDado);
    armazenarRegistroOrdemCrescente(no->item, indice, arvoreBinaria, tipoDado);
    arvoreBinariaArmazenarOrdemCrescenteAux(no->direita, indice, arvoreBinaria, tipoDado);
    return true;
}

bool arvoreBinariaIndiceArmazenarRegistrosOrdemCrescente(INDICE* indice, ARVORE_BINARIA* arvoreBinaria, char* tipoDado) {
    if (!arvoreBinariaExiste(arvoreBinaria) || !indiceExiste(indice)) return false;
    arvoreBinariaArmazenarOrdemCrescenteAux(arvoreBinaria->raiz, indice, arvoreBinaria, tipoDado);
    return true;
}

bool arvoreBinariaBuscaAux(
    NO* raiz, TABELA* tabela, CABECALHO* cabecalho, char* campoIndexado, 
    char** listaCamposDeBusca, void** listaValoresDeBusca, int numeroParesCampoValor, 
    char** listaCamposDeAtualizacao, void** listaValoresDeAtualizacao, int numeroParesCampoValorAtualizacao,
    int* totalRegistros, int funcionalidade
) {
    if (!arvoreBinariaNoExiste(raiz) || listaCamposDeBusca == NULL || listaValoresDeBusca == NULL) {
      return false;
    } 
    
    int numeroCampo = dadosObterNumeroCampoIndexado(campoIndexado);

    DADOS* dados = itemObterDados(raiz->item);
    METADADOS* metadados = noObterMetadados(raiz);

    void* valor = dadosObterCampoIndexado(dados, campoIndexado);
    
    bool buscarEsquerda = true;

    int32_t* valorBuscaInt = NULL;
    int32_t* valorEncontradoInt = NULL;
    char* valorBuscaStr = "";
    char* valorEncontradoStr = "";

    switch (numeroCampo) {
        case 0:;
            valorBuscaInt = (int32_t*) valor;
            valorEncontradoInt = (int32_t*) dadosObterCampoIndexado(dados, campoIndexado);
            if (*valorEncontradoInt > *valorBuscaInt) buscarEsquerda = false;
            break;

        case 1:;
            valorBuscaStr = (char*) valor;
            valorEncontradoStr = (char*) dadosObterCampoIndexado(dados, campoIndexado);
            if (strcmp(valorBuscaStr, valorEncontradoStr) > 0) buscarEsquerda = false;
            break;

        default:;
            break;
    }

    if (buscarEsquerda) {
        arvoreBinariaBuscaAux(
            raiz->esquerda, tabela, cabecalho,
            campoIndexado, listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValor, 
            listaCamposDeAtualizacao, listaValoresDeAtualizacao, numeroParesCampoValorAtualizacao,
            totalRegistros, funcionalidade);
    }
    
    bool correspondenciaCompleta = dadosBuscaCorrespondenciaCompleta(dados, listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValor);
  	if (correspondenciaCompleta) { 
      switch (funcionalidade) {
          case 4:; // Imprimir
              dadosImprimir(dados, metadados);
              break;

          case 5:; //Remover
            // arvoreBinariaRemoverDadoLogicamente(tabela, raiz);
						tabelaRemoverRegistroLogicamente(tabela, cabecalho, itemObterByteOffset(raiz->item));
            break;

          case 7:; // Atualizar
              if (listaCamposDeAtualizacao == NULL || listaValoresDeAtualizacao == NULL || numeroParesCampoValorAtualizacao == 0) return false;

              int64_t tamRegistro = dadosMetadadosObterTamanhoRegistro(dados, metadados);
              dadosAtualizaCamposEspecificados(dados, metadados, listaCamposDeAtualizacao, listaValoresDeAtualizacao, numeroParesCampoValorAtualizacao);
              METADADOS* metadadosAtualizados = tabelaLerArmazenarMetadado(dados);
              
              int64_t tamRegistroAtualizado = dadosMetadadosObterTamanhoRegistro(dados, metadadosAtualizados);
              int64_t byteOffset = itemObterByteOffset(raiz->item);
              tabelaResetLeituraArquivoBinario(tabela, byteOffset);

              if (tamRegistro == tamRegistroAtualizado) {
                  tabelaAtualizarDados(tabela, dados, metadadosAtualizados, '|');
              }
              if (tamRegistro > tamRegistroAtualizado) {
                  tabelaAtualizarDados(tabela, dados, metadadosAtualizados, '|');
                  tabelaResetLeituraArquivoBinario(tabela, byteOffset+tamRegistroAtualizado-1);
                  char preenchimentoReg = '$';
                  for (int k = tamRegistroAtualizado; k < tamRegistro; k++) {
                      tabelaEscreverChar(tabela, &preenchimentoReg);    
                  }

              } 
              if (tamRegistro < tamRegistroAtualizado) {
                  char removido = '1';
                  tabelaEscreverChar(tabela, &removido);

                  int64_t byteOffsetFinal = cabecalhoObterProxByteOffset(cabecalho);
                  tabelaResetLeituraArquivoBinario(tabela, byteOffsetFinal);

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
              
              dadosMetadadosDeletar(&metadadosAtualizados);
              break;
            
            default:
                break;
        }
        
        int contadorRegistrosEncontrados = *totalRegistros;
        contadorRegistrosEncontrados++;
        *totalRegistros = contadorRegistrosEncontrados;
    }

    arvoreBinariaBuscaAux(
        raiz->direita, tabela, cabecalho,
        campoIndexado, listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValor, 
        listaCamposDeAtualizacao, listaValoresDeAtualizacao, numeroParesCampoValorAtualizacao,
        totalRegistros, funcionalidade);
    return true;
}

int arvoreBinariaBusca(
    ARVORE_BINARIA* arvoreBinaria, TABELA* tabela, CABECALHO* cabecalho, char* campoIndexado, 
    char** listaCamposDeBusca, void** listaValoresDeBusca, int numeroParesCampoValor, 
    char** listaCamposDeAtualizacao, void** listaValoresDeAtualizacao, int numeroParesCampoValorAtualizacao,
    int funcionalidade) 
{
    if (!arvoreBinariaExiste(arvoreBinaria) || listaCamposDeBusca == NULL || listaValoresDeBusca == NULL) return -1;

    int totalRegistros = 0;
    arvoreBinariaBuscaAux(
        arvoreBinaria->raiz, tabela, cabecalho, campoIndexado, 
        listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValor, 
        listaCamposDeAtualizacao, listaValoresDeAtualizacao, numeroParesCampoValorAtualizacao,
        &totalRegistros, funcionalidade
    );
    return totalRegistros;
}