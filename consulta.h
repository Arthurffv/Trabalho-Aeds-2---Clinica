#ifndef CONSULTA_H_INCLUDED
#define CONSULTA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Consulta {
    int id;
    char data[11];
    char hora[6];
    int id_paciente;
    int id_medico;
    char observacoes[200];
} Tconsulta;

Tconsulta *consulta(int id, char *data, char *hora, int id_paciente, int id_medico, char *observacoes);

void salva_consulta(Tconsulta *c, FILE *out);

Tconsulta *le_consulta(FILE *in);

size_t tamanho_registro_consulta();

void criar_base_consulta_embaralhada(FILE *out, int tam);

void listar_consultas(FILE *in);

Tconsulta *buscar_consulta_por_data(FILE *in, char *data);

void atualizar_consulta(FILE *in, int id, Tconsulta *nova);

void excluir_consulta(FILE *in, int id, FILE *temp);


#endif
