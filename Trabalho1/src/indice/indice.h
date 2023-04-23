#ifndef INDICE_H
    #define INDICE_H

    #include "dadosIndice.h"
    #include "cabecalhoIndice.h"
    #include "../tabela/tabela.h"
    
    typedef struct indice_ INDICE;

    INDICE* indiceCriar(char* nomeArquivoIndice);
    bool indiceExiste(INDICE* indice);
    bool indiceAtualizarCabecalho(INDICE* indice, CABECALHO_INDICE* cabecalhoIndice);
    bool indiceAtualizarDados(INDICE* indice, DADOS_INDICE* dadosIndice);
    char* indiceObterNomeArquivo(INDICE* indice);
    bool indiceFecharArquivo(INDICE* indice);
    bool indiceDeletar(INDICE** indice, bool manterArquivo);

    INDICE* indiceCriarBinario(char* nomeArquivoEntrada, char* campoIndexado, char* tipoDado, char* nomeArquivoIndice);
    
#endif