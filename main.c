#include "chamados.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


static void exibir_menu(void);
static void adicionar_chamado(Arvore *arvore);
static void listar_chamados(Arvore *arvore);
static void resolver_chamado(Arvore *arvore, Pilha *historico);
static void desfazer_resolucao(Arvore *arvore, Pilha *historico);

int main(void) {
    Arvore *arvore = arvore_criar();
    Pilha *historico = pilha_criar();

    if (arvore == NULL || historico == NULL) {
        fprintf(stderr, "Erro ao inicializar estruturas\n");
        return 1;
    }

    
    carregar_chamados(arvore);

    int opcao = 0;
    int sair = 0;

    while (!sair) {
        exibir_menu();
        
        if (scanf("%d", &opcao) != 1) {
            fprintf(stderr, "Entrada inválida. Digite um número.\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        switch (opcao) {
            case 1:
                adicionar_chamado(arvore);
                break;
            case 2:
                listar_chamados(arvore);
                break;
            case 3:
                resolver_chamado(arvore, historico);
                break;
            case 4:
                desfazer_resolucao(arvore, historico);
                break;
            case 5:
                salvar_chamados(arvore);
                sair = 1;
                printf("Encerrando sistema. Até logo!\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

        if (!sair) {
            printf("\nPressione ENTER para continuar...");
            getchar();
            printf("\n");
        }
    }

   
    arvore_destruir(arvore->raiz);
    free(arvore);
    pilha_destruir(historico);

    return 0;
}


static void exibir_menu(void) {
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║  Sistema de Chamados de TI por Prioridade  ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("1 - Adicionar chamado\n");
    printf("2 - Listar chamados por prioridade\n");
    printf("3 - Resolver próximo chamado (maior prioridade)\n");
    printf("4 - Desfazer última resolução\n");
    printf("5 - Salvar e sair\n");
    printf("Escolha: ");
}


static void adicionar_chamado(Arvore *arvore) {
    if (arvore == NULL) return;

    printf("\n=== Adicionar Novo Chamado ===\n");

    Chamado novo;
    novo.id = arvore_proximo_id(arvore->raiz);
    novo.data_criacao = time(NULL);

    printf("Descrição do chamado (máx 255 caracteres): ");
    if (fgets(novo.descricao, sizeof(novo.descricao), stdin) == NULL) {
        fprintf(stderr, "Erro ao ler descrição\n");
        return;
    }
    
    novo.descricao[strcspn(novo.descricao, "\n")] = 0;

    if (strlen(novo.descricao) == 0) {
        printf("Descrição não pode estar vazia.\n");
        return;
    }

    printf("\nNível de prioridade:\n");
    printf("  1 - Baixa\n");
    printf("  2 - Média\n");
    printf("  3 - Alta\n");
    printf("  4 - Crítica\n");
    printf("Escolha (1-4): ");

    int prio;
    if (scanf("%d", &prio) != 1) {
        fprintf(stderr, "Entrada inválida\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    if (prio < 1 || prio > 4) {
        printf("Prioridade inválida. Usando Baixa como padrão.\n");
        novo.prioridade = PRIORIDADE_BAIXA;
    } else {
        novo.prioridade = (Prioridade)prio;
    }

    arvore_inserir(arvore, novo);
    printf("\n✓ Chamado #%d adicionado com sucesso!\n", novo.id);
}


static void listar_chamados(Arvore *arvore) {
    if (arvore == NULL) return;

    printf("\n=== Chamados Registrados ===\n");

    if (arvore->quantidade == 0) {
        printf("Nenhum chamado registrado.\n");
        return;
    }

    printf("Total de chamados: %d\n\n", arvore->quantidade);
    arvore_listar_inorder(arvore->raiz);
}


static void resolver_chamado(Arvore *arvore, Pilha *historico) {
    if (arvore == NULL || historico == NULL) return;

    if (arvore->quantidade == 0) {
        printf("\nNenhum chamado para resolver.\n");
        return;
    }

    Chamado *chamado_max = arvore_buscar_max(arvore->raiz);
    if (chamado_max == NULL) {
        printf("\nErro ao buscar chamado de maior prioridade.\n");
        return;
    }

    printf("\n=== Resolvendo Chamado ===\n");
    printf("Chamado #%d (Prioridade: %s)\n", 
           chamado_max->id, 
           prioridade_para_string(chamado_max->prioridade));
    printf("Descrição: %s\n", chamado_max->descricao);

    
    pilha_push(historico, *chamado_max);

    
    arvore_remover_max(arvore);

    printf("\n✓ Chamado resolvido e movido para histórico.\n");
}


static void desfazer_resolucao(Arvore *arvore, Pilha *historico) {
    if (arvore == NULL || historico == NULL) return;

    if (pilha_vazia(historico)) {
        printf("\nNenhuma resolução para desfazer.\n");
        return;
    }

    Chamado *chamado = pilha_pop(historico);
    if (chamado == NULL) {
        printf("\nErro ao recuperar chamado do histórico.\n");
        return;
    }

    printf("\n=== Desfazendo Resolução ===\n");
    printf("Chamado #%d reaberto.\n", chamado->id);

    
    arvore_inserir(arvore, *chamado);

    free(chamado);
    printf("\n✓ Chamado reaberto com sucesso.\n");
}
