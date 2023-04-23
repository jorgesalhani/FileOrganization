#ifndef INDICE_H
    #define INDICE_H

    #define TAMANHO_CHAVE_BUSCA 12

    #include "dadosIndice.h"
    #include "cabecalhoIndice.h"
    #include "../utilitarios/funcoesFornecidas.h"
    #include "../tabela/tabela.h"
    
    typedef struct indice_ INDICE;

    INDICE* indiceCriar(char* nomeArquivoIndice, char* modoAberturaArquivo);
    bool indiceExiste(INDICE* indice);
    bool indiceAtualizarCabecalho(INDICE* indice, CABECALHO_INDICE* cabecalhoIndice);
    bool indiceInteiroAtualizarDados(INDICE* indice, DADOS_INDICE_INTEIRO* dadosIndiceInteiro);
    bool indiceStringAtualizarDados(INDICE* indice, DADOS_INDICE_STRING* dadosIndiceString);
    char* indiceObterNomeArquivo(INDICE* indice);
    bool indiceFecharArquivo(INDICE* indice);
    bool indiceDeletar(INDICE** indice, bool manterArquivo);

    INDICE* indiceCriarBinario(char* nomeArquivoEntrada, char* campoIndexado, char* tipoDado, char* nomeArquivoIndice);
    
#endif