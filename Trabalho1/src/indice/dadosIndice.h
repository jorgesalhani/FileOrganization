#ifndef DADOS_INDICE_H
    #define DADOS_INDICE_H
    #define TAMANHO_CHAVE_BUSCA 12

    #include <stdbool.h>
    #include <stdint.h>

    typedef struct dados_indice_ DADOS_INDICE;

    DADOS_INDICE* dadosIndiceCriar(char* tipoDado, uint32_t chaveBuscaInteiro, char chaveBuscaString[TAMANHO_CHAVE_BUSCA], uint64_t byteOffset);
    bool dadosIndiceExiste(DADOS_INDICE* dadosIndice);
    uint32_t dadosIndiceObterChaveBuscaInteiro(DADOS_INDICE* dadosIndice);
    char* dadosIndiceObterChaveBuscaString(DADOS_INDICE* dadosIndice);
    bool chaveBuscaInteiroValida(uint32_t chaveBuscaInteiro);
    uint64_t dadosIndiceObterByteOffset(DADOS_INDICE* dadosIndice);
    bool dadosIndiceAtualizarChaveBusca(DADOS_INDICE* dadosIndice, uint32_t novaChaveBusca);
    bool dadosIndiceAtualizarByteOffset(DADOS_INDICE* dadosIndice, uint64_t novoByteOffset);
    bool dadosIndiceDeletar(DADOS_INDICE** dadosIndice);

#endif