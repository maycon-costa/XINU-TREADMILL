#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

#define PINO_MOTOR 5
#define SENSOR_INICIO 6
#define SENSOR_FIM 7
#define ATRASO 500

#define TRUE 1
#define FALSE 0

int em_movimento = FALSE;

void iniciar_esteira(void) {
  kprintf("Iniciando a esteira.\n");
  em_movimento = TRUE;
}

void parar_esteira(void) {
  kprintf("Parando a esteira.\n");
  em_movimento = FALSE;
}
