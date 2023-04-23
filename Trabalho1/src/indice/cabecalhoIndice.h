#ifndef CABECALHO_INDICE_H
    #define CABECALHO_INDICE_H

    #include <stdbool.h>
    #include <stdlib.h>
    #include <stdint.h>

    typedef struct cabecalho_indice_ CABECALHO_INDICE;

    CABECALHO_INDICE* cabecalhoIndiceCriar(char status);
    bool cabecalhoIndiceExiste(CABECALHO_INDICE* cabecalhoIndice);
    bool cabecalhoIndiceAtualizarStatus(CABECALHO_INDICE* cabecalhoIndice, char novoStatus);
    char cabecalhoIndiceObterStatus(CABECALHO_INDICE* cabecalhoIndice);
    bool cabecalhoIndiceDeletar(CABECALHO_INDICE** cabecalhoIndice);

#endif