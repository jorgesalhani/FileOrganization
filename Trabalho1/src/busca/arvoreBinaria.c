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
    free(*no);
    *no = NULL;
    no = NULL;
    return true;
}

bool arvoreBinariaDeletarAux(NO* no) {
    if (!arvoreBinariaNoExiste(no)) return false;

    arvoreBinariaDeletarAux(no->direita);
    arvoreBinariaDeletarAux(no->esquerda);
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

ITEM* arvoreBinariaObterItem(ARVORE_BINARIA* arvoreBinaria, int32_t chave) {

}

char* arvoreBinariaObterCampoIndexado(ARVORE_BINARIA* arvoreBinaria) {
    if (!arvoreBinariaExiste(arvoreBinaria)) return NULL;
    return arvoreBinaria->campoIndexado;
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
        case 0:
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

                    if (campoIntAnterior > campoIntRaiz) anterior->esquerda = novoNo;
                    else anterior->direita = novoNo;
                    raiz->direita = NULL;
                    return true;
                }
            }
            break;

        case 1:
            char* campoStrRaiz = (char*) valorCampoEscolhidoRaiz;
            char* campoStr = (char*) valorCampoEscolhido;
            char* campoStrAnterior = (char*) valorCampoEscolhidoAnterior;
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
                if (!arvoreBinariaNoExiste(raiz->esquerda)) {
                    raiz->esquerda = novoNo;
                        return true;
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

ITEM* arvoreBinariaRemoverItem(ARVORE_BINARIA* arvoreBinaria, int32_t chave) {

}

bool arvoreBinariaDeletar(ARVORE_BINARIA** arvoreBinaria) {
    if (arvoreBinaria == NULL || !arvoreBinariaExiste(*arvoreBinaria)) return false;

    arvoreBinariaDeletarAux((*arvoreBinaria)->raiz);
    free(*arvoreBinaria);
    *arvoreBinaria = NULL;
    arvoreBinaria = NULL;
    return true;
}

bool arvoreBinariaArmazenarRegistrosOrdenados(ARVORE_BINARIA* arvoreBinaria, TABELA* tabela, CABECALHO* cabecalho) {
    if (!arvoreBinariaExiste(arvoreBinaria) || !tabelaExiste(tabela) || !cabecalhoExiste(cabecalho)) return false;

    int32_t nroRegArq = cabecalhoObterNroRegArq(cabecalho);
    int64_t byteOffset = cabecalhoObterTamanhoRegistro(cabecalho);
    int32_t chave = 0;
    while(nroRegArq--) {
        DADOS* dados = tabelaLerArmazenarDado(tabela);
        METADADOS* metadados = tabelaLerArmazenarMetadado(dados);
        if (!dadosExiste(dados)) continue;        
        arvoreBinariaAdicionar(arvoreBinaria, dados, metadados, chave, byteOffset, arvoreBinaria->campoIndexado);
        chave++;
        byteOffset += dadosMetadadosObterTamanhoRegistro(dados, metadados);
    }

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
        case 0:
            int32_t* campoInt = (int32_t*) valorCampoEscolhido;
            dadosIndiceInteiro = dadosIndiceInteiroCriar(tipoDado, *campoInt, itemObterByteOffset(item));
            
            if (*campoInt != -1) indiceInteiroAtualizarDados(indice, dadosIndiceInteiro);
            
            dadosIndiceInteiroDeletar(&dadosIndiceInteiro);
            break;
        case 1:
            char* campoStr = (char*) valorCampoEscolhido;
            if (strlen(campoStr) == 0) break;
            char* campoTruncado = dadosIndiceTruncarString(campoStr);
            dadosIndiceString = dadosIndiceStringCriar(tipoDado, campoTruncado, itemObterByteOffset(item));

            if (campoTruncado[0] != '$') indiceStringAtualizarDados(indice, dadosIndiceString);

            dadosIndiceStringDeletar(&dadosIndiceString);
            free(campoTruncado);
            break;
        default:
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

bool indiceArmazenarRegistrosOrdemCrescente(INDICE* indice, ARVORE_BINARIA* arvoreBinaria, char* tipoDado) {
    if (!arvoreBinariaExiste(arvoreBinaria) || !indiceExiste(indice)) return false;
    arvoreBinariaArmazenarOrdemCrescenteAux(arvoreBinaria->raiz, indice, arvoreBinaria, tipoDado);
    return true;
}

int arvoreBinariaImprimirBuscaAux(
    NO* raiz, char* campoIndexado, char** listaCamposDeBusca, 
    void** listaValoresDeBusca, int numeroParesCampoValor
) {
    if (!arvoreBinariaNoExiste(raiz) || listaCamposDeBusca == NULL || listaValoresDeBusca == NULL) return false;

    int numeroCampo = dadosObterNumeroCampoIndexado(campoIndexado);

    DADOS* dados = noObterDados(raiz);
    METADADOS* metadados = noObterMetadados(raiz);

    void* valor = dadosObterCampoIndexado(dados, campoIndexado);
    
    bool buscarEsquerda = true;

    int32_t* valorBuscaInt = NULL;
    int32_t* valorEncontradoInt = NULL;
    char* valorBuscaStr = NULL;
    char* valorEncontradoStr = NULL;

    switch (numeroCampo) {
        case 0:
            valorBuscaInt = (int32_t*) valor;
            valorEncontradoInt = (int32_t*) dadosObterCampoIndexado(dados, campoIndexado);
            if (*valorEncontradoInt > *valorBuscaInt) buscarEsquerda = false;
            break;

        case 1:
            valorBuscaStr = (char*) valor;
            valorEncontradoStr = (char*) dadosObterCampoIndexado(dados, campoIndexado);
            if (strcmp(valorBuscaStr, valorEncontradoStr) > 0) buscarEsquerda = false;
            break;

        default:
            break;
    }

    int totalRegistros = 0;
    if (buscarEsquerda) {
        arvoreBinariaImprimirBuscaAux(raiz->esquerda, campoIndexado, listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValor);
    }
    
    bool correspondenciaCompleta = dadosBuscaCorrespondenciaCompleta(dados, listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValor);
    if (correspondenciaCompleta) {
        dadosImprimir(dados, metadados);
        totalRegistros++;
    }

    arvoreBinariaImprimirBuscaAux(raiz->direita, campoIndexado, listaCamposDeBusca, listaValoresDeBusca, numeroParesCampoValor);
    return totalRegistros;
}

int arvoreBinariaImprimirBusca(
    ARVORE_BINARIA* arvoreBinaria, char* campoIndexado, char** listaCamposDeBusca, 
    void** listaValoresDeBusca, int numeroParesCampoValor) 
{
    if (!arvoreBinariaExiste(arvoreBinaria) || listaCamposDeBusca == NULL || listaValoresDeBusca == NULL) return -1;

    int totalRegistros = 0;
    totalRegistros = arvoreBinariaImprimirBuscaAux(
        arvoreBinaria->raiz, campoIndexado, listaCamposDeBusca, 
        listaValoresDeBusca, numeroParesCampoValor
    );
    return totalRegistros;
}