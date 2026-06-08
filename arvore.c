#include "chamados.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Criar uma nova árvore vazia */
Arvore* arvore_criar(void) {
    Arvore *arvore = (Arvore *)malloc(sizeof(Arvore));
    if (arvore == NULL) {
        fprintf(stderr, "Erro ao alocar memória para árvore\n");
        return NULL;
    }
    arvore->raiz = NULL;
    arvore->quantidade = 0;
    return arvore;
}

/* Criar um novo nó da árvore */
static NoArvore* no_criar(Chamado chamado) {
    NoArvore *no = (NoArvore *)malloc(sizeof(NoArvore));
    if (no == NULL) {
        fprintf(stderr, "Erro ao alocar memória para nó\n");
        return NULL;
    }
    no->chamado = chamado;
    no->esquerda = NULL;
    no->direita = NULL;
    return no;
}

/* Inserir um chamado na árvore (BST ordenada por prioridade, depois por ID) */
static NoArvore* _inserir_recursivo(NoArvore *no, Chamado chamado) {
    if (no == NULL) {
        return no_criar(chamado);
    }

    /* Comparação: prioridade maior vai para direita (raiz tem maior prioridade) */
    if (chamado.prioridade > no->chamado.prioridade ||
        (chamado.prioridade == no->chamado.prioridade && chamado.id > no->chamado.id)) {
        no->direita = _inserir_recursivo(no->direita, chamado);
    } else {
        no->esquerda = _inserir_recursivo(no->esquerda, chamado);
    }

    return no;
}

void arvore_inserir(Arvore *arvore, Chamado chamado) {
    if (arvore == NULL) return;
    if (arvore->quantidade >= MAX_CHAMADOS) {
        fprintf(stderr, "Limite de chamados atingido\n");
        return;
    }
    arvore->raiz = _inserir_recursivo(arvore->raiz, chamado);
    arvore->quantidade++;
}

/* Buscar o chamado com maior prioridade (raiz ou nó mais à direita) */
Chamado* arvore_buscar_max(NoArvore *no) {
    if (no == NULL) return NULL;
    
    NoArvore *atual = no;
    while (atual->direita != NULL) {
        atual = atual->direita;
    }
    return &(atual->chamado);
}

/* Remover o nó com maior prioridade */
static NoArvore* _remover_max_recursivo(NoArvore *no, Chamado *chamado_removido) {
    if (no == NULL) return NULL;

    if (no->direita == NULL) {
        /* Este é o nó de máxima prioridade */
        *chamado_removido = no->chamado;
        NoArvore *esquerda = no->esquerda;
        free(no);
        return esquerda;
    }

    no->direita = _remover_max_recursivo(no->direita, chamado_removido);
    return no;
}

void arvore_remover_max(Arvore *arvore) {
    if (arvore == NULL || arvore->raiz == NULL) return;

    Chamado temp;
    arvore->raiz = _remover_max_recursivo(arvore->raiz, &temp);
    arvore->quantidade--;
}

/* Listar chamados em ordem de prioridade (in-order da BST) */
void arvore_listar_inorder(NoArvore *no) {
    if (no == NULL) return;

    /* Percurso reverso para mostrar maior prioridade primeiro */
    arvore_listar_inorder(no->direita);
    exibir_chamado(&(no->chamado));
    arvore_listar_inorder(no->esquerda);
}

/* Encontrar o próximo ID disponível */
int arvore_proximo_id(NoArvore *no) {
    if (no == NULL) return 1;

    int max_id = no->chamado.id;
    
    if (no->esquerda != NULL) {
        int id_esq = arvore_proximo_id(no->esquerda);
        if (id_esq > max_id) max_id = id_esq;
    }
    
    if (no->direita != NULL) {
        int id_dir = arvore_proximo_id(no->direita);
        if (id_dir > max_id) max_id = id_dir;
    }

    return max_id + 1;
}

/* Destruir a árvore (liberar memória) */
void arvore_destruir(NoArvore *no) {
    if (no == NULL) return;

    arvore_destruir(no->esquerda);
    arvore_destruir(no->direita);
    free(no);
}
