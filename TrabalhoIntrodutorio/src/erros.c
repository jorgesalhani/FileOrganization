#include <stdlib.h>
#include <stdio.h>
#include "erros.h"

void erroGenerico() {
  printf("Falha no processamento do arquivo.\n");
  exit(1);
}

void erroSemRegistros() {
  printf("Registro inexistente.\n");
  exit(1);
}

void erroModo() {
  printf("Modo inexistente.\n");
  exit(1);
}
