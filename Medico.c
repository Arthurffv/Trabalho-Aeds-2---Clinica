#include "Medico.h"

Tmedico *medico(int id, char *nome, char *crm, char *especialidade, char *telefone) {
    Tmedico *m = (Tmedico *) malloc(sizeof(Tmedico));
    if (m) memset(m, 0, sizeof(Tmedico));
    m->id = id;
    strcpy(m->nome, nome);
    strcpy(m->crm, crm);
    strcpy(m->especialidade, especialidade);
    strcpy(m->telefone, telefone);
    return m;
}

void salva_medico(Tmedico *m, FILE *out) {
    fwrite(&m->id, sizeof(int), 1, out);
    fwrite(m->nome, sizeof(char), sizeof(m->nome), out);
    fwrite(m->crm, sizeof(char), sizeof(m->crm), out);
    fwrite(m->especialidade, sizeof(char), sizeof(m->especialidade), out);
    fwrite(m->telefone, sizeof(char), sizeof(m->telefone), out);
}

Tmedico *le_medico(FILE *in) {
    Tmedico *m = (Tmedico *) malloc(sizeof(Tmedico));
    if (0 >= fread(&m->id, sizeof(int), 1, in)) {
        free(m);
        return NULL;
    }
    fread(m->nome, sizeof(char), sizeof(m->nome), in);
    fread(m->crm, sizeof(char), sizeof(m->crm), in);
    fread(m->especialidade, sizeof(char), sizeof(m->especialidade), in);
    fread(m->telefone, sizeof(char), sizeof(m->telefone), in);
    return m;
}

// CORREÇÃO: Tipo de retorno 'size_t'
size_t tamanho_registro_medico() {
    // Retorna o tamanho como size_t
    return (size_t)(sizeof(int) + sizeof(char) * 50 + sizeof(char) * 20 + sizeof(char) * 30 + sizeof(char) * 15);
}

void criar_base_medico_embaralhada(FILE *out, int tam) {
    int vet[tam];
    for (int i = 0; i < tam; i++) vet[i] = i + 1;

    // CORREÇÃO: 'srand' removido. Chame-o apenas uma vez na 'main'.
    // srand(time(NULL));

    for (int t = 0; t < tam; t++) {
        int i = rand() % tam, j = rand() % tam;
        int tmp = vet[i]; vet[i] = vet[j]; vet[j] = tmp;
    }
    for (int i = 0; i < tam; i++) {
        Tmedico *m = medico(vet[i], "Medico", "CRM0000", "Clinico Geral", "(00)00000-0000");
        salva_medico(m, out);
        free(m);
    }
}

void listar_medicos(FILE *in) {
    rewind(in);
    Tmedico *m;
    while ((m = le_medico(in)) != NULL) {
        printf("ID: %d | Nome: %s | CRM: %s | Especialidade: %s | Telefone: %s\n",
               m->id, m->nome, m->crm, m->especialidade, m->telefone);
        free(m);
    }
}

Tmedico *buscar_medico_por_crm(FILE *in, char *crm) {
    rewind(in);
    Tmedico *m;
    while ((m = le_medico(in)) != NULL) {
        if (strcmp(m->crm, crm) == 0) return m;
        free(m);
    }
    return NULL;
}

void atualizar_medico(FILE *in, char *crm, Tmedico *novo) {
    rewind(in);
    Tmedico *m;
    while ((m = le_medico(in)) != NULL) {
        if (strcmp(m->crm, crm) == 0) {
            fseek(in, -(long)tamanho_registro_medico(), SEEK_CUR); // cast (long)
            salva_medico(novo, in);
            free(m);
            break;
        }
        free(m);
    }
}

void excluir_medico(FILE *in, char *crm, FILE *temp) {
    rewind(in);
    Tmedico *m;
    while ((m = le_medico(in)) != NULL) {
        if (strcmp(m->crm, crm) != 0) salva_medico(m, temp);
        free(m);
    }
}

