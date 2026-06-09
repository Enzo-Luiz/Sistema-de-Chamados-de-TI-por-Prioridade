#include "chamados.h"
#include <stdlib.h>
#include <stdio.h>


Pilha* pilha_criar(void) {
    Pilha *pilha = (Pilha *)malloc(sizeof(Pilha));
    if (pilha == NULL) {
        fprintf(stderr, "Erro ao alocar memória para pilha\n");
        return NULL;
    }
    pilha->topo = NULL;
    pilha->quantidade = 0;
    return pilha;
}


void pilha_push(Pilha *pilha, Chamado chamado) {
    if (pilha == NULL) return;

    NoPilha *novo = (NoPilha *)malloc(sizeof(NoPilha));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memória para nó da pilha\n");
        return;
    }

    novo->chamado = chamado;
    novo->proximo = pilha->topo;
    pilha->topo = novo;
    pilha->quantidade++;
}


Chamado* pilha_pop(Pilha *pilha) {
    if (pilha == NULL || pilha->topo == NULL) return NULL;

    NoPilha *temp = pilha->topo;
    Chamado *chamado = (Chamado *)malloc(sizeof(Chamado));
    if (chamado == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
        return NULL;
    }

    *chamado = temp->chamado;
    pilha->topo = temp->proximo;
    free(temp);
    pilha->quantidade--;

    return chamado;
}


Chamado* pilha_peek(Pilha *pilha) {
    if (pilha == NULL || pilha->topo == NULL) return NULL;
    return &(pilha->topo->chamado);
}


int pilha_vazia(Pilha *pilha) {
    if (pilha == NULL) return 1;
    return pilha->topo == NULL;
}


void pilha_destruir(Pilha *pilha) {
    if (pilha == NULL) return;

    while (pilha->topo != NULL) {
        NoPilha *temp = pilha->topo;
        pilha->topo = pilha->topo->proximo;
        free(temp);
    }

    free(pilha);
}
