#ifndef CHAMADOS_H
#define CHAMADOS_H

#include <time.h>

#define MAX_DESCRICAO 256
#define MAX_CHAMADOS 1000
#define ARQUIVO_DADOS "chamados.csv"


typedef enum {
    PRIORIDADE_BAIXA = 1,
    PRIORIDADE_MEDIA = 2,
    PRIORIDADE_ALTA = 3,
    PRIORIDADE_CRITICA = 4
} Prioridade;


typedef struct {
    int id;
    char descricao[MAX_DESCRICAO];
    Prioridade prioridade;
    time_t data_criacao;
} Chamado;


typedef struct NoArvore {
    Chamado chamado;
    struct NoArvore *esquerda;
    struct NoArvore *direita;
} NoArvore;


typedef struct NoPilha {
    Chamado chamado;
    struct NoPilha *proximo;
} NoPilha;


typedef struct {
    NoArvore *raiz;
    int quantidade;
} Arvore;


typedef struct {
    NoPilha *topo;
    int quantidade;
} Pilha;


Arvore* arvore_criar(void);
void arvore_inserir(Arvore *arvore, Chamado chamado);
Chamado* arvore_buscar_max(NoArvore *no);
void arvore_remover_max(Arvore *arvore);
void arvore_listar_inorder(NoArvore *no);
void arvore_destruir(NoArvore *no);
int arvore_proximo_id(NoArvore *no);


Pilha* pilha_criar(void);
void pilha_push(Pilha *pilha, Chamado chamado);
Chamado* pilha_pop(Pilha *pilha);
Chamado* pilha_peek(Pilha *pilha);
int pilha_vazia(Pilha *pilha);
void pilha_destruir(Pilha *pilha);


void carregar_chamados(Arvore *arvore);
void salvar_chamados(Arvore *arvore);


const char* prioridade_para_string(Prioridade p);
Prioridade string_para_prioridade(const char *str);
void limpar_buffer(void);
void exibir_chamado(Chamado *chamado);

#endif
