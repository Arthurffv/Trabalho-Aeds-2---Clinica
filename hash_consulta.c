#include "hash_consulta.h"

void criarTabelaHash(char* hashFileName, char* overflowFileName) {
    FILE *arq_Hash = fopen(hashFileName, "wb");
    THashNode nodeVazio;
    nodeVazio.dado.id = -1;
    nodeVazio.proximo = -1;

    for (int i = 0; i < TAMANHO_TABELA; i++) {
        fwrite(&nodeVazio, sizeof(THashNode), 1, arq_Hash);
    }
    fclose(arq_Hash);

    FILE *arq_Over = fopen(overflowFileName, "wb");
    long freeListHead = -1; // Cabeça da lista de espaços vazios
    fwrite(&freeListHead, sizeof(long), 1, arq_Over);
    fclose(arq_Over);
}

int funcaoHash(int id) {
    return abs(id) % TAMANHO_TABELA;
}

long obterPosicaoDisponivel(FILE *overflowFile) {
    long freeListHead;
    rewind(overflowFile);
    fread(&freeListHead, sizeof(long), 1, overflowFile);

    if (freeListHead == -1) {
        fseek(overflowFile, 0, SEEK_END);
        return ftell(overflowFile);
    }

    THashNode nodeExcluido;
    fseek(overflowFile, freeListHead, SEEK_SET);
    fread(&nodeExcluido, sizeof(THashNode), 1, overflowFile);
    
    rewind(overflowFile);
    fwrite(&nodeExcluido.proximo, sizeof(long), 1, overflowFile);
    
    return freeListHead;
}

void inserir_Hash(FILE *hashFile, FILE *overflowFile, Tconsulta *c) {
    int indice = funcaoHash(c->id);
    long posHash = indice * sizeof(THashNode);
    THashNode nodeAtual;

    fseek(hashFile, posHash, SEEK_SET);
    fread(&nodeAtual, sizeof(THashNode), 1, hashFile);

    if (nodeAtual.dado.id == -1) {
        nodeAtual.dado = *c;
        nodeAtual.proximo = -1;
        fseek(hashFile, posHash, SEEK_SET);
        fwrite(&nodeAtual, sizeof(THashNode), 1, hashFile);
    } else {
        long novaPos = obterPosicaoDisponivel(overflowFile);
        THashNode novoNode = {*c, -1};
        fseek(overflowFile, novaPos, SEEK_SET);
        fwrite(&novoNode, sizeof(THashNode), 1, overflowFile);

        long posUltimo = posHash;
        FILE *arqUltimo = hashFile;
        while (nodeAtual.proximo != -1) {
            posUltimo = nodeAtual.proximo;
            arqUltimo = overflowFile;
            fseek(overflowFile, posUltimo, SEEK_SET);
            fread(&nodeAtual, sizeof(THashNode), 1, overflowFile);
        }
        nodeAtual.proximo = novaPos;
        fseek(arqUltimo, posUltimo, SEEK_SET);
        fwrite(&nodeAtual, sizeof(THashNode), 1, arqUltimo);
    }
}

void remover_Hash(FILE *hashFile, FILE *overflowFile, int id) {
    int indice = funcaoHash(id);
    long posAtual = (long)indice * sizeof(THashNode);
    FILE *arqAtual = hashFile;
    THashNode atual, anterior;
    long posAnterior = -1;
    FILE *arqAnterior = NULL;

    fseek(arqAtual, posAtual, SEEK_SET);
    if (fread(&atual, sizeof(THashNode), 1, arqAtual) != 1 || atual.dado.id == -1) return;

    while (atual.dado.id != -1) {
        if (atual.dado.id == id) {
            if (arqAtual == hashFile) {
                if (atual.proximo != -1) {
                    THashNode proxNode;
                    fseek(overflowFile, atual.proximo, SEEK_SET);
                    fread(&proxNode, sizeof(THashNode), 1, overflowFile);
                    
                    long auxProx = atual.proximo;
                    long headAntiga;
                    rewind(overflowFile);
                    fread(&headAntiga, sizeof(long), 1, overflowFile);
                    
                    fseek(overflowFile, auxProx, SEEK_SET);
                    THashNode nodeParaFree;
                    nodeParaFree.proximo = headAntiga;
                    fwrite(&nodeParaFree, sizeof(THashNode), 1, overflowFile);
                    
                    rewind(overflowFile);
                    fwrite(&auxProx, sizeof(long), 1, overflowFile);

                    fseek(hashFile, posAtual, SEEK_SET);
                    fwrite(&proxNode, sizeof(THashNode), 1, hashFile);
                } else {
                    atual.dado.id = -1;
                    fseek(hashFile, posAtual, SEEK_SET);
                    fwrite(&atual, sizeof(THashNode), 1, hashFile);
                }
            } else {
                anterior.proximo = atual.proximo;
                fseek(arqAnterior, posAnterior, SEEK_SET);
                fwrite(&anterior, sizeof(THashNode), 1, arqAnterior);

                long headAntiga;
                rewind(overflowFile);
                fread(&headAntiga, sizeof(long), 1, overflowFile);
                atual.proximo = headAntiga;
                fseek(overflowFile, posAtual, SEEK_SET);
                fwrite(&atual, sizeof(THashNode), 1, overflowFile);
                rewind(overflowFile);
                fwrite(&posAtual, sizeof(long), 1, overflowFile);
            }
            return;
        }
        posAnterior = posAtual; arqAnterior = arqAtual; anterior = atual;
        if (atual.proximo == -1) break;
        posAtual = atual.proximo; arqAtual = overflowFile;
        fseek(overflowFile, posAtual, SEEK_SET);
        fread(&atual, sizeof(THashNode), 1, overflowFile);
    }
}

Tconsulta* buscar_Hash(FILE *hashFile, FILE *overflowFile, int id) {
    int indice = funcaoHash(id);
    long posAtual = (long)indice * sizeof(THashNode);
    FILE *arqAtual = hashFile;
    THashNode node;

    while (posAtual != -1) {
        fseek(arqAtual, posAtual, SEEK_SET);
        if (fread(&node, sizeof(THashNode), 1, arqAtual) != 1) break;
        if (node.dado.id == id) {
            Tconsulta *res = malloc(sizeof(Tconsulta));
            *res = node.dado;
            return res;
        }
        posAtual = node.proximo;
        arqAtual = overflowFile;
    }
    return NULL;
}