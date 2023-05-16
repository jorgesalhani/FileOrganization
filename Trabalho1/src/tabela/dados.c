#include <stdio.h>
#include <string.h>
#include "dados.h"

/********************
 * ESTRUTURA DE DADOS
 * ******************
*/

struct metadados_ {
  int tamanhoLugarCrime;
  int tamanhoDescricaoCrime;
  int64_t tamanhoRegistro;
};

struct dados_ {
    int32_t idCrime;
    char dataCrime[TAMANHO_DATA_CRIME];
    int32_t numeroArtigo; 
    char marcaCelular[TAMANHO_MARCA_CELULAR];
    char* lugarCrime;
    char* descricaoCrime;
    char removido;
    char delimitador;
};

enum camposIndexados { 
  idCrime, dataCrime, numeroArtigo, marcaCelular, 
  lugarCrime, descricaoCrime, removido, delimitador, 
  ENUM_FIM};

static const char* camposIndexadosNomes[ENUM_FIM] = {
  "idCrime", "dataCrime", "numeroArtigo", "marcaCelular", 
  "lugarCrime", "descricaoCrime", "removido", "delimitador"};

/**
 * Tipos de campos indexados
 * 0 - int32_t: 4 bytes
 * 1 - char*: string (char*) sem tamanho pre-definido
 * 2 - char: 1 byte
*/
static const int camposIndexadosTipos[ENUM_FIM] = {
  0, 1, 0, 1, 1, 1, 2, 2
};

/*********************
 * FUNCOES UTILITARIAS
 * *******************
*/

bool stringFixaValida(char* entrada, size_t tamanho, char delimitador) {
    printf("%ld %ld\n", strlen(entrada), tamanho);
    return (strlen(entrada) <= sizeof(char)*tamanho) ? true : false;
}

bool dadosEntrada4BytesValida(int32_t entrada) {
    return (sizeof(entrada) <= sizeof(int32_t)) ? true : false;
}

bool dadosEntradasValidas(char removido, int32_t idCrime, char* dataCrime, int32_t numeroArtigo, char* marcaCelular) {
    if (
        removidoValido(removido) && stringFixaValida(dataCrime, TAMANHO_DATA_CRIME, '$') && 
        stringFixaValida(marcaCelular, TAMANHO_MARCA_CELULAR, '$') && dadosEntrada4BytesValida(idCrime) &&
        dadosEntrada4BytesValida(numeroArtigo)
    ) return true;
    return false;
}

void preencherCamposFixos(char* novoCampo, char* campoEntrada, int tamanho) {
    for (int i = 0; i < strlen(campoEntrada); i++) novoCampo[i] = campoEntrada[i];
    for (int i = strlen(campoEntrada); i < tamanho; i++) novoCampo[i] = '$';
}

bool deletarStringsFixas(char* stringFixa) {
    if (stringFixa == NULL) return false;
    free(stringFixa);
    stringFixa = NULL;
}

bool strVazia(char* str) {
  if(strcmp(str, "") == 0 || str[0] == '$') {
    strcpy(str, STR_VAZIA);
    return true;
  }
  return false;
}

bool dadosRemovido(DADOS* dados) {
  if (!dadosExiste(dados)) return true;
  return dadosObterRemovido(dados) == '1' ? true : false;
}

/********************
 * FUNCOES PRINCIPAIS
 * ******************
*/

bool metadadosExiste(METADADOS* metadados) {
  return metadados != NULL ? true : false;
}

bool removidoValido(char removido) {
    return (removido != '0' && removido != '1') ? false : true;
}

bool dadosExiste(DADOS* dados) {
    return dados != NULL ? true : false;
}

DADOS* dadosCriar(
    int32_t idCrime, char dataCrime[TAMANHO_DATA_CRIME], 
    int32_t numeroArtigo, char marcaCelular[TAMANHO_MARCA_CELULAR], 
    char* lugarCrime, char* descricaoCrime, char removido) {
    
    DADOS* dados = (DADOS*) malloc(sizeof(DADOS));
    
    if (!dadosExiste(dados)) return NULL;
    // if (!dadosEntradasValidas(removido, idCrime, dataCrime, numeroArtigo, marcaCelular)) {
    //     free(dados);
    //     dados = NULL;
    //     return NULL;
    // }

    dados->removido = removido;
    dados->idCrime = idCrime;
    strcpy(dados->dataCrime, dataCrime);
    dados->numeroArtigo = numeroArtigo;
    strcpy(dados->marcaCelular, marcaCelular);

    int tamanhoLugarCrime = strlen(lugarCrime);
    int tamanhoDescricaoCrime = strlen(descricaoCrime);

    dados->lugarCrime = (char*) malloc(sizeof(char)*(tamanhoLugarCrime + 1));
    strcpy(dados->lugarCrime, lugarCrime);
    
    dados->descricaoCrime = (char*) malloc(sizeof(char)*(tamanhoDescricaoCrime + 1));  
    strcpy(dados->descricaoCrime, descricaoCrime);

    dados->delimitador = '#';

    return dados;
}

METADADOS* dadosCriarMetadados(int tamanhoDescricaoCrime, int tamanhoLugarCrime) {
  METADADOS* metadados = (METADADOS*) malloc(sizeof(METADADOS));
  if (!metadadosExiste(metadados)) return NULL;
  metadados->tamanhoDescricaoCrime = tamanhoDescricaoCrime;
  metadados->tamanhoLugarCrime = tamanhoLugarCrime;
  return metadados;
}

void dadosImprimir(DADOS* dados, METADADOS* metadados) {
  if (!dadosExiste(dados)) return;
  int32_t idAux = dadosObterIdCrime(dados);
  int32_t numArtAux = dadosObterNumeroArtigo(dados);

  char dataAux[TAMANHO_DATA_CRIME+5] = ""; 
  strcpy(dataAux, dadosObterDataCrime(dados));
  dataAux[TAMANHO_DATA_CRIME] = '\0';

  char marcaAux[TAMANHO_MARCA_CELULAR+5] = "";
  strcpy(marcaAux, dadosObterMarcaCelular(dados));
  marcaAux[TAMANHO_MARCA_CELULAR] = '\0';
  if(!strVazia(marcaAux)) {
    for(int j = 0; j < TAMANHO_MARCA_CELULAR-1; j++) {
      if(marcaAux[j] == '$') {
        marcaAux[j] = '\0';
        break;
      }
    }
  }
  
  int tamanhoLugarAux = dadosMetadadosObterTamanhoLugarCrime(metadados);
  char lugarAux[tamanhoLugarAux + 5];
  strcpy(lugarAux, dadosObterLugarCrime(dados));
  lugarAux[tamanhoLugarAux] = '\0';

  int tamanhoDescricaoAux = dadosMetadadosObterTamanhoDescricaoCrime(metadados);
  char descricaoAux[tamanhoDescricaoAux + 5];
  strcpy(descricaoAux, dadosObterDescricaoCrime(dados));
  descricaoAux[tamanhoDescricaoAux] = '\0';
  
  strVazia(dataAux); 
  strVazia(lugarAux);
  strVazia(descricaoAux);

  if(numArtAux == -1) {
    printf("%d, %s, %s, %s, %s, %s\n", idAux, dataAux, STR_VAZIA, lugarAux, descricaoAux, marcaAux); 
  } else {
    printf("%d, %s, %d, %s, %s, %s\n", idAux, dataAux, numArtAux, lugarAux, descricaoAux, marcaAux); 
  }
}

bool dadosAtualizarRemovido(DADOS* dados, char removido) {
  if (!dadosExiste(dados)) return false;
  dados->removido = removido;
  return true;
}

bool dadosAtualizarIdCrime(DADOS* dados, int32_t novoIdCrime) {
  if (!dadosExiste(dados)) return false;
  dados->idCrime = novoIdCrime;
  return true;
}

bool dadosAtualizarDataCrime(DADOS* dados, char* novoDataCrime) {
  if (!dadosExiste(dados)) return false;
  strcpy(dados->dataCrime, novoDataCrime);
  if(strlen(novoDataCrime) == 0) {
    for(int i = 0; i < TAMANHO_DATA_CRIME; i++) {
      dados->dataCrime[i] = '$';
    }
  } 
  return true;
}

