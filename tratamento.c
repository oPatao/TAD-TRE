#include "tratamento.h"

Lista criar_lista() {
    return NULL;
}

Lista inserir_final(Lista l, Unidade u) {
    Nodo *novo = (Nodo*) malloc(sizeof(Nodo));
    novo->info = u;
    novo->prox = NULL;
    
    if (l == NULL) return novo;
    
    Nodo *atual = l;
    while (atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = novo;
    return l;
}

void liberar_lista(Lista l) {
    Nodo *atual = l;
    while (atual != NULL) {
        Nodo *temp = atual;
        atual = atual->prox;
        free(temp);
    }
}

Lista carregar_csv(Lista l, char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) return l;

    char buffer[2048];
    fgets(buffer, 2048, f); // Pula o cabeçalho

    while (fgets(buffer, 2048, f)) {
        Unidade u;
        buffer[strcspn(buffer, "\r\n")] = 0;

        char *token = strtok(buffer, ",");
        int col = 0;
        
        while (token != NULL) {
            if (token[0] == '"') { token++; token[strlen(token)-1] = '\0'; }

            // Agora guardamos todas as colunas de texto (0 a 9)
            if (col == 0) strcpy(u.sigla_tribunal, token);
            else if (col == 1) strcpy(u.procedimento, token);
            else if (col == 2) strcpy(u.ramo_justica, token);
            else if (col == 3) strcpy(u.sigla_grau, token);
            else if (col == 4) strcpy(u.uf_oj, token);
            else if (col == 5) strcpy(u.municipio_oj, token);
            else if (col == 6) u.id_ultimo_oj = atoi(token);
            else if (col == 7) strcpy(u.nome, token);
            else if (col == 8) strcpy(u.mesano_cnm1, token);
            else if (col == 9) strcpy(u.mesano_sent, token);
            
            // Guarda os dados numéricos (10 a 32)
            else if (col >= 10 && col <= 32) {
                if (strcmp(token, "NA") == 0 || strlen(token) == 0) {
                    u.dados[col-10] = 0;
                } else {
                    u.dados[col-10] = atof(token);
                }
            }
            token = strtok(NULL, ",");
            col++;
        }
        l = inserir_final(l, u); 
    }
    fclose(f);
    return l;
}

void calcular_meta2(Lista l) {
    float soma_julgm2 = 0, soma_distm2 = 0, soma_suspm2 = 0;
    Nodo *atual = l;
    while (atual != NULL) {
        soma_distm2 += atual->info.dados[6];
        soma_julgm2 += atual->info.dados[7];
        soma_suspm2 += atual->info.dados[8];
        atual = atual->prox;
    }
    
    if ((soma_distm2 - soma_suspm2) != 0) {
        float meta = (soma_julgm2 / (soma_distm2 - soma_suspm2)) * 100;
        printf("Meta 2 (A): %.2f%%\n", meta);
    } else {
        printf("Meta 2 (A): N/A (Divisao por zero)\n");
    }
}

