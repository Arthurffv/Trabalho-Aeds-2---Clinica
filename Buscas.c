#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Buscas.h"
#include "Medico.h"
#include "Paciente.h"
#include "consulta.h"


Tmedico *Medico_buscaSequencial_PorId(FILE *in, int chave, FILE *log){
    rewind(in);
    Tmedico *med;
    int cont = 0;
    clock_t inicioT = clock();

    while ((med = le_medico(in)) != NULL){
        cont++;
        if (med->id == chave){
            clock_t fimT = clock();
            double total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;
            fprintf(log, "\n---------------------------");
            fprintf(log, "\nMedico encontrado!");
            fprintf(log, "\nComparacoes Sequencial: %d ", cont);
            fprintf(log, "\nTempo Sequencial: %f ", total);
            fprintf(log, "\n---------------------------\n");
            return med;
        }
        free(med);
    }

    clock_t fimT = clock();
    double total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;
    printf("Medico não encontrado\n");
    fprintf(log, "\n---------------------------");
    fprintf(log, "\nMedico não encontrado!");
    fprintf(log, "\nComparacoes Sequencial: %d ", cont);
    fprintf(log, "\nTempo Sequencial: %f ", total);
    fprintf(log, "\n---------------------------\n");
    return NULL;
}


Tpaciente *Paciente_buscaSequencial_PorId(FILE *in, int chave, FILE *log){
    rewind(in);
    Tpaciente *pac;
    int cont = 0;
    clock_t inicioT = clock();

    while ((pac = le_paciente(in)) != NULL){
        cont++;
        if (pac->id == chave){
            clock_t fimT = clock();
            double total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;
            fprintf(log, "\n---------------------------");
            fprintf(log, "\nPaciente encontrado!");
            fprintf(log, "\nComparacoes Sequencial: %d ", cont);
            fprintf(log, "\nTempo Sequencial: %f ", total);
            fprintf(log, "\n---------------------------\n");
            return pac;
        }
        free(pac);
    }

    clock_t fimT = clock();
    double total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;
    printf("Paciente não encontrado\n");
    fprintf(log, "\n---------------------------");
    fprintf(log, "\nPaciente não encontrado!");
    fprintf(log, "\nComparacoes Sequencial: %d ", cont);
    fprintf(log, "\nTempo Sequencial: %f ", total);
    fprintf(log, "\n---------------------------\n");
    return NULL;
}


Tconsulta *Consulta_buscaSequencial_PorId(FILE *in, int chave, FILE *log){
    rewind(in);
    Tconsulta *con;
    int cont = 0;
    clock_t inicioT = clock();

    while ((con = le_consulta(in)) != NULL){
        cont++;
        if (con->id == chave){
            clock_t fimT = clock();
            double total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;
            fprintf(log, "\n---------------------------");
            fprintf(log, "\nConsulta encontrada!");
            fprintf(log, "\nComparacoes Sequencial: %d ", cont);
            fprintf(log, "\nTempo Sequencial: %f ", total);
            fprintf(log, "\n---------------------------\n");
            return con;
        }
        free(con);
    }

    clock_t fimT = clock();
    double total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;
    printf("Consulta não encontrada\n");
    fprintf(log, "\n---------------------------");
    fprintf(log, "\nConsulta não encontrada!");
    fprintf(log, "\nComparacoes Sequencial: %d ", cont);
    fprintf(log, "\nTempo Sequencial: %f ", total);
    fprintf(log, "\n---------------------------\n");
    return NULL;
}


Tmedico *Medico_buscaBinaria_PorId(FILE *in, int chave, int inicio, int fim, FILE *log){
    int cod = -1;
    Tmedico *med = NULL;
    int cont = 0;
    clock_t inicioT = clock();

    while (inicio <= fim){
        int meio = inicio + (fim - inicio) / 2;

        fseek(in, meio * (long)tamanho_registro_medico(), SEEK_SET);
        med = le_medico(in);
        if (med == NULL) break;

        cod = med->id;
        cont++;

        if (cod == chave){
            clock_t fimT = clock();
            double total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;
            fprintf(log, "\n---------------------------");
            fprintf(log, "\nMedico encontrado!");
            fprintf(log, "\nComparacoes Binaria: %d ", cont);
            fprintf(log, "\nTempo Binaria: %f ", total);
            fprintf(log, "\n---------------------------\n");
            return med;
        } else if (cod > chave) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
        free(med);
    }

    clock_t fimT = clock();
    double total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;
    fprintf(log, "\n---------------------------");
    fprintf(log, "\nMedico não encontrado!");
    fprintf(log, "\nComparacoes Binaria: %d ", cont);
    fprintf(log, "\nTempo Binaria: %f ", total);
    fprintf(log, "\n---------------------------\n");
    return NULL;
}

Tpaciente *Paciente_buscaBinaria_PorId(FILE *in, int chave, int inicio, int fim, FILE *log){
    int cod = -1;
    Tpaciente *pac = NULL;
    int cont = 0;
    clock_t inicioT = clock();

    while (inicio <= fim){
        int meio = inicio + (fim - inicio) / 2;

        fseek(in, meio * (long)tamanho_registro_paciente(), SEEK_SET);
        pac = le_paciente(in);
        if (pac == NULL) break;

        cod = pac->id;
        cont++;

        if (cod == chave){
            clock_t fimT = clock();
            double total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;
            fprintf(log, "\n---------------------------");
            fprintf(log, "\nPaciente encontrado!");
            fprintf(log, "\nComparacoes Binaria: %d ", cont);
            fprintf(log, "\nTempo Binaria: %f ", total);
            fprintf(log, "\n---------------------------\n");
            return pac;
        } else if (cod > chave) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
        free(pac);
    }

    clock_t fimT = clock();
    double total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;
    fprintf(log, "\n---------------------------");
    fprintf(log, "\nPaciente não encontrado!");
    fprintf(log, "\nComparacoes Binaria: %d ", cont);
    fprintf(log, "\nTempo Binaria: %f ", total);
    fprintf(log, "\n---------------------------\n");
    return NULL;
}

Tconsulta *Consulta_buscaBinaria_PorId(FILE *in, int chave, int inicio, int fim, FILE *log){
    int cod = -1;
    Tconsulta *con = NULL;
    int cont = 0;
    clock_t inicioT = clock();

    while (inicio <= fim){
        int meio = inicio + (fim - inicio) / 2;

        fseek(in, meio * (long)tamanho_registro_consulta(), SEEK_SET);
        con = le_consulta(in);
        if (con == NULL) break;

        cod = con->id;
        cont++;

        if (cod == chave){
            clock_t fimT = clock();
            double total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;
            fprintf(log, "\n---------------------------");
            fprintf(log, "\nConsulta encontrada!");
            fprintf(log, "\nComparacoes Binaria: %d ", cont);
            fprintf(log, "\nTempo Binaria: %f ", total);
            fprintf(log, "\n---------------------------\n");
            return con;
        } else if (cod > chave) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
        free(con);
    }

    clock_t fimT = clock();
    double total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;
    fprintf(log, "\n---------------------------");
    fprintf(log, "\nConsulta não encontrada!");
    fprintf(log, "\nComparacoes Binaria: %d ", cont);
    fprintf(log, "\nTempo Binaria: %f ", total);
    fprintf(log, "\n---------------------------\n");
    return NULL;
}
