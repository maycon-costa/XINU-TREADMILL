#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define MAX_SACOS_NO_PALLET 10  // Capacidade máxima do pallet
#define TEMPO_MANUTENCAO 4000   // Tempo para manutenção em ms
#define TEMPO_TURNO 10000       // Tempo total do turno (1 minuto)
#define TEMPO_FUNIL_MEDIO 2000  // Tempo do funil na velocidade média

int em_movimento = FALSE;
int pallet_cheio = FALSE;
int contador_sacos = 0;
int total_sacos_produzidos = 0;
int velocidade_esteira = TEMPO_FUNIL_MEDIO;
int necessidade_manutencao = FALSE;
int tempo_operacao = 0;


sid32 semaforo_esteira;
sid32 semaforo_funil;


int verificar_presenca_pallet(void) {
    return rand() % 2; 
}

void atualizar_leds(void) {
    if (em_movimento) {
        kprintf("Esteira em movimento (LED verde).\n");
    } else {
        kprintf("Esteira parada (LED vermelho).\n");
    }
}

void iniciar_esteira(void) {
    if (!verificar_presenca_pallet()) {
        kprintf("Nenhum pallet detectado! A esteira nao pode iniciar.\n");
        return;
    }

    em_movimento = TRUE;
    atualizar_leds();
}

void parar_esteira(void) {
    em_movimento = FALSE;
    atualizar_leds();
}

void enviar_para_estoque(void) {
    kprintf("Pallet enviado para o estoque.\n");
    contador_sacos = 0;  
    pallet_cheio = FALSE; 
    if (!necessidade_manutencao) {
        iniciar_esteira();
    }
}

void realizar_manutencao(void) {
    kprintf("Realizando manutencao na esteira...\n");
    sleepms(TEMPO_MANUTENCAO);
    kprintf("Manutencao concluida.\n");
    necessidade_manutencao = FALSE;
    iniciar_esteira();
}

void monitorar_pallet(void) {
    if (contador_sacos >= MAX_SACOS_NO_PALLET) {
        if (!pallet_cheio) {
            pallet_cheio = TRUE;
            parar_esteira();
            kprintf("Pallet cheio! A esteira foi parada.\n");
        }
    }
}

void relatorio_producao(void) {
    kprintf("\n--- Relatorio de Producao ---\n");
    kprintf("Sacos de racao produzidos: %d\n", total_sacos_produzidos);
    kprintf("Turno total: %d ms\n", tempo_operacao);
    kprintf("-----------------------------\n");
}

void processo_esteira(void) {
    while (TRUE) {
        wait(semaforo_esteira);
        if (tempo_operacao >= TEMPO_TURNO) {
            parar_esteira();
            relatorio_producao();
            exit(); 
        }
        signal(semaforo_esteira);
        sleepms(2000);
    }
}

void processo_sensores(void) {
    while (TRUE) {
        wait(semaforo_esteira);
        if (!pallet_cheio) {
            contador_sacos++;
            total_sacos_produzidos++;
            kprintf("Saco de racao adicionado! Total de sacos no pallet: %d\n", contador_sacos);
            monitorar_pallet();
        } else if (necessidade_manutencao) {
            realizar_manutencao();
        } else {
            enviar_para_estoque();
        }
        signal(semaforo_esteira);
        sleepms(velocidade_esteira);
    }
}

void processo_funil(void) {
    while (TRUE) {
        wait(semaforo_funil);
        if (em_movimento && !pallet_cheio && !necessidade_manutencao) {
            sleepms(velocidade_esteira);
        }
        signal(semaforo_funil);
    }
}



int main(void) {
    semaforo_esteira = semcreate(1);
    semaforo_funil = semcreate(1);

    iniciar_esteira();

    resume(create(processo_esteira, 1024, 20, "esteira", 0));
    resume(create(processo_sensores, 1024, 20, "sensores", 0));
    resume(create(processo_funil, 1024, 20, "funil", 0));


    return OK;
}