bool dadosAtualizarNumeroArtigo(DADOS* dados, int32_t novoNumeroArtigo) {
  if (!dadosExiste(dados)) return false;
  dados->numeroArtigo = novoNumeroArtigo;
  return true;
}

bool dadosAtualizarMarcaCelular(DADOS* dados, char* novoMarcaCelular) {
  if (!dadosExiste(dados)) return false;
  strcpy(dados->marcaCelular, novoMarcaCelular);
  int tam = strlen(novoMarcaCelular);
  for(int i = tam; i < TAMANHO_MARCA_CELULAR; i++) {
    dados->marcaCelular[i] = '$';
  }
  return true;
}

bool dadosAtualizarLugarCrime(DADOS* dados, char* novoLugarCrime, METADADOS* metadados) {
  if (!dadosExiste(dados) || !metadadosExiste(metadados)) return false;
  
  metadados->tamanhoLugarCrime = strlen(novoLugarCrime);
  dados->lugarCrime = realloc(
    dados->lugarCrime, sizeof(char)*(strlen(novoLugarCrime) + 1)
  );

  for (int i = 0; i < strlen(novoLugarCrime) + 1; i++) {
    dados->lugarCrime[i] = novoLugarCrime[i];
  }
  
  return true;
}

bool dadosAtualizarDescricaoCrime(DADOS* dados, char* novoDescricaoCrime, METADADOS* metadados) {
  if (!dadosExiste(dados) || !metadadosExiste(metadados)) return false;
  
  metadados->tamanhoDescricaoCrime = strlen(novoDescricaoCrime);
  dados->descricaoCrime = realloc(
    dados->descricaoCrime, sizeof(char)*(strlen(novoDescricaoCrime) + 1)
  );

  for (int i = 0; i < strlen(novoDescricaoCrime) + 1; i++) {
    dados->descricaoCrime[i] = novoDescricaoCrime[i];
  }
  
  return true;
}

char dadosObterRemovido(DADOS* dados) {
    if (!dadosExiste(dados)) return 'X';
    return dados->removido;
}

int32_t dadosObterIdCrime(DADOS* dados) {
    if (!dadosExiste(dados)) return -1;
    return dados->idCrime;
}

char* dadosObterDataCrime(DADOS* dados) {
    if (!dadosExiste(dados)) return "$$$$$$$$$";
    return dados->dataCrime;
}

int32_t dadosObterNumeroArtigo(DADOS* dados) {
    if (!dadosExiste(dados)) return -1;
    return dados->numeroArtigo;
}

char* dadosObterMarcaCelular(DADOS* dados) {
    if (!dadosExiste(dados)) return "$$$$$$$$$$$";
    return dados->marcaCelular;
}

char* dadosObterLugarCrime(DADOS* dados) {
    if (!dadosExiste(dados)) return "|";
    return dados->lugarCrime;
}

char* dadosObterDescricaoCrime(DADOS* dados) {
    if (!dadosExiste(dados)) return "|";
    return dados->descricaoCrime;
}

char dadosObterDelimitadorRegistro(DADOS* dados) {
    if (!dadosExiste(dados)) return '#';
    return dados->delimitador;
}

bool dadosDeletar(DADOS** dados) {
    if (dados == NULL || !dadosExiste(*dados)) return false;
    free((*dados)->lugarCrime);  
    free((*dados)->descricaoCrime);
    free(*dados);
    *dados = NULL;
    dados = NULL;
    return true;
}

int64_t dadosMetadadosObterTamanhoLugarCrime(METADADOS* metadados) {
  if (!metadadosExiste(metadados)) return -1;
  return metadados->tamanhoLugarCrime;
}

int64_t dadosMetadadosObterTamanhoDescricaoCrime(METADADOS* metadados) {
  if (!metadadosExiste(metadados)) return -1;
  return metadados->tamanhoDescricaoCrime;
}

bool dadosMetadadosDeletar(METADADOS** metadados) {
  if (metadados == NULL || !metadadosExiste(*metadados)) return false;
  free(*metadados);
  *metadados = NULL;
  metadados = NULL;
  return true;
}

