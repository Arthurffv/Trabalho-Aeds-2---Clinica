#ifndef BUSCAS_H
#define BUSCAS_H

#include <stdio.h>
#include <time.h>

#include "Paciente.h"
#include "Medico.h"
#include "consulta.h"


Tpaciente *Paciente_buscaSequencial_PorId(FILE *in, int chave, FILE *log);
Tmedico *Medico_buscaSequencial_PorId(FILE *in, int chave, FILE *log);
Tconsulta *Consulta_buscaSequencial_PorId(FILE *in, int chave, FILE *log);


Tpaciente *Paciente_buscaBinaria_PorId(FILE *in, int chave, int inicio, int fim, FILE *log);
Tmedico *Medico_buscaBinaria_PorId(FILE *in, int chave, int inicio, int fim, FILE *log);
Tconsulta *Consulta_buscaBinaria_PorId(FILE *in, int chave, int inicio, int fim, FILE *log);

#endif
