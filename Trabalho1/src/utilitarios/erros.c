#include <stdlib.h>
#include <stdio.h>
#include "erros.h"

void erroGenerico() {
  printf("Falha no processamento do arquivo.\n");
}

void erroSemRegistros() {
  printf("Registro inexistente.\n");
}

void erroModo() {
  printf("Modo inexistente.\n");
}
