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

## 🛠️ Tecnologias Utilizadas<br>

- **Linguagem:** C (Padrão C99/C11)
- **Bibliotecas:** `stdio.h`, `stdlib.h`, `string.h`, `time.h`<br>

---<br>

## 📂 Estrutura de Arquivos<br>

O projeto foi estruturado de forma modular:<br>

| Arquivo | Descrição |<br>
| :--- | :--- |<br>
| `main.c` | Ponto de entrada, menus e orquestração do sistema. |<br>
| `Medico.c/h` | TAD para manipulação da entidade Médico. |<br>
| `Paciente.c/h` | TAD para manipulação da entidade Paciente. |<br>
| `consulta.c/h` | TAD para manipulação da entidade Consulta. |<br>
| `Buscas.c/h` | Implementação centralizada das buscas Sequencial e Binária. |<br>
| `MergeSort*.c/h` | Implementações do MergeSort Externo para cada entidade. |<br>
| `*.dat` | Arquivos binários gerados pelo sistema (Banco de Dados). |<br>
| `*.txt` | Logs de desempenho gerados automaticamente. |<br>

---<br>
Arthur Fernando Fernandes Vasconcelos Engenharia de Computação - Universidade Federal de Ouro Preto (UFOP)<br>
