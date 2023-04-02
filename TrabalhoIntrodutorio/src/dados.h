#ifndef DADOS_H
    #define DADOS_H

    #include <stdint.h>
    #include <stdlib.h>
    #include <stdbool.h>

    typedef struct dados_ DADOS;

    DADOS* dadosCriar(char removido, int32_t idCrime, char dataCrime[10], int32_t numeroArtigo, 
                      char marcaCelular[12], char* lugarCrime, char* descricaoCrime);
    void dadosImprimir(DADOS* dados);
    bool dadosAtualizarRemovido(DADOS* dados, char removido);
    bool dadosAtualizarIdCrime(DADOS* dados, int32_t novoIdCrime);
    bool dadosAtualizarDataCrime(DADOS* dados, char* novoDataCrime);
    bool dadosAtualizarNumeroArtigo(DADOS* dados, int32_t novoNumeroArtigo);
    bool dadosAtualizarMarcaCelular(DADOS* dados, char* novoMarcaCelular);
    bool dadosAtualizarLugarCrime(DADOS* dados, char* novoLugarCrime);
    bool dadosAtualizarDescricaoCrime(DADOS* dados, char* novoDescricaoCrime);
    bool dadosDeletar(DADOS** dados);

#endif