#ifndef DADOS_H
    #define DADOS_H
    #define TAMANHO_DATA_CRIME 10
    #define TAMANHO_MARCA_CELULAR 12

    #include <stdint.h>
    #include <stdlib.h>
    #include <stdbool.h>

    typedef struct dados_ DADOS;

    DADOS* dadosCriar(char removido, uint32_t idCrime, char* dataCrime, uint32_t numeroArtigo, 
                      char* marcaCelular, char* lugarCrime, char* descricaoCrime);
    void dadosImprimir(DADOS* dados);
    bool dadosAtualizarRemovido(DADOS* dados, char removido);
    bool dadosAtualizarIdCrime(DADOS* dados, uint32_t novoIdCrime);
    bool dadosAtualizarDataCrime(DADOS* dados, char* novoDataCrime);
    bool dadosAtualizarNumeroArtigo(DADOS* dados, uint32_t novoNumeroArtigo);
    bool dadosAtualizarMarcaCelular(DADOS* dados, char* novoMarcaCelular);
    bool dadosAtualizarLugarCrime(DADOS* dados, char* novoLugarCrime);
    bool dadosAtualizarDescricaoCrime(DADOS* dados, char* novoDescricaoCrime);
    bool dadosDeletar(DADOS** dados);

#endif