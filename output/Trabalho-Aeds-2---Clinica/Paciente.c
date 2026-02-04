#include "Paciente.h"

Tpaciente *paciente(int id, char *nome, char *cpf, char *data_nascimento, char *telefone) {
    Tpaciente *p = (Tpaciente *) malloc(sizeof(Tpaciente));
    if (p) memset(p, 0, sizeof(Tpaciente));
    p->id = id;
    strcpy(p->nome, nome);
    strcpy(p->cpf, cpf);
    strcpy(p->data_nascimento, data_nascimento);
    strcpy(p->telefone, telefone);
    return p;
}

void salva_paciente(Tpaciente *p, FILE *out) {
    fwrite(&p->id, sizeof(int), 1, out);
    fwrite(p->nome, sizeof(char), sizeof(p->nome), out);
    fwrite(p->cpf, sizeof(char), sizeof(p->cpf), out);
    fwrite(p->data_nascimento, sizeof(char), sizeof(p->data_nascimento), out);
    fwrite(p->telefone, sizeof(char), sizeof(p->telefone), out);
}

Tpaciente *le_paciente(FILE *in) {
    Tpaciente *p = (Tpaciente *) malloc(sizeof(Tpaciente));
    if (0 >= fread(&p->id, sizeof(int), 1, in)) {
        free(p);
        return NULL;
    }
    fread(p->nome, sizeof(char), sizeof(p->nome), in);
    fread(p->cpf, sizeof(char), sizeof(p->cpf), in);
    fread(p->data_nascimento, sizeof(char), sizeof(p->data_nascimento), in);
    fread(p->telefone, sizeof(char), sizeof(p->telefone), in);
    return p;
}

size_t tamanho_registro_paciente() {
    return (size_t)(sizeof(int) + sizeof(char) * 50 + sizeof(char) * 15 + sizeof(char) * 11 + sizeof(char) * 15);
}

void criar_base_paciente_embaralhada(FILE *out, int tam) {
    int vet[tam];
    for (int i = 0; i < tam; i++) vet[i] = i + 1;


    for (int t = 0; t < tam; t++) {
        int i = rand() % tam, j = rand() % tam;
        int tmp = vet[i]; vet[i] = vet[j]; vet[j] = tmp;
    }
    for (int i = 0; i < tam; i++) {
        Tpaciente *p = paciente(vet[i], "Paciente", "000.000.000-00", "01/01/1990", "(00)00000-0000");
        salva_paciente(p, out);
        free(p);
    }
}

void listar_pacientes(FILE *in) {
    rewind(in);
    Tpaciente *p;
    while ((p = le_paciente(in)) != NULL) {
        printf("ID: %d | Nome: %s | CPF: %s | Nascimento: %s | Telefone: %s\n", p->id, p->nome, p->cpf, p->data_nascimento, p->telefone);
        free(p);
    }
}

Tpaciente *buscar_paciente_por_cpf(FILE *in, char *cpf) {
    rewind(in);
    Tpaciente *p;
    while ((p = le_paciente(in)) != NULL) {
        if (strcmp(p->cpf, cpf) == 0) return p;
        free(p);
    }
    return NULL;
}

void atualizar_paciente(FILE *in, char *cpf, Tpaciente *novo) {
    rewind(in);
    Tpaciente *p;
    while ((p = le_paciente(in)) != NULL) {
        if (strcmp(p->cpf, cpf) == 0) {
            fseek(in, -(long)tamanho_registro_paciente(), SEEK_CUR);
            salva_paciente(novo, in);
            free(p);
            break;
        }
        free(p);
    }
}

void excluir_paciente(FILE *in, char *cpf, FILE *temp) {
    rewind(in);
    Tpaciente *p;
    while ((p = le_paciente(in)) != NULL) {
        if (strcmp(p->cpf, cpf) != 0) salva_paciente(p, temp);
        free(p);
    }
}

