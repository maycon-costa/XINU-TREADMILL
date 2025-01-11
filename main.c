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
void monitorar_sensores(void) {
   int sensor_inicio_ativado = (rand() % 10) == 0;
   int sensor_fim_ativado = (rand() % 20) == 0;

   if (sensor_inicio_ativado) {
        iniciar_esteira();
   }
   if (sensor_fim_ativado) {
        parar_esteira();
   }
}
process processo_esteira(void) {
   while (TRUE) {
     if (em_movimento) {
          kprintf("Esteira em movimento...\n");
     } else {
          kprintf("Esteira parada.\n");
       }
        sleepms(ATRASO);  
    }
    return OK;
}
