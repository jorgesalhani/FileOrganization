#include <stdlib.h>
#include <stdio.h>
#include "erros.h"

void erroGenerico() {
  printf("Falha no processamento do arquivo.");
  exit(1);
}

void erroSemRegistros() {
  printf("Registro inexistente.");
  exit(1);
}

void erroModo() {
  printf("Modo inexistente");
  exit(1);
}