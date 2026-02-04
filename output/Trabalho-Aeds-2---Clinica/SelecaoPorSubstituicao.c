#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "consulta.h" 
#include "SelecaoPorSubstituicao.h"

int selecaoSubstituicaoConsulta(FILE *arqConsultas, int memDisponivel, int totalRegistros, FILE *log) {
    clock_t inicioT = clock();
    long comparacoes = 0;
    
    Tconsulta *reservatorio = malloc(memDisponivel * sizeof(Tconsulta));
    int *congelado = calloc(memDisponivel, sizeof(int));
    
    if (!reservatorio || !congelado) {
        fprintf(stderr, "Erro ao alocar memória para Seleção com Substituição.\n");
        if(reservatorio) free(reservatorio);
        if(congelado) free(congelado);
        return -1;
    }
    
    rewind(arqConsultas);
    
    int numRuns = 0;
    int registrosProcessados = 0;
    int registrosLidos = 0;

    //carregamento M registros
    for (int i = 0; i < memDisponivel && registrosLidos < totalRegistros; i++) {
        Tconsulta *c = le_consulta(arqConsultas);
        if (c) {
            reservatorio[i] = *c;
            free(c);
            registrosLidos++;
        } else {
            congelado[i] = 1; 
        }
    }
    

    while (registrosProcessados < totalRegistros) {
        numRuns++;
        char nomeParticao[100];
        sprintf(nomeParticao, "run_consultas_%d.dat", numRuns - 1);
        
        FILE *arqSaida = fopen(nomeParticao, "wb");
        if (!arqSaida) {
            fprintf(stderr, "Erro ao criar partição %s.\n", nomeParticao);
            free(reservatorio);
            free(congelado);
            return -1;
        }
        
        int ultimoId = -1;
        int registrosAtivos = 0;
        
        for (int i = 0; i < memDisponivel; i++) {
            if (!congelado[i] && (registrosLidos > 0 || registrosProcessados < totalRegistros)) { 
                 if (i < registrosLidos || registrosProcessados < registrosLidos) registrosAtivos++;
            }
        }
        registrosAtivos = 0;
        for(int i=0; i<memDisponivel; i++) {
            if(!congelado[i]) registrosAtivos++;
        }

        while (registrosAtivos > 0) {
            int minIndice = -1;
            int minId = INT_MAX;
            
            //seleção de menor registro ativo
            for (int i = 0; i < memDisponivel; i++) {
                if (!congelado[i]) {
                    comparacoes++;
                    if (reservatorio[i].id < minId) {
                        minId = reservatorio[i].id;
                        minIndice = i;
                    }
                }
            }
            
            if (minIndice == -1) {
                break;
            }
            

            salva_consulta(&reservatorio[minIndice], arqSaida);
            ultimoId = reservatorio[minIndice].id;
            registrosProcessados++;
            
            if (registrosLidos < totalRegistros) {
                Tconsulta *novaC = le_consulta(arqConsultas);
                if (novaC) {
                    reservatorio[minIndice] = *novaC;
                    free(novaC);
                    registrosLidos++;

                    //congelamento
                    if (reservatorio[minIndice].id < ultimoId) {
                        congelado[minIndice] = 1;
                        registrosAtivos--;
                    }
                } else {
                    congelado[minIndice] = 1;
                    registrosAtivos--;
                }
            } else {
                congelado[minIndice] = 1;
                registrosAtivos--;
            }
        }
        
        fclose(arqSaida);
        

        for (int i = 0; i < memDisponivel; i++) {

            if (registrosProcessados < totalRegistros)
                congelado[i] = 0;             
        }

    }
    
    free(reservatorio);
    free(congelado);
    
    clock_t fimT = clock();
    double tempo_execucao = ((double)(fimT - inicioT)) / CLOCKS_PER_SEC;
    
    if (log != NULL) {
        fprintf(log, "\n--------------%d Execucao----------------\n", numRuns);
        fprintf(log, "Algoritmo: Selecao com Substituicao (Partições de Consultas)\n");
        fprintf(log, "Tempo de Execucao: %.6f segundos\n", tempo_execucao);
        fprintf(log, "Comparacoes: %ld\n", comparacoes);
        fprintf(log, "Numero de corridas geradas: %d\n", numRuns);
        fprintf(log, "Arquivos gerados: run_consultas_0.dat, ...\n");
        fprintf(log, "------------------------------\n");
    }
    
    printf("\nCriação de corridas de Consultas finalizada. %d corridas foram geradas.\n", numRuns);
    
    return numRuns;
}