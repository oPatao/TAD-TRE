#include "tratamento.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *estados[27] = {
        "AC", "AL", "AM", "AP", "BA", "CE", "DF", "ES", "GO", 
        "MA", "MG", "MS", "MT", "PA", "PB", "PE", "PI", "PR", 
        "RJ", "RN", "RO", "RR", "RS", "SC", "SE", "SP", "TO"
    };

    Lista lista_geral = criar_lista();
    char nome_arquivo[50];

    printf("Carregando e concatenando base de dados dos TREs...\n");

    for (int i = 0; i < 27; i++) {
        sprintf(nome_arquivo, "teste_TRE-%s.csv", estados[i]);
        lista_geral = carregar_csv(lista_geral, nome_arquivo);
    }

    if (lista_geral != NULL) {
        
        gerar_resumo_tribunais(lista_geral);
        
        char municipio_alvo[100];
        printf("\n[Item 3] Informe o municipio para gerar o relatorio CSV: ");
        
        scanf(" %99[^\n]", municipio_alvo);

        for(int j = 0; municipio_alvo[j] != '\0'; j++){
            municipio_alvo[j] = toupper(municipio_alvo[j]);
        }

        gerar_csv_municipio(lista_geral, municipio_alvo);
        liberar_lista(lista_geral);
    } else {
        printf("Nenhum arquivo CSV encontrado.\n");
    }

    return 0;
}