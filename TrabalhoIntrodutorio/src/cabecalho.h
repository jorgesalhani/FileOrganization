#ifndef CABECALHO_H
    #define CABECALHO_H

    #include <stdlib.h>
    #include <stdint.h>
    #include <stdbool.h>

    typedef struct cabecalho_ CABECALHO;

    CABECALHO* cabecalhoCriar(char status, uint64_t proxByteOffset, uint32_t nroRegArq, uint32_t nroRegRem);
    void cabecalhoImprimir(CABECALHO* cabecalho);
    bool cabecalhoAtualizarStatus(CABECALHO* cabecalho, char novoStatus);
    bool cabecalhoAtualizarProxByteOffset(CABECALHO* cabecalho, uint64_t novoProxByteOffset);
    bool cabecalhoAtualizarNroRegArq(CABECALHO* cabecalho, uint32_t novoNroRegArq);
    bool cabecalhoAtualizarNroRegRem(CABECALHO* cabecalho, uint32_t novoNroRegRem);
    bool cabecalhoDeletar(CABECALHO** cabecalho);

#endif