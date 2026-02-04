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

//aqui intercala tudo de uma vez
int intercalarArquivosConsulta(char **nomesArquivos, int numArquivos, char *nomeSaida, FILE *log) {
    ArquivoEntradaConsulta *arquivos = malloc(numArquivos * sizeof(ArquivoEntradaConsulta));
    if (!arquivos) return -1;
    
    for (int i = 0; i < numArquivos; i++) {
        arquivos[i].arquivo = fopen(nomesArquivos[i], "rb");
        if (!arquivos[i].arquivo) {
            for (int j = 0; j < i; j++) fclose(arquivos[j].arquivo);
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
        for (int i = 0; i < numArquivos; i++) fclose(arquivos[i].arquivo);
        free(arquivos);
        return -1;
    }
    
    int registrosEscritos = 0;
    
    while (1) {
        int menorIndice = encontrarMenorIdConsulta(arquivos, numArquivos);
        if (menorIndice == -1) break; 
        
        salva_consulta(&arquivos[menorIndice].registroAtual, arquivoSaida);
        registrosEscritos++;
        
        if(log && registrosEscritos % 100 == 0) { 
        }
        
        lerProximoRegistroConsulta(&arquivos[menorIndice]);
    }
    
    for (int i = 0; i < numArquivos; i++) fclose(arquivos[i].arquivo);
    fclose(arquivoSaida);
    free(arquivos);
    
    return registrosEscritos;
}

//aqui intercala de F-1
int intercalacaoOtimaConsulta(char **particoes, int numParticoes, int F, char *nomeSaida, FILE *log) {
    if (numParticoes <= 0) return -1;
    
    if (numParticoes == 1) {
        remove(nomeSaida);
        rename(particoes[0], nomeSaida);
        return 1;
    }

    int rodada = 0;

    while (numParticoes > 1) {
        int novoNumParticoes = 0;
        int numGrupos = (numParticoes + (F - 2)) / (F - 1);

        
        char **novasParticoes = malloc(numGrupos * sizeof(char*));

        for (int i = 0; i < numGrupos; i++) {
            int inicio = i * (F - 1);
            int fim = inicio + (F - 1);

            if (fim > numParticoes) fim = numParticoes;
            
            int tamGrupo = fim - inicio;
            
            char **grupoArquivos = malloc(tamGrupo * sizeof(char*));
            for (int k = 0; k < tamGrupo; k++) {
                grupoArquivos[k] = particoes[inicio + k];
            }

            char nomeTmp[100];
            sprintf(nomeTmp, "temp_rodada%d_grupo%d.dat", rodada, i);
            
            if (log) fprintf(log, "Rodada %d: Intercalando %d arquivos em %s\n", rodada, tamGrupo, nomeTmp);

            intercalarArquivosConsulta(grupoArquivos, tamGrupo, nomeTmp, log);
            
            novasParticoes[novoNumParticoes] = malloc(100);
            strcpy(novasParticoes[novoNumParticoes], nomeTmp);
            novoNumParticoes++;

            free(grupoArquivos);
        }

        for (int i = 0; i < numParticoes; i++) free(particoes[i]);
        free(particoes);

        particoes = novasParticoes;
        numParticoes = novoNumParticoes;
        rodada++;
    }

    remove(nomeSaida);
    rename(particoes[0], nomeSaida);
    
    free(particoes[0]);
    free(particoes);

    return 1;
}

//aqui coredena tudo usando as funções a cima e junta a seleçãoPorSubtituição
int ordenarPorIntercalacaoOtimaConsulta(FILE *arqConsultas, int memDisponivel, int totalRegistros, int F, FILE *log) {
    clock_t inicioT = clock();
    
    int numRuns = selecaoSubstituicaoConsulta(arqConsultas, memDisponivel, totalRegistros, log);
    
    if (numRuns <= 0) {
        if(log) fprintf(log, "Erro: Nenhuma run gerada.\n");
        return -1;
    }
    
    char **nomesArquivos = malloc(numRuns * sizeof(char*));
    for (int i = 0; i < numRuns; i++) {
        nomesArquivos[i] = malloc(100);
        sprintf(nomesArquivos[i], "run_consultas_%d.dat", i);
    }
    
    intercalacaoOtimaConsulta(nomesArquivos, numRuns, F, "consultas_ordenadas.dat", log);
    

    clock_t fimT = clock();
    double tempo_total = ((double)(fimT - inicioT)) / CLOCKS_PER_SEC;
    
    if (log != NULL) {
        fprintf(log, "\nTempo total de ordenacao (Consultas - Otima): %.6f segundos\n", tempo_total);
    }
    
    return 1;
}