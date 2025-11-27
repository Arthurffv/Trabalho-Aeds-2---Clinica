#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "consulta.h"
#include "SelecaoPorSubstituicao.h"
#include "IntercalacaoOtimaConsulta.h"

typedef struct {
    FILE *arquivo;
    Tconsulta registroAtual;
    int ativo;
    char nomeArquivo[100];
} ArquivoEntradaConsulta;

int selecaoSubstituicaoConsulta(FILE *arqConsultas, int memDisponivel, int totalRegistros, FILE *log);

int lerProximoRegistroConsulta(ArquivoEntradaConsulta *arquivo) {
    Tconsulta *c = le_consulta(arquivo->arquivo);
    if (c) {
        arquivo->registroAtual = *c;
        free(c);
        return 1;
    } else {
        arquivo->ativo = 0;
        return 0;
    }
}

int encontrarMenorIdConsulta(ArquivoEntradaConsulta *arquivos, int numArquivos) {
    int menorIndice = -1;
    int menorId = INT_MAX;
    
    for (int i = 0; i < numArquivos; i++) {
        if (arquivos[i].ativo && arquivos[i].registroAtual.id < menorId) {
            menorId = arquivos[i].registroAtual.id;
            menorIndice = i;
        }
    }
    
    return menorIndice;
}

int intercalarArquivosConsulta(char **nomesArquivos, int numArquivos, char *nomeSaida, FILE *log) {
    clock_t inicioT = clock();
    long comparacoes = 0;
    
    ArquivoEntradaConsulta *arquivos = malloc(numArquivos * sizeof(ArquivoEntradaConsulta));
    if (!arquivos) {
        fprintf(stderr, "Erro ao alocar memória para arquivos de consulta.\n");
        return -1;
    }
    
    for (int i = 0; i < numArquivos; i++) {
        char caminhoCompleto[200];
        sprintf(caminhoCompleto, "%s", nomesArquivos[i]);
        arquivos[i].arquivo = fopen(caminhoCompleto, "rb");
        
        if (!arquivos[i].arquivo) {
            fprintf(stderr, "Erro ao abrir arquivo %s.\n", nomesArquivos[i]);
            for (int j = 0; j < i; j++) {
                fclose(arquivos[j].arquivo);
            }
            free(arquivos);
            return -1;
        }
        
        strcpy(arquivos[i].nomeArquivo, nomesArquivos[i]);
        arquivos[i].ativo = 1;
        
        if (!lerProximoRegistroConsulta(&arquivos[i])) {
            arquivos[i].ativo = 0;
        }
    }
    
    FILE *arquivoSaida = fopen(nomeSaida, "wb");
    if (!arquivoSaida) {
        fprintf(stderr, "Erro ao criar arquivo de saída %s.\n", nomeSaida);
        for (int i = 0; i < numArquivos; i++) {
            fclose(arquivos[i].arquivo);
        }
        free(arquivos);
        return -1;
    }
    
    int registrosEscritos = 0;
    
    while (1) {
        int menorIndice = encontrarMenorIdConsulta(arquivos, numArquivos);
        
        if (menorIndice == -1) {
            break;
        }
        
        salva_consulta(&arquivos[menorIndice].registroAtual, arquivoSaida);
        registrosEscritos++;
        comparacoes++;
        
        lerProximoRegistroConsulta(&arquivos[menorIndice]);
    }
    
    for (int i = 0; i < numArquivos; i++) {
        fclose(arquivos[i].arquivo);
    }
    fclose(arquivoSaida);
    free(arquivos);
    
    clock_t fimT = clock();
    double tempo_execucao = ((double)(fimT - inicioT)) / CLOCKS_PER_SEC;
    
    if (log != NULL) {
        fprintf(log, "\n------------------------------\n");
        fprintf(log, "Algoritmo: Intercalação Ótima (Consultas)\n");
        fprintf(log, "Tempo de Execução: %.6f segundos\n", tempo_execucao);
        fprintf(log, "Comparações: %ld\n", comparacoes);
        fprintf(log, "Registros escritos: %d\n", registrosEscritos);
        fprintf(log, "Arquivo de saída: %s\n", nomeSaida);
        fprintf(log, "------------------------------\n");
    }
    
    return registrosEscritos;
}

int intercalacaoOtimaConsulta(char **nomesArquivos, int numArquivos, char *nomeSaida, FILE *log) {
    if (numArquivos <= 0) {
        fprintf(stderr, "Número inválido de arquivos para intercalar.\n");
        return -1;
    }
    
    if (numArquivos == 1) {
        char caminhoCompleto[200];
        char caminhoCompleto2[200];
        sprintf(caminhoCompleto, "%s", nomesArquivos[0]);
        sprintf(caminhoCompleto2, "%s", nomeSaida);
        
        FILE *origem = fopen(caminhoCompleto, "rb");
        FILE *destino = fopen(caminhoCompleto2, "wb");
        
        if (!origem || !destino) {
            fprintf(stderr, "Erro ao copiar arquivo único de consulta.\n");
            if (origem) fclose(origem);
            if (destino) fclose(destino);
            return -1;
        }
        
        Tconsulta *c;
        int registrosCopiados = 0;
        while ((c = le_consulta(origem)) != NULL) {
            salva_consulta(c, destino);
            free(c);
            registrosCopiados++;
        }
        
        fclose(origem);
        fclose(destino);
        
        if (log != NULL) {
            fprintf(log, "\n------------------------------\n");
            fprintf(log, "Algoritmo: Intercalação Ótima - Consultas (Cópia Simples)\n");
            fprintf(log, "Registros copiados: %d\n", registrosCopiados);
            fprintf(log, "Arquivo de saída: %s\n", nomeSaida);
            fprintf(log, "------------------------------\n");
        }
        
        return registrosCopiados;
    }
    
    return intercalarArquivosConsulta(nomesArquivos, numArquivos, nomeSaida, log);
}

int ordenarPorIntercalacaoOtimaConsulta(FILE *arqConsultas, int memDisponivel, int totalRegistros, FILE *log) {
    clock_t inicioT = clock();
    int numRuns = selecaoSubstituicaoConsulta(arqConsultas, memDisponivel, totalRegistros, log);
    
    if (numRuns <= 0) {
        fprintf(stderr, "Erro na criacao das corridas de consulta.\n");
        return -1;
    }
    
    char **nomesArquivos = malloc(numRuns * sizeof(char*));
    if (!nomesArquivos) {
        fprintf(stderr, "Erro ao alocar memoria para nomes de arquivos.\n");
        return -1;
    }
    
    for (int i = 0; i < numRuns; i++) {
        nomesArquivos[i] = malloc(100 * sizeof(char));
        sprintf(nomesArquivos[i], "run_consultas_%d.dat", i); 
    }
    
    int resultado = intercalacaoOtimaConsulta(nomesArquivos, numRuns, "consultas_ordenadas.dat", log);
    
    for (int i = 0; i < numRuns; i++) {

        free(nomesArquivos[i]);
    }
    free(nomesArquivos);
    
    clock_t fimT = clock();
    double tempo_total = ((double)(fimT - inicioT)) / CLOCKS_PER_SEC;
    
    if (log != NULL) {
        fprintf(log, "\nTempo total de ordenacao (Consultas): %.6f segundos\n", tempo_total);
    }
    printf("\nOrdenacao de Consultas por Intercalacao Otima concluida!\n");
    printf("Arquivo ordenado: consultas_ordenadas.dat\n");
    
    return resultado;
}