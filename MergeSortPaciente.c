#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Paciente.h"

static void mergeSortRecursivoPaciente(
    FILE *arq,
    int inicio,
    int fim
) {
    if (inicio >= fim) return;

    int meio = (inicio + fim) / 2;
    mergeSortRecursivoPaciente(arq, inicio, meio);
    mergeSortRecursivoPaciente(arq, meio + 1, fim);

    Tpaciente regEsq;
    Tpaciente regDir;
    Tpaciente regTemp;

    size_t tam = tamanho_registro_paciente();
    long tam_long = (long)tam;

    FILE *tempFile = tmpfile();
    if (!tempFile) {
        fprintf(stderr, "Erro ao criar arquivo temporário.\n");
        return;
    }

    int i = inicio, j = meio + 1, k = 0;

    while (i <= meio && j <= fim) {

        fseek(arq, i * tam_long, SEEK_SET);
        fread(&regEsq, tam, 1, arq);

        fseek(arq, j * tam_long, SEEK_SET);
        fread(&regDir, tam, 1, arq);


        if (regEsq.id <= regDir.id) {
            fwrite(&regEsq, tam, 1, tempFile);
            i++;
        } else {
            fwrite(&regDir, tam, 1, tempFile);
            j++;
        }
        k++;
    }


    while (i <= meio) {
        fseek(arq, i++ * tam_long, SEEK_SET);
        fread(&regEsq, tam, 1, arq);
        fwrite(&regEsq, tam, 1, tempFile);
        k++;
    }


    while (j <= fim) {
        fseek(arq, j++ * tam_long, SEEK_SET);
        fread(&regDir, tam, 1, arq);
        fwrite(&regDir, tam, 1, tempFile);
        k++;
    }


    rewind(tempFile);


    for (int x = 0; x < k; x++) {
        fread(&regTemp, tam, 1, tempFile);


        fseek(arq, (inicio + x) * tam_long, SEEK_SET);
        fwrite(&regTemp, tam, 1, arq);
    }

    fclose(tempFile);
}


void mergeSortDiscoPaciente(
    FILE *arq,
    int totalRegistros,
    FILE *log
) {
    clock_t inicio = clock();

    mergeSortRecursivoPaciente(arq, 0, totalRegistros - 1);

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    if (log != NULL) {
        fprintf(log, "\n------------------------------\n");
        fprintf(log, "Algoritmo: MergeSort (Pacientes por ID)\n");
        fprintf(log, "Registros ordenados: %d\n", totalRegistros);
        fprintf(log, "Tempo de execução: %.6f segundos\n", tempo);
        fprintf(log, "------------------------------\n");
    }
}