int64_t dadosMetadadosObterTamanhoRegistro(DADOS* dados, METADADOS* metadados) {
  if (!dadosExiste(dados) || !metadadosExiste(metadados)) return -1;
  metadados->tamanhoRegistro = 0;
  metadados->tamanhoRegistro += 
    sizeof(dados->idCrime) + sizeof(dados->dataCrime) + 
    sizeof(dados->numeroArtigo) + sizeof(dados->marcaCelular) + 
    sizeof(dados->removido) + sizeof(dados->delimitador);

  metadados->tamanhoRegistro += 
    metadados->tamanhoLugarCrime + metadados->tamanhoDescricaoCrime;

  metadados->tamanhoRegistro += sizeof(char) * (NUMERO_CAMPOS_VARIAVEIS);
  
  return metadados->tamanhoRegistro;
}

bool dadosAtualizarRegistro(
  DADOS* dados, METADADOS* metadados,
  char removido, int32_t novoIdCrime, 
  char* novoDataCrime, int32_t novoNumeroArtigo, 
  char* novoMarcaCelular, char* novoLugarCrime, 
  char* novoDescricaoCrime
) {
  dados->delimitador = '#';
  dadosAtualizarRemovido(dados, removido);
  dadosAtualizarIdCrime(dados, novoIdCrime);
  dadosAtualizarDataCrime(dados, novoDataCrime);
  dadosAtualizarNumeroArtigo(dados, novoNumeroArtigo);
  dadosAtualizarMarcaCelular(dados, novoMarcaCelular);
  dadosAtualizarDescricaoCrime(dados, novoDescricaoCrime, metadados);
  dadosAtualizarLugarCrime(dados, novoLugarCrime, metadados);
  return true;
}

bool dadosCampoIndexadoValido(char* campoIndexado) {
  enum camposIndexados campo_i;
  for (campo_i = 0; campo_i < ENUM_FIM; ++campo_i) {
    if (strcmp(campoIndexado, camposIndexadosNomes[campo_i]) == 0) {
      return true;
    }
  }
  return false;
}

int dadosObterNumeroCampoIndexado(char* campoIndexado) {
  enum camposIndexados campo_i;
  for (campo_i = 0; campo_i < ENUM_FIM; ++campo_i) {
    if (strcmp(campoIndexado, camposIndexadosNomes[campo_i]) == 0) {
      return camposIndexadosTipos[campo_i];
    }
  }
  return ENUM_FIM;
}

void* dadosObterCampoIndexado(DADOS* dados, char* campoIndexado) {

  void *dadosCamposEmRegistro[ENUM_FIM] = {
    &(dados->idCrime), dados->dataCrime, &(dados->numeroArtigo), dados->marcaCelular, 
    dados->lugarCrime, dados->descricaoCrime, &(dados->removido), &(dados->delimitador), 
  };

  enum camposIndexados campo_i;
  for (campo_i = 0; campo_i < ENUM_FIM; ++campo_i) {
    if (strcmp(campoIndexado, camposIndexadosNomes[campo_i]) == 0) {
      return dadosCamposEmRegistro[campo_i];
    }
  }

  return NULL;
}

int dadosObterEnumCampo(char* campoIndexado) {
  enum camposIndexados campo_i;
  for (campo_i = 0; campo_i < ENUM_FIM; ++campo_i) {
    if (strcmp(campoIndexado, camposIndexadosNomes[campo_i]) == 0) {
      return campo_i;
    }
  }
  return ENUM_FIM;
}

bool dadosValorIndexadoValido(DADOS* dados, char* campoIndexado, char* tipoDado) {
  if (!dadosExiste(dados) || (!tipoDadoInteiroValido(tipoDado) && !tipoDadoStringValido(tipoDado))) return false;
  
  if (strcmp(tipoDado, "inteiro") == 0) {
    int32_t* valorInt = (int32_t*) dadosObterCampoIndexado(dados, campoIndexado);
    if (valorInt == NULL) return false;
    if (*valorInt != -1) return true;
  } else {
    char* valorStr = (char*) dadosObterCampoIndexado(dados, campoIndexado);
    if (valorStr == NULL) return false;
    if (strlen(valorStr) != 0 && valorStr[0] != '$') return true;
  }

  return false;
}

