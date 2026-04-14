#ifndef TRATAMENTO_H
#define TRATAMENTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char sigla_tribunal[20];
    char procedimento[100];
    char ramo_justica[50];
    char sigla_grau[5];
    char uf_oj[5];
    char municipio_oj[100];
    int id_ultimo_oj;
    char nome[150];
    char mesano_cnm1[20];
    char mesano_sent[20];
    float dados[23]; 
} Unidade;

typedef struct Nodo {
    Unidade info;
    struct Nodo *prox;
} Nodo;

typedef Nodo* Lista;

Lista criar_lista();
Lista inserir_final(Lista l, Unidade u);
void liberar_lista(Lista l);
Lista carregar_csv(Lista l, char *nome_arquivo); 

void gerar_resumo_tribunais(Lista l); 

void gerar_csv_municipio(Lista l, char *municipio);

#endif