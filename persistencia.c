#define _XOPEN_SOURCE 700
#include "chamados.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Carregar chamados do arquivo CSV */
void carregar_chamados(Arvore *arvore) {
    if (arvore == NULL) return;

    FILE *arquivo = fopen(ARQUIVO_DADOS, "r");
    if (arquivo == NULL) {
        /* Arquivo não existe (primeira execução) */
        printf("Arquivo de dados não encontrado. Iniciando com lista vazia.\n");
        return;
    }

    char linha[512];
    int primeira_linha = 1;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        /* Pular linha de cabeçalho */
        if (primeira_linha) {
            primeira_linha = 0;
            continue;
        }

        /* Remover newline */
        linha[strcspn(linha, "\n")] = 0;

        if (strlen(linha) == 0) continue;

        /* Parse da linha CSV */
        Chamado chamado;
        char prioridade_str[20];
        char data_str[30];
        
        int campos = sscanf(linha, "%d,%255[^,],%19[^,],%29[^\n]",
                   &chamado.id,
                   chamado.descricao,
                   prioridade_str,
                   data_str);",
                        

        if (campos != 4) {
            fprintf(stderr, "Erro ao parsear linha: %s\n", linha);
            continue;
        }

        chamado.prioridade = string_para_prioridade(prioridade_str);
        
        /* Parse da data (formato: YYYY-MM-DD HH:MM:SS) */
        struct tm tm_info = {0};
        if (strptime(data_str, "%Y-%m-%d %H:%M:%S", &tm_info) == NULL) {
            fprintf(stderr, "Erro ao parsear data: %s\n", data_str);
            chamado.data_criacao = time(NULL);
        } else {
            chamado.data_criacao = mktime(&tm_info);
        }

        arvore_inserir(arvore, chamado);
    }

    fclose(arquivo);
    printf("Carregados %d chamados do arquivo.\n", arvore->quantidade);
}

/* Função auxiliar para salvar recursivamente */
static void salvar_recursivo(FILE *arquivo, NoArvore *no) {
    if (no == NULL) return;

    /* Salvar em ordem (esquerda, raiz, direita) */
    salvar_recursivo(arquivo, no->esquerda);

    /* Formatar data */
    char data_str[30];
    struct tm *tm_info = localtime(&(no->chamado.data_criacao));
    strftime(data_str, sizeof(data_str), "%Y-%m-%d %H:%M:%S", tm_info);

    /* Escrever linha CSV */
    fprintf(arquivo, "%d,%s,%s,%s\n",
           no->chamado.id,
           no->chamado.descricao,
           prioridade_para_string(no->chamado.prioridade),
           data_str);

    salvar_recursivo(arquivo, no->direita);
}

/* Salvar chamados no arquivo CSV */
void salvar_chamados(Arvore *arvore) {
    if (arvore == NULL) return;

    FILE *arquivo = fopen(ARQUIVO_DADOS, "w");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo para escrita\n");
        return;
    }

    /* Escrever cabeçalho */
    fprintf(arquivo, "id,descricao,prioridade,data_criacao\n");

    salvar_recursivo(arquivo, arvore->raiz);
    fclose(arquivo);
    printf("Chamados salvos com sucesso.\n");
}