void calcular_meta4(Lista l) {
    float s_j4a = 0, s_d4a = 0, s_s4a = 0;
    float s_j4b = 0, s_d4b = 0, s_s4b = 0;
    Nodo *atual = l;
    while (atual != NULL) {
        s_d4a += atual->info.dados[15]; 
        s_j4a += atual->info.dados[16]; 
        s_s4a += atual->info.dados[17]; 
        
        s_d4b += atual->info.dados[19]; 
        s_j4b += atual->info.dados[20]; 
        s_s4b += atual->info.dados[21]; 
        atual = atual->prox;
    }
    
    float divA = s_d4a - s_s4a;
    float divB = s_d4b - s_s4b;
    
    if(divA != 0) printf("Meta 4A: %.2f%%\n", (s_j4a / divA) * 100);
    else printf("Meta 4A: N/A\n");
    
    if(divB != 0) printf("Meta 4B: %.2f%%\n", (s_j4b / divB) * 100);
    else printf("Meta 4B: N/A\n");
}
void gerar_resumo_tribunais(Lista l) {
    char *estados[27] = {
        "AC", "AL", "AM", "AP", "BA", "CE", "DF", "ES", "GO", 
        "MA", "MG", "MS", "MT", "PA", "PB", "PE", "PI", "PR", 
        "RJ", "RN", "RO", "RR", "RS", "SC", "SE", "SP", "TO"
    };

    FILE *f = fopen("resumo_tribunais.csv", "w");
    if (!f) {
        printf("Erro ao criar o arquivo de resumo.\n");
        return;
    }

    fprintf(f, "sigla_tribunal,total_julgados_2026,Meta1,Meta2A,Meta2Ant,Meta4A,Meta4B\n");

    for (int i = 0; i < 27; i++) {
        char sigla_alvo[20];
        sprintf(sigla_alvo, "TRE-%s", estados[i]);

        float sum_casos_novos = 0, sum_julgados = 0;
        float sum_d2a = 0, sum_j2a = 0, sum_s2a = 0;
        float sum_d2ant = 0, sum_j2ant = 0, sum_s2ant = 0;
        float sum_d4a = 0, sum_j4a = 0, sum_s4a = 0;
        float sum_d4b = 0, sum_j4b = 0, sum_s4b = 0;

        Nodo *atual = l;
        int tem_dados = 0;

        while (atual != NULL) {
            if (strcmp(atual->info.sigla_tribunal, sigla_alvo) == 0) {
                tem_dados = 1;
                sum_casos_novos += atual->info.dados[0]; 
                sum_julgados    += atual->info.dados[1]; 
                sum_d2a += atual->info.dados[6];  
                sum_j2a += atual->info.dados[7];  
                sum_s2a += atual->info.dados[8];  

                sum_d2ant += atual->info.dados[10]; 
                sum_j2ant += atual->info.dados[11]; 
                sum_s2ant += atual->info.dados[12]; 

                sum_d4a += atual->info.dados[15]; 
                sum_j4a += atual->info.dados[16]; 
                sum_s4a += atual->info.dados[17]; 

                sum_d4b += atual->info.dados[19]; 
                sum_j4b += atual->info.dados[20]; 
                sum_s4b += atual->info.dados[21]; 
            }
            atual = atual->prox;
        }

        if (tem_dados) {
            float meta1 = (sum_casos_novos != 0) ? (sum_julgados / sum_casos_novos) * 100 : 0;
            float meta2a = ((sum_d2a - sum_s2a) != 0) ? (sum_j2a / (sum_d2a - sum_s2a)) * 100 : 0;
            float meta2ant = ((sum_d2ant - sum_s2ant) != 0) ? (sum_j2ant / (sum_d2ant - sum_s2ant)) * 100 : 0;
            float meta4a = ((sum_d4a - sum_s4a) != 0) ? (sum_j4a / (sum_d4a - sum_s4a)) * 100 : 0;
            float meta4b = ((sum_d4b - sum_s4b) != 0) ? (sum_j4b / (sum_d4b - sum_s4b)) * 100 : 0;

            fprintf(f, "%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                    sigla_alvo, sum_julgados, meta1, meta2a, meta2ant, meta4a, meta4b);
        }
    }

    fclose(f);
    printf("\n[Item 2] Arquivo 'resumo_tribunais.csv' gerado com sucesso!\n");
}

void gerar_csv_municipio(Lista l, char *municipio) {
    char nome_saida[150];
    sprintf(nome_saida, "%s.csv", municipio);
    FILE *f = fopen(nome_saida, "w");

    fprintf(f, "sigla_tribunal,procedimento,ramo_justica,sigla_grau,uf_oj,municipio_oj,id_ultimo_oj,nome,mesano_cnm1,mesano_sent,casos_novos_2026,julgados_2026,prim_sent2026,suspensos_2026,dessobrestados_2026,cumprimento_meta1,distm2_a,julgm2_a,suspm2_a,cumprimento_meta2a,distm2_ant,julgm2_ant,suspm2_ant,desom2_ant,cumprimento_meta2ant,distm4_a,julgm4_a,suspm4_a,cumprimento_meta4a,distm4_b,julgm4_b,suspm4_b,cumprimento_meta4b\n"); 
    
    Nodo *atual = l;
    int encontrou = 0;
    
    while (atual != NULL) {
        if (strcasecmp(atual->info.municipio_oj, municipio) == 0) {
            fprintf(f, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%d,\"%s\",\"%s\",\"%s\"", 
                    atual->info.sigla_tribunal, atual->info.procedimento, 
                    atual->info.ramo_justica, atual->info.sigla_grau, 
                    atual->info.uf_oj, atual->info.municipio_oj, 
                    atual->info.id_ultimo_oj, atual->info.nome, 
                    atual->info.mesano_cnm1, atual->info.mesano_sent);
         
            for(int i = 0; i < 23; i++) {
                fprintf(f, ",%.2f", atual->info.dados[i]);
            }
            fprintf(f, "\n");
            encontrou++;
        }
        atual = atual->prox;
    }
    fclose(f);
    printf("Arquivo %s gerado com sucesso! (%d registros encontrados)\n", nome_saida, encontrou);
}