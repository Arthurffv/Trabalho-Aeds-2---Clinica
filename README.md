# 🏥 Sistema de Gerenciamento de Clínica Médica <br>

> Trabalho Prático de **Algoritmos e Estruturas de Dados II (AEDS II)** > **Universidade Federal de Ouro Preto (UFOP)**<br>

## 📄 Sobre o Projeto</br>

Este projeto é uma implementação em linguagem **C** de um sistema de gerenciamento para uma clínica médica (Médicos, Pacientes e Consultas). <br>

O foco principal deste trabalho não é apenas a gestão de dados, mas sim a aplicação prática de conceitos avançados de **manipulação de arquivos binários**, **ordenação externa** e **análise de complexidade de algoritmos**. O sistema simula um ambiente onde os dados excedem a capacidade da memória RAM, exigindo leitura e escrita direta em disco.<br>

## 🎯 Objetivos Acadêmicos </br>

1.  **Persistência de Dados:** Implementação de arquivos binários (`.dat`) com registros de tamanho fixo.<br>
2.  **Acesso Aleatório:** Utilização de `fseek` para navegar diretamente para registros específicos sem leitura sequencial.<br>
3.  **Ordenação Externa:** Implementação do algoritmo **MergeSort Externo** para ordenar arquivos grandes.<br>
4.  **Análise de Desempenho:** Comparação empírica entre **Busca Sequencial** ($O(n)$) e **Busca Binária** ($O(\log n)$) medindo tempo de CPU e número de comparações.<br>

---<br>

## ⚙️ Funcionalidades<br>

### 1. Gestão de Dados (CRUD)<br>
O sistema gerencia três entidades principais:<br>
- **Médicos:** ID, Nome, CRM, Especialidade, Telefone.<br>
- **Pacientes:** ID, Nome, CPF, Data de Nascimento, Telefone.<br>
- **Consultas:** ID, Data, Hora, ID Paciente, ID Médico, Observações.<br>

Operações disponíveis:<br>
- Geração automática de bases de teste (1000 registros aleatórios).<br>
- Listagem completa.<br>
- Inclusão (Agendamento), Edição e Exclusão lógica/física de consultas.<br>

### 2. Algoritmos de Ordenação<br>
- **MergeSort Externo:** Divide os arquivos binários em blocos, ordena-os e intercala-os usando arquivos temporários. Essencial para permitir a busca binária.<br>
- Gera log de desempenho em `log_ordenacao.txt`.<br>

### 3. Algoritmos de Busca<br>
O sistema permite buscar registros por ID (ou CRM/CPF) usando duas abordagens:<br>
- **Busca Sequencial:** Lê o arquivo do início ao fim. Funciona com arquivos desordenados, mas é lenta.<br>
- **Busca Binária:** "Pula" pelo arquivo dividindo o espaço de busca. Extremamente rápida, mas exige arquivo ordenado.<br>
- Gera log de comparação em `log_buscas.txt`.<br>

---<br>

## 🔄 Principais Mudanças e Evoluções do Trabalho

Nesta versão do trabalho, o sistema passou por **evoluções significativas em relação à versão inicial**, especialmente no módulo de ordenação externa das **Consultas**.

### ✅ Implementação da Seleção por Substituição

- Implementado o algoritmo de **Seleção por Substituição** para geração de *runs* (corridas iniciais).
- Utiliza um **reservatório em memória** limitado (`memDisponivel`).
- Introduz o conceito de **congelamento de registros**, permitindo a criação de *runs* maiores que o tamanho da memória.
- Cada *run* é gravada em arquivos binários separados:
  - `run_consultas_0.dat`, `run_consultas_1.dat`, ...

📌 Resultados:
- Maior eficiência na fase inicial da ordenação.
- Redução do número de corridas quando comparado à ordenação interna simples.

---

### ✅ Implementação da Intercalação Ótima (F - 1)

- Implementada a **Intercalação Ótima**, onde até **F - 1 arquivos** são intercalados por rodada.
- O processo ocorre em múltiplas rodadas até restar apenas um arquivo final ordenado.
- Arquivos temporários são gerados no formato:
  - `temp_rodadaX_grupoY.dat`

📌 Benefícios:
- Redução do número total de passadas sobre o disco
- Melhor aproveitamento do fator de intercalação
- Maior desempenho para grandes volumes de dados

---

### ✅ Ordenação Completa Integrada

Foi criada uma função de alto nível que integra todo o processo:

1. Geração das *runs* com **Seleção por Substituição**
2. Intercalação das *runs* usando **Intercalação Ótima**
3. Geração do arquivo final:
   - `consultas_ordenadas.dat`

Todo o processo é cronometrado e registrado em log.

---

### ✅ Logs de Desempenho

O sistema gera automaticamente arquivos de log contendo:

- Tempo total de execução
- Número de comparações
- Quantidade de *runs* geradas
- Detalhes das rodadas de intercalação

Esses logs permitem **análise empírica do desempenho** dos algoritmos implementados.

---

## 🛠️ Tecnologias Utilizadas

- **Linguagem:** C (Padrão C99 / C11)
- **Bibliotecas:**  
  `stdio.h`, `stdlib.h`, `string.h`, `time.h`, `limits.h`

---

## 📂 Estrutura de Arquivos

| Arquivo | Descrição |
|------|---------|
| `main.c` | Ponto de entrada e menus |
| `Medico.c/h` | TAD Médico |
| `Paciente.c/h` | TAD Paciente |
| `consulta.c/h` | TAD Consulta |
| `Buscas.c/h` | Busca Sequencial e Binária |
| `SelecaoPorSubstituicao.c/h` | Geração de runs |
| `IntercalacaoOtimaConsulta.c/h` | Intercalação Ótima |
| `run_*.dat` | Corridas geradas |
| `consultas_ordenadas.dat` | Arquivo final ordenado |
| `*.txt` | Logs de desempenho |

---

## 👨‍🎓 Autor

**Arthur Fernando Fernandes Vasconcelos**  
Engenharia de Computação  
Universidade Federal de Ouro Preto (UFOP)
