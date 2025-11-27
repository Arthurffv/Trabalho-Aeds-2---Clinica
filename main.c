#include "Medico.c"
#include "Paciente.c"
#include "consulta.c"
#include "Buscas.c"
#include "MergeSortMedico.c"
#include "MergeSortPaciente.c"
#include "MergeSortConsulta.c"
#include "SelecaoPorSubstituicao.c"
#include "IntercalacaoOtimaConsulta.c"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define ERROR(msg) fprintf(stderr, "\033[1;31m%s\033[0m\n", msg)

void limpar_tela_ansi()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausarTela()
{
    printf("\nPressione Enter para continuar...");
    limpar_buffer();
    getchar();
}

int main()
{
    srand(time(NULL));

    FILE *arq_medicos, *arq_pacientes, *arq_consultas;
    FILE *log_buscas, *log_ordenacao;

        if ((arq_medicos = fopen("medicos.dat", "wb+")) == NULL) {
            ERROR("Erro ao criar/abrir arquivo medicos.dat");
            exit(1);
        }

        if ((arq_pacientes = fopen("pacientes.dat", "wb+")) == NULL) {
            ERROR("Erro ao criar/abrir arquivo pacientes.dat");
            exit(1);
        }

        if ((arq_consultas = fopen("consultas.dat", "wb+")) == NULL) {
            ERROR("Erro ao criar/abrir arquivo consultas.dat");
            exit(1);
        }

    if ((log_buscas = fopen("log_buscas.txt", "a")) == NULL)
    {
        ERROR("Erro ao abrir arquivo log_buscas.txt");
        exit(1);
    }
    if ((log_ordenacao = fopen("log_ordenacao.txt", "a")) == NULL)
    {
        ERROR("Erro ao abrir arquivo log_ordenacao.txt");
        exit(1);
    }

    fseek(arq_medicos, 0, SEEK_END);
    int tamMed = ftell(arq_medicos) / tamanho_registro_medico();

    fseek(arq_pacientes, 0, SEEK_END);
    int tamPac = ftell(arq_pacientes) / tamanho_registro_paciente();

    fseek(arq_consultas, 0, SEEK_END);
    int tamCon = ftell(arq_consultas) / tamanho_registro_consulta();


    if (tamMed == 0 && tamPac == 0 && tamCon == 0)
    {
        printf("Bases de dados nao encontradas ou vazias. Criando novas...\n");
        tamMed = 1000;
        tamPac = 1000;
        tamCon = 1000;
        criar_base_medico_embaralhada(arq_medicos, tamMed);
        criar_base_paciente_embaralhada(arq_pacientes, tamPac);
        criar_base_consulta_embaralhada(arq_consultas, tamCon);
        printf("Bases criadas com sucesso!\n");
        pausarTela();
    } else {
        printf("Bases de dados carregadas com %d Medicos, %d Pacientes, %d Consultas.\n", tamMed, tamPac, tamCon);
        printf("\nPressione Enter para continuar...");
        getchar();
    }


    int sair = 1;
    do
    {
        limpar_tela_ansi();
        printf("--------------------------- MENU ---------------------------\n");
        printf("1 - Ordenar Bases (MergeSort Externo / Intercalacao)\n");
        printf("2 - Fazer Busca (Sequencial ou Binaria)\n");
        printf("3 - Listar Bases de Dados\n");
        printf("4 - Gerenciar Consultas (CRUD)\n");
        printf("0 - Sair\n");
        printf("------------------------------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &sair);

        switch (sair)
        {
        case 1:
        {
            limpar_tela_ansi();
            int esc1 = 0;
            do
            {
                printf("----------------- Ordenacao -----------------\n");
                printf("Qual base de dados deseja ordenar?\n");
                printf("1 - Medicos\n");
                printf("2 - Pacientes\n");
                printf("3 - Consultas\n");
                printf("4 - Voltar\n");
                printf("---------------------------------------------\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &esc1);

                if (esc1 == 4)
                    break;

                switch (esc1)
                {
                case 1:
                    limpar_tela_ansi();
                    printf("Iniciando MergeSort para Medicos (%d registros)...\n", tamMed);
                    rewind(arq_medicos);
                    mergeSortDiscoMedico(arq_medicos, tamMed, log_ordenacao);
                    printf("...Ordenacao de Medicos concluida!\n");
                    pausarTela();
                    break;
                case 2:
                    limpar_tela_ansi();
                    printf("Iniciando MergeSort para Pacientes (%d registros)...\n", tamPac);
                    rewind(arq_pacientes);
                    mergeSortDiscoPaciente(arq_pacientes, tamPac, log_ordenacao);
                    printf("...Ordenacao de Pacientes concluida!\n");
                    pausarTela();
                    break;
                
                case 3:
                {
                    limpar_tela_ansi();
                    int tipoAlg = 0;
                    printf("----------------- Algoritmo para Consultas -----------------\n");
                    printf("1 - MergeSort Padrao (Recursivo)\n");
                    printf("2 - Intercalacao Otima (Selecao com Substituicao)\n");
                    printf("------------------------------------------------------------\n");
                    printf("Escolha o algoritmo: ");
                    scanf("%d", &tipoAlg);

                    if (tipoAlg == 1) {
                        printf("\nIniciando MergeSort Padrao (%d registros)...\n", tamCon);
                        rewind(arq_consultas);
                        mergeSortDiscoConsulta(arq_consultas, tamCon, log_ordenacao);
                        printf("...Ordenacao de Consultas concluida!\n");
                    } 
                    else if (tipoAlg == 2) {
                        printf("\nIniciando Intercalacao Otima (%d registros)...\n", tamCon);
                        rewind(arq_consultas);
                        
                        int M = 50; 


                        ordenarPorIntercalacaoOtimaConsulta(arq_consultas, M, tamCon, log_ordenacao);

                        
                        fclose(arq_consultas);
                        remove("consultas.dat");
                        if (rename("consultas_ordenadas.dat", "consultas.dat") != 0) {
                             ERROR("Erro ao atualizar o arquivo principal consultas.dat");
                        }

                        arq_consultas = fopen("consultas.dat", "r+b");
                        if (!arq_consultas) {
                             ERROR("Erro Fatal: Nao foi possivel reabrir consultas.dat");
                             exit(1);
                        }

                        printf("...Ordenacao de Consultas (Intercalacao Otima) concluida!\n");
                    } 
                    else {
                        printf("Opcao invalida.\n");
                    }
                    pausarTela();
                    break;
                }

                default:
                    ERROR("Opcao invalida!");
                    pausarTela();
                    break;
                }
            } while (esc1 != 4);
            break;
        }

        case 2:
        {
            limpar_tela_ansi();
            int esc2 = 0;
            do
            {
                printf("-------------------- Tipo de Busca --------------------\n");
                printf("1 - Busca Sequencial\n");
                printf("2 - Busca Binaria\n");
                printf("3 - Voltar\n");
                printf("---------------------------------------------------------\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &esc2);


                if (esc2 == 3)
                    break;

                if (esc2 == 1)
                {
                    int esc_seq = 0;
                    do
                    {
                        limpar_tela_ansi();
                        printf("--- Busca Sequencial em qual base? ---\n");
                        printf("1 - Medicos\n");
                        printf("2 - Pacientes\n");
                        printf("3 - Consultas\n");
                        printf("4 - Voltar\n");
                        printf("--------------------------------------\n");
                        printf("Escolha uma opcao: ");
                        scanf("%d", &esc_seq);

                        if (esc_seq == 4)
                            break;

                        int id_busca;
                        printf("Digite o ID que deseja buscar: ");
                        scanf("%d", &id_busca);

                        switch (esc_seq)
                        {
                        case 1:
                        {
                            Tmedico *med = Medico_buscaSequencial_PorId(arq_medicos, id_busca, log_buscas);
                            if (med)
                            {
                                printf("--- Medico Encontrado! ---\n");
                                printf("ID: %d | Nome: %s | CRM: %s\n", med->id, med->nome, med->crm);
                                free(med);
                            }
                            else
                            {
                                ERROR("Medico nao encontrado.");
                            }
                            pausarTela();
                            break;
                        }
                        case 2:
                        {
                            Tpaciente *pac = Paciente_buscaSequencial_PorId(arq_pacientes, id_busca, log_buscas);
                            if (pac)
                            {
                                printf("--- Paciente Encontrado! ---\n");
                                printf("ID: %d | Nome: %s | CPF: %s\n", pac->id, pac->nome, pac->cpf);
                                free(pac);
                            }
                             else
                            {
                                ERROR("Paciente nao encontrado.");
                            }
                            pausarTela();
                            break;
                        }
                        case 3:
                        {
                            Tconsulta *con = Consulta_buscaSequencial_PorId(arq_consultas, id_busca, log_buscas);
                            if (con)
                            {
                                printf("--- Consulta Encontrada! ---\n");
                                printf("ID: %d | Data: %s | ID Paciente: %d | ID Medico: %d\n", con->id, con->data, con->id_paciente, con->id_medico);
                                free(con);
                            }
                             else
                            {
                                ERROR("Consulta nao encontrada.");
                            }
                            pausarTela();
                            break;
                        }
                        }
                    } while (esc_seq != 4);
                }
                else if (esc2 == 2)
                {
                    int esc_bin = 0;
                    do
                    {
                        limpar_tela_ansi();
                        printf("--- Busca Binaria em qual base? ---\n");
                        printf("(Lembre-se: A base DEVE estar ordenada!)\n");
                        printf("1 - Medicos\n");
                        printf("2 - Pacientes\n");
                        printf("3 - Consultas\n");
                        printf("4 - Voltar\n");
                        printf("-----------------------------------\n");
                        printf("Escolha uma opcao: ");
                        scanf("%d", &esc_bin);


                        if (esc_bin == 4)
                            break;

                        int id_busca;
                        printf("Digite o ID que deseja buscar: ");
                        scanf("%d", &id_busca);

                        switch (esc_bin)
                        {
                        case 1:
                        {
                            Tmedico *med = Medico_buscaBinaria_PorId(arq_medicos, id_busca, 0, tamMed - 1, log_buscas);
                            if (med)
                            {
                                printf("--- Medico Encontrado! ---\n");
                                printf("ID: %d | Nome: %s | CRM: %s\n", med->id, med->nome, med->crm);
                                free(med);
                            }
                            else
                            {
                                ERROR("Medico nao encontrado.");
                            }
                            pausarTela();
                            break;
                        }
                        case 2:
                        {
                            Tpaciente *pac = Paciente_buscaBinaria_PorId(arq_pacientes, id_busca, 0, tamPac - 1, log_buscas);
                            if (pac)
                            {
                                printf("--- Paciente Encontrado! ---\n");
                                printf("ID: %d | Nome: %s | CPF: %s\n", pac->id, pac->nome, pac->cpf);
                                free(pac);
                            }
                             else
                            {
                                ERROR("Paciente nao encontrado.");
                            }
                            pausarTela();
                            break;
                        }
                        case 3:
                        {
                            Tconsulta *con = Consulta_buscaBinaria_PorId(arq_consultas, id_busca, 0, tamCon - 1, log_buscas);
                            if (con)
                            {
                                printf("--- Consulta Encontrada! ---\n");
                                printf("ID: %d | Data: %s | ID Paciente: %d | ID Medico: %d\n", con->id, con->data, con->id_paciente, con->id_medico);
                                free(con);
                            }
                             else
                            {
                                ERROR("Consulta nao encontrada.");
                            }
                            pausarTela();
                            break;
                        }
                        }
                    } while (esc_bin != 4);
                }

            } while (esc2 != 3);
            break;
        }

        case 3:
        {
            limpar_tela_ansi();
            int esc3 = 0;
            do
            {
                printf("----------------- Listar Bases de Dados -----------------\n");
                printf("Qual base de dados deseja listar?\n");
                printf("1 - Medicos\n");
                printf("2 - Pacientes\n");
                printf("3 - Consultas\n");
                printf("4 - Voltar\n");
                printf("---------------------------------------------------------\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &esc3);

                limpar_tela_ansi();
                switch (esc3)
                {
                case 1:
                    printf("--- Listando Medicos ---\n");
                    listar_medicos(arq_medicos);
                    pausarTela();
                    break;
                case 2:
                    printf("--- Listando Pacientes ---\n");
                    listar_pacientes(arq_pacientes);
                    pausarTela();
                    break;
                case 3:
                    printf("--- Listando Consultas ---\n");
                    listar_consultas(arq_consultas);
                    pausarTela();
                    break;
                case 4:
                    break;
                default:
                    ERROR("Opcao invalida!");
                    pausarTela();
                    break;
                }
            } while (esc3 != 4);
            break;
        }

        case 4:
        {
            limpar_tela_ansi();
            int esc4 = 0;
            do
            {
                printf("----------------- Gerenciar Consultas -----------------\n");
                printf("1 - Agendar (Criar) Nova Consulta\n");
                printf("2 - Editar (Atualizar) Observacoes da Consulta\n");
                printf("3 - Cancelar (Excluir) Consulta\n");
                printf("4 - Voltar\n");
                printf("-------------------------------------------------------\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &esc4);
                
                // Limpeza basica de buffer antes de entrar nos cases que usam fgets
                limpar_buffer();

                switch (esc4)
                {
                case 1: // CRIAR
                {
                    limpar_tela_ansi();
                    printf("--- Agendar Nova Consulta ---\n");
                    int id_pac, id_med;
                    char data[11], hora[6], obs[200];

                    printf("Digite o ID do Paciente: ");
                    scanf("%d", &id_pac);
                    limpar_buffer();

                    Tpaciente *pac = Paciente_buscaSequencial_PorId(arq_pacientes, id_pac, log_buscas);
                    if (pac == NULL)
                    {
                        ERROR("Paciente nao encontrado! Agendamento cancelado.");
                        pausarTela();
                        break;
                    }
                    printf("Paciente encontrado: %s\n", pac->nome);
                    free(pac);

                    printf("Digite o ID do Medico: ");
                    scanf("%d", &id_med);
                    limpar_buffer();

                    Tmedico *med = Medico_buscaSequencial_PorId(arq_medicos, id_med, log_buscas);
                    if (med == NULL)
                    {
                        ERROR("Medico nao encontrado! Agendamento cancelado.");
                        pausarTela();
                        break;
                    }
                    printf("Medico encontrado: %s\n", med->nome);
                    free(med);

                    printf("Digite a Data (DD/MM/AAAA): ");
                    scanf("%10s", data);
                    limpar_buffer();

                    printf("Digite a Hora (HH:MM): ");
                    scanf("%5s", hora);
                    limpar_buffer();

                    printf("Digite as Observacoes (max 199 chars): ");
                    fgets(obs, 199, stdin);
                    obs[strcspn(obs, "\n")] = 0;

                    int novo_id = tamCon + 1;
                    Tconsulta *nova_con = consulta(novo_id, data, hora, id_pac, id_med, obs);

                    fseek(arq_consultas, 0, SEEK_END);
                    salva_consulta(nova_con, arq_consultas);
                    tamCon++;

                    printf("\nConsulta agendada com sucesso! ID: %d\n", novo_id);
                    free(nova_con);
                    pausarTela();
                    break;
                }
                case 2: // ATUALIZAR
                {
                    limpar_tela_ansi();
                    printf("--- Editar Observacoes da Consulta ---\n");
                    int id_con;
                    printf("Digite o ID da consulta a ser editada: ");
                    scanf("%d", &id_con);
                    limpar_buffer();

                    Tconsulta *con_antiga = Consulta_buscaSequencial_PorId(arq_consultas, id_con, log_buscas);
                    if (con_antiga == NULL)
                    {
                        ERROR("Consulta nao encontrada!");
                        pausarTela();
                        break;
                    }

                    printf("Consulta encontrada:\n");
                    printf("ID: %d | Data: %s | Obs Antiga: %s\n", con_antiga->id, con_antiga->data, con_antiga->observacoes);

                    char novas_obs[200];
                    printf("\nDigite as NOVAS Observacoes (max 199 chars): ");
                    fgets(novas_obs, 199, stdin);
                    novas_obs[strcspn(novas_obs, "\n")] = 0;

                    strcpy(con_antiga->observacoes, novas_obs);
                    atualizar_consulta(arq_consultas, id_con, con_antiga);

                    printf("Observacoes atualizadas com sucesso!\n");
                    free(con_antiga);
                    pausarTela();
                    break;
                }
                case 3: // EXCLUIR
                {
                    limpar_tela_ansi();
                    printf("--- Cancelar (Excluir) Consulta ---\n");
                    int id_con;
                    printf("Digite o ID da consulta a ser excluida: ");
                    scanf("%d", &id_con);
                    limpar_buffer();

                    Tconsulta *con_check = Consulta_buscaSequencial_PorId(arq_consultas, id_con, log_buscas);
                    if (con_check == NULL)
                    {
                        ERROR("Consulta nao encontrada!");
                        pausarTela();
                        break;
                    }
                    free(con_check);

                    FILE *temp_con = fopen("consultas.tmp", "wb");
                    if (temp_con == NULL)
                    {
                        ERROR("Erro ao criar arquivo temporario!");
                        pausarTela();
                        break;
                    }

                    excluir_consulta(arq_consultas, id_con, temp_con);

                    fclose(arq_consultas);
                    fclose(temp_con);

                    if (remove("consultas.dat") != 0) {
                        ERROR("Erro ao remover consultas.dat");
                    }
                    if (rename("consultas.tmp", "consultas.dat") != 0) {
                        ERROR("Erro ao renomear consultas.tmp");
                    }

                    arq_consultas = fopen("consultas.dat", "r+b");
                    if (arq_consultas == NULL)
                    {
                        ERROR("ERRO FATAL: Nao foi possivel reabrir consultas.dat!");
                        exit(1);
                    }

                    tamCon--;
                    printf("Consulta ID %d excluida com sucesso!\n", id_con);
                    pausarTela();
                    break;
                }
                case 4:
                    break;
                default:
                    ERROR("Opcao invalida!");
                    pausarTela();
                    break;
                }
            } while (esc4 != 4);
            break;
        }

        case 0:
            printf("\nEncerrando o programa...\n");
            break;

        default:
            ERROR("Opcao invalida! Tente novamente.");
            pausarTela();
            break;
        }

    } while (sair != 0);

    printf("Fechando arquivos.\n");
    fclose(arq_medicos);
    fclose(arq_pacientes);
    fclose(arq_consultas);
    fclose(log_buscas);
    fclose(log_ordenacao);

    return 0;
}