##### Autor: Richard Fernando Heise Ferreira
##### Matrícula: 201900121214
##### Data: 11/2024
##### Instituição: Universidade Federal do Paraná
##### Curso: Programa de Pós-Graduação em Segurança da Computação
##### Motivo: Trabalho da disciplina de Desempenho de Sistemas de Gerenciamento de Banco de Dados

# Tabela Hash Distribuída

Neste exercício, iremos implementar o roteamento e armazenamento de uma Tabela Hash Distribuída (DHT). A execução será centralizada, e nossa DHT é uma simplificação da DHT Chord ["Ion Stoica, Robert Morris, David R. Karger, M. Frans Kaashoek, Hari Balakrishnan: Chord: A scalable peer-to-peer lookup service for internet applications. SIGCOMM 2001: 149-160"].

### Objetivo

Uma Tabela Hash Distribuída pode ser usada em diversos contextos, mas principalmente em bancos de dados distribuídos. A ideia aqui é permitir que um cliente se conecte a qualquer nó e armazene/leia dados do sistema de forma transparente. Cada nó possui uma tabela de roteamento, chamada de FingerTable, que é atualizada sempre que um novo nó entra ou sai da rede. Cada nó também possui uma KeyTable, onde as chaves (dados) são armazenadas. Não há chaves duplicadas e o sistema é circular, ou seja, o próximo nó após o último identificador de nó é o primeiro nó (o menor ID na rede). Sempre que um novo nó entra ou sai do sistema, suas respectivas chaves devem ser movidas para um novo nó, de modo que nenhum dado seja perdido.

### Hipótese

É possível distribuir uma tabela hash indefinidamente (teoricamente) entre N nós e ler/armazenar dados dessa forma.

### Implementação

O programa está dividido em:

- `main.cpp`, que contém a chamada de funções e o processamento de entrada, assim como a função de processamento propriamente dita.

- `dht.cpp`, que contém as implementações das funções utilizadas neste exercício.

Um nó possui:
- ID
- FingerTable
- KeyTable

A classe DHT possui:
- `map<int, node> nodes`

Dessa forma, a DHT armazena apenas uma árvore vermelha e preta (map) de nós, o que facilita o acesso.

As funções dignas de menção são:

- `findNode(int nodeId)`:
    - Esta função encontra o próximo nó que deve vir após o nodeId passado por cópia. Utiliza a função `lower_bound()` devido ao uso do `map<>`.

- `findPrevNode(int nodeId)`:
    - Encontra o nó anterior ao nodeId passado por cópia. Usa a função `prev()` para o `map<>`.

- `fingerTableBits()`:
    - Esta função determina a quantidade de bits necessária para representar o tamanho da FingerTable. É dada por `floor(log_2(maxId)) + 1`.

- `isWithinBounds(int start, int value, int end)`:
    - Verifica se o valor está entre start e end em uma rede circular.

- `addKey(int key)`:
    - Aqui, não usamos a busca para verificar onde inserir a chave. Apenas a inserimos no nó correto via a função `findNode()`.

- `lookupKey(int time, int nodeId, int key)`:
    - Esta é a função mais complexa do código. Imprimimos o caminho que percorremos na rede para encontrar o nó que possui a chave. Lembre-se, podemos buscar uma chave por meio de qualquer nó. A função usada para encontrar o próximo nó é:
    
\[
\log_2 \left( \left( \left( \min(2^{\text{ftBits}}, \text{key}) - \text{nodeId} \right) \mod 2^{\text{ftBits}} + 2^{\text{ftBits}} \right) \mod 2^{\text{ftBits}} \right)
\]
    que utiliza a distância entre onde estamos e onde precisamos estar para encontrar a chave de forma modular.

### Experimentos
Realizados pelo professor.

### Conclusão

A implementação em C++ foi suficiente para demonstrar a versão simplificada desta Tabela Hash Distribuída.

