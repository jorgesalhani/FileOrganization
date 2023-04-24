#ifndef DADOS_INDICE_H
    #define DADOS_INDICE_H
    #define TAMANHO_CHAVE_BUSCA 12

    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdint.h>
    #include <string.h>

    typedef struct dados_indice_inteiro_ DADOS_INDICE_INTEIRO;
    typedef struct dados_indice_string_ DADOS_INDICE_STRING;

    DADOS_INDICE_INTEIRO* dadosIndiceInteiroCriar(char* tipoDado, int32_t chaveBuscaInteiro, int64_t byteOffset);
    DADOS_INDICE_STRING* dadosIndiceStringCriar(char* tipoDado, char chaveBuscaString[TAMANHO_CHAVE_BUSCA], int64_t byteOffset);
    
    bool dadosIndiceInteiroExiste(DADOS_INDICE_INTEIRO* dadosIndiceInteiro);
    bool dadosIndiceStringExiste(DADOS_INDICE_STRING* dadosIndiceString);

    bool tipoDadoStringValido(char* tipoDado);
    bool tipoDadoInteiroValido(char* tipoDado);

    int32_t dadosIndiceObterChaveBuscaInteiro(DADOS_INDICE_INTEIRO* dadosIndiceInteiro);
    char* dadosIndiceObterChaveBuscaString(DADOS_INDICE_STRING* dadosIndiceString);

    int64_t dadosIndiceInteiroObterByteOffset(DADOS_INDICE_INTEIRO* dadosIndiceInteiro);
    int64_t dadosIndiceStringObterByteOffset(DADOS_INDICE_STRING* dadosIndiceString);

    bool dadosIndiceInteiroAtualizarChaveBusca(DADOS_INDICE_INTEIRO* dadosIndice, int32_t novaChaveBuscaInteiro);
    bool dadosIndiceStringAtualizarChaveBusca(DADOS_INDICE_STRING* dadosIndice, char novaChaveBuscaString[TAMANHO_CHAVE_BUSCA]);

    bool dadosIndiceInteiroAtualizarByteOffset(DADOS_INDICE_INTEIRO* dadosIndice, int64_t novoByteOffset);
    bool dadosIndiceStringAtualizarByteOffset(DADOS_INDICE_STRING* dadosIndice, int64_t novoByteOffset);

    bool dadosIndiceInteiroDeletar(DADOS_INDICE_INTEIRO** dadosIndiceInteiro);
    bool dadosIndiceStringDeletar(DADOS_INDICE_STRING** dadosIndiceString);

#endif