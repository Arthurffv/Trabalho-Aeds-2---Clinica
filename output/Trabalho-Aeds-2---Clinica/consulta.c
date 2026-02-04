#include "consulta.h"

Tconsulta *consulta(int id, char *data, char *hora, int id_paciente, int id_medico, char *observacoes) {
    Tconsulta *c = (Tconsulta *) malloc(sizeof(Tconsulta));
    if (c) memset(c, 0, sizeof(Tconsulta));
    c->id = id;
    strcpy(c->data, data);
    strcpy(c->hora, hora);
    c->id_paciente = id_paciente;
    c->id_medico = id_medico;
    strcpy(c->observacoes, observacoes);
    return c;
}

void salva_consulta(Tconsulta *c, FILE *out) {
    fwrite(&c->id, sizeof(int), 1, out);
    fwrite(c->data, sizeof(char), sizeof(c->data), out);
    fwrite(c->hora, sizeof(char), sizeof(c->hora), out);
    fwrite(&c->id_paciente, sizeof(int), 1, out);
    fwrite(&c->id_medico, sizeof(int), 1, out);
    fwrite(c->observacoes, sizeof(char), sizeof(c->observacoes), out);
}

Tconsulta *le_consulta(FILE *in) {
    Tconsulta *c = (Tconsulta *) malloc(sizeof(Tconsulta));
    if (0 >= fread(&c->id, sizeof(int), 1, in)) {
        free(c);
        return NULL;
    }
    fread(c->data, sizeof(char), sizeof(c->data), in);
    fread(c->hora, sizeof(char), sizeof(c->hora), in);
    fread(&c->id_paciente, sizeof(int), 1, in);
    fread(&c->id_medico, sizeof(int), 1, in);
    fread(c->observacoes, sizeof(char), sizeof(c->observacoes), in);
    return c;
}

size_t tamanho_registro_consulta() {
    return (size_t)(sizeof(int) + sizeof(char) * 11 + sizeof(char) * 6 + sizeof(int) * 2 + sizeof(char) * 200);
}

void criar_base_consulta_embaralhada(FILE *out, int tam) {
    int vet[tam];
    for (int i = 0; i < tam; i++) vet[i] = i + 1;


    for (int t = 0; t < tam; t++) {
        int i = rand() % tam, j = rand() % tam;
        int tmp = vet[i]; vet[i] = vet[j]; vet[j] = tmp;
    }
    for (int i = 0; i < tam; i++) {
        Tconsulta *c = consulta(vet[i], "10/10/2025", "10:00", i+1, i+1, "Consulta de rotina");
        salva_consulta(c, out);
        free(c);
    }
}

void listar_consultas(FILE *in) {
    rewind(in);
    Tconsulta *c;
    while ((c = le_consulta(in)) != NULL) {
        printf("ID: %d | Data: %s | Hora: %s | Paciente: %d | Medico: %d | Obs: %s\n",
               c->id, c->data, c->hora, c->id_paciente, c->id_medico, c->observacoes);
        free(c);
    }
}

Tconsulta *buscar_consulta_por_data(FILE *in, char *data) {
    rewind(in);
    Tconsulta *c;
    while ((c = le_consulta(in)) != NULL) {
        if (strcmp(c->data, data) == 0) return c;
        free(c);
    }
    return NULL;
}

void atualizar_consulta(FILE *in, int id, Tconsulta *nova) {
    rewind(in);
    Tconsulta *c;
    while ((c = le_consulta(in)) != NULL) {
        if (c->id == id) {
            fseek(in, -(long)tamanho_registro_consulta(), SEEK_CUR);
            salva_consulta(nova, in);
            free(c);
            break;
        }
        free(c);
    }
}

void excluir_consulta(FILE *in, int id, FILE *temp) {
    rewind(in);
    Tconsulta *c;
    while ((c = le_consulta(in)) != NULL) {
        if (c->id != id) salva_consulta(c, temp);
        free(c);
    }
}