bool dadosBuscaCorrespondenciaCompleta(
  DADOS* dados, char** listaCamposDeBusca,
  void** listaValoresDeBusca, int numeroParesCampoValor
) {
  for (int i = 0; i < numeroParesCampoValor; i++) {
    char* campo = listaCamposDeBusca[i];
    void* valor = listaValoresDeBusca[i];

    int numeroCampo = dadosObterNumeroCampoIndexado(campo);
    
    int32_t* valorBuscaInt = NULL;
    int32_t* valorEncontradoInt = NULL;
    char* valorBuscaStr = NULL;
    char* valorEncontradoStr = NULL;
    
    switch (numeroCampo) {
      case 0:
        valorBuscaInt = (int32_t*) valor;
        valorEncontradoInt = (int32_t*) dadosObterCampoIndexado(dados, campo);
        if (*valorBuscaInt != *valorEncontradoInt) return false;
        break;

      case 1:
        valorBuscaStr = (char*) valor;
        valorEncontradoStr = (char*) dadosObterCampoIndexado(dados, campo);

        char valorEncontradoStrFormat[50] = "";
        for (int i = 0; i < strlen(valorEncontradoStr); i++) {
          if (valorEncontradoStr[i] == '$') valorEncontradoStr[i] = '\0';
          else valorEncontradoStrFormat[i] = valorEncontradoStr[i];
        }
        if (strcmp(valorBuscaStr, valorEncontradoStrFormat) != 0) return false;
        break;

      default:
       break;
    }
  }

  return true;

}

bool dadosAtualizaCamposEspecificados(DADOS* dados, METADADOS* metadados, char** listaCamposDeAtualizacao, void** listaValoresDeAtualizacao, int numeroParesCampoValorAtualizacao) {
  if (!dadosExiste(dados) || !metadadosExiste(metadados) ||
    listaCamposDeAtualizacao == NULL || listaValoresDeAtualizacao == NULL
  ) return false;
  
  for (int i = 0; i < numeroParesCampoValorAtualizacao; i++) {
    char* campoIndexado = listaCamposDeAtualizacao[i];
    void* valorAtualizado = listaValoresDeAtualizacao[i];
    int numeroCampoIndexado = dadosObterNumeroCampoIndexado(campoIndexado);
    
    int32_t* valorInt = NULL;
    int32_t* valorNovoInt = NULL;
    char* valorStr = "";
    char* valorNovoStr = "";

    int tamStrAntiga = 0;
    int tamStrNova = 0;

    int64_t tamRegAntigo = dadosMetadadosObterTamanhoRegistro(dados, metadados);

    switch (numeroCampoIndexado) {
      case 0:;
        valorInt = (int*) dadosObterCampoIndexado(dados, campoIndexado);
        valorNovoInt = (int*) valorAtualizado;
        *valorInt = *valorNovoInt;
        break;
      
      case 1:;
        int enumCampo = dadosObterEnumCampo(campoIndexado);
        valorStr = (char*) dadosObterCampoIndexado(dados, campoIndexado);
        valorNovoStr = (char*) valorAtualizado;
        METADADOS* metadadosAtualizados = NULL;
        switch (enumCampo) {
          case 1:; // dataCrime
            dadosAtualizarDataCrime(dados, valorNovoStr);
            break;

          case 3:; // marcaCelular
            dadosAtualizarMarcaCelular(dados, valorNovoStr);
            break;

          case 4:; // lugarCrime
            tamStrNova = (int)strlen(valorNovoStr);
            metadados->tamanhoLugarCrime = tamStrNova;
            dadosAtualizarLugarCrime(dados, valorNovoStr, metadados);
            break;
          
          case 5:; // descricaoCrime
            tamStrNova = (int)strlen(valorNovoStr);
            metadados->tamanhoDescricaoCrime = tamStrNova;
            dadosAtualizarDescricaoCrime(dados, valorNovoStr, metadados);
            break;

          default:;
            break;
        }

        break;

      default:;
        break;
    }
  }

  return true;
}