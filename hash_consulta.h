#ifndef HASH_CONSULTA_H
#define HASH_CONSULTA_H

#include "consulta.h"

#define TAMANHO_TABELA 101

typedef struct HashNode {
    Tconsulta dado;
    long proximo; // Offset para o próximo nó ou para a Free List
} THashNode;

// Protótipos das funções de gerenciamento
void criarTabelaHash(char* hashFileName, char* overflowFileName);
int funcaoHash(int id);
long obterPosicaoDisponivel(FILE *overflowFile);
void inserir_Hash(FILE *hashFile, FILE *overflowFile, Tconsulta *c);
void remover_Hash(FILE *hashFile, FILE *overflowFile, int id);
Tconsulta* buscar_Hash(FILE *hashFile, FILE *overflowFile, int id);

#endif