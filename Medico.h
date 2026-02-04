#ifndef MEDICO_H_INCLUDED
#define MEDICO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Medico {
    int id;
    char nome[50];
    char crm[20];
    char especialidade[30];
    char telefone[15];
} Tmedico;

Tmedico *medico(int id, char *nome, char *crm, char *especialidade, char *telefone);

void salva_medico(Tmedico *m, FILE *out);
Tmedico *le_medico(FILE *in);

size_t tamanho_registro_medico();

void criar_base_medico_embaralhada(FILE *out, int tam);

void listar_medicos(FILE *in);

Tmedico *buscar_medico_por_crm(FILE *in, char *crm);

void atualizar_medico(FILE *in, char *crm, Tmedico *novo);

void excluir_medico(FILE *in, char *crm, FILE *temp);


#endif