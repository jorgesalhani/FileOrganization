#ifndef CABECALHO_H
    #define CABECALHO_H

    #include <stdlib.h>
    #include <stdint.h>
    #include <stdbool.h>

    typedef struct cabecalho_ CABECALHO;

    CABECALHO* cabecalhoCriar(char status, int64_t proxByteOffset, int32_t nroRegArq, int32_t nroRegRem);
    void cabecalhoImprimir(CABECALHO* cabecalho);
    bool cabecalhoAtualizarStatus(CABECALHO* cabecalho, char novoStatus);
    bool cabecalhoAtualizarProxByteOffset(CABECALHO* cabecalho, int64_t novoProxByteOffset);
    bool cabecalhoAtualizarNroRegArq(CABECALHO* cabecalho, int32_t novoNroRegArq);
    bool cabecalhoAtualizarNroRegRem(CABECALHO* cabecalho, int32_t novoNroRegRem);
    bool cabecalhoDeletar(CABECALHO** cabecalho);

#endif