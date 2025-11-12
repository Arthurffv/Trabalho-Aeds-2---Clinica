#ifndef PACIENTE_H_INCLUDED
#define PACIENTE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Paciente {
    int id;
    char nome[50];
    char cpf[15];
    char data_nascimento[11];
    char telefone[15];
} Tpaciente;

Tpaciente *paciente(int id, char *nome, char *cpf, char *data_nascimento, char *telefone);

void salva_paciente(Tpaciente *p, FILE *out);

Tpaciente *le_paciente(FILE *in);

size_t tamanho_registro_paciente();

void criar_base_paciente_embaralhada(FILE *out, int tam);

void listar_pacientes(FILE *in);

Tpaciente *buscar_paciente_por_cpf(FILE *in, char *cpf);

void atualizar_paciente(FILE *in, char *cpf, Tpaciente *novo);

void excluir_paciente(FILE *in, char *cpf, FILE *temp);


#endif
