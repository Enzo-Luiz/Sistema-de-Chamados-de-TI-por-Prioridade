#include "chamados.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* Converter prioridade para string */
const char* prioridade_para_string(Prioridade p) {
    switch (p) {
        case PRIORIDADE_BAIXA:
            return "Baixa";
        case PRIORIDADE_MEDIA:
            return "Média";
        case PRIORIDADE_ALTA:
            return "Alta";
        case PRIORIDADE_CRITICA:
            return "Crítica";
        default:
            return "Desconhecida";
    }
}

/* Converter string para prioridade */
Prioridade string_para_prioridade(const char *str) {
    if (str == NULL) return PRIORIDADE_BAIXA;

    char str_lower[20];
    strncpy(str_lower, str, sizeof(str_lower) - 1);
    str_lower[sizeof(str_lower) - 1] = '\0';

    /* Converter para minúsculas */
    for (int i = 0; str_lower[i]; i++) {
        str_lower[i] = tolower((unsigned char)str_lower[i]);
    }

    if (strcmp(str_lower, "baixa") == 0 || strcmp(str_lower, "1") == 0) {
        return PRIORIDADE_BAIXA;
    } else if (strcmp(str_lower, "média") == 0 || strcmp(str_lower, "media") == 0 || strcmp(str_lower, "2") == 0) {
        return PRIORIDADE_MEDIA;
    } else if (strcmp(str_lower, "alta") == 0 || strcmp(str_lower, "3") == 0) {
        return PRIORIDADE_ALTA;
    } else if (strcmp(str_lower, "crítica") == 0 || strcmp(str_lower, "critica") == 0 || strcmp(str_lower, "4") == 0) {
        return PRIORIDADE_CRITICA;
    }

    return PRIORIDADE_BAIXA;
}

/* Limpar buffer de entrada */
void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Exibir um chamado formatado */
void exibir_chamado(Chamado *chamado) {
    if (chamado == NULL) return;

    char data_str[30];
    struct tm *tm_info = localtime(&(chamado->data_criacao));
    strftime(data_str, sizeof(data_str), "%d/%m/%Y %H:%M:%S", tm_info);

    printf("  ID: %d | Prioridade: %-8s | Data: %s\n", 
           chamado->id, 
           prioridade_para_string(chamado->prioridade),
           data_str);
    printf("  Descrição: %s\n", chamado->descricao);
    printf("  ---\n");
}
