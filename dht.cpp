/***********************************************************************
*
* Autor: Richard Fernando Heise Ferreira
* Matrícula: 201900121214
* Data: 11/2024
* Instituição: Universidade Federal do Paraná
* Curso: Mestrado em Segurança da Computação - PPG-Inf
* Motivo: Trabalho da Disciplina de Desempenho de SGBD
*
************************************************************************/

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <cmath>
#include <stdexcept>

using namespace std;

/** 
 * Classe nodo, consiste de um ID inteiro, um vetor (vector) de inteiros para a fingerTable 
 * e um conjunto de valores (pois não há repetição de chaves) na keyTable
**/
class Node {

/*************************************************************************
 * Funções privadas da classe Node
*************************************************************************/
public:
    int id;                              // Idenetificador do nodo
    vector<int> fingerTable;             // Tabela de roteamente
    set<int> keyTable;                   // Tabela de valores

     // Construtor padrão
    Node() : id(0) {}

    // Construtor com ID
    Node(int nodeId) : id(nodeId) {}

/************************************************************************/

    // Função que printa um nodo: id, fingerTable e keyTable, usada para depuração
    void print() const {
        // Imprimir fingerTable
        cout << "Finger Table: ";
        for (int finger : fingerTable) {
            cout << finger << " ";
        }
        cout << endl;
        
        // Imprimir keyTable
        cout << "Key Table: ";
        for (int key : keyTable) {
            cout << key << " ";
        }
        cout << endl;
    }

/************************************************************************/

};


/** 
 * Classe DHT, consiste de um map<int, Node> que são os nodos da rede
**/
class DHT {


/*************************************************************************
 * Funções privadas da classe DHT
*************************************************************************/

private:
    map<int, Node> nodes;                // Nodos da rede armazenados em árvore

/************************************************************************/

    // Função que computa quantos bits são necessários para a fingerTable
    int fingerTableBits() const {
        // Verificar se o mapa de nós está vazio
        if (nodes.empty()) {
            throw logic_error("Error: empty DHT, no bits to calculate.");
        }

        // Maior ID
        int maxId = rbegin(nodes)->first;

        // Verificar se o maior ID é válido (positivo)
        if (maxId < 0) {
            throw logic_error("Error: ID must be positive.");
        }

        // Se o ID é zero o cálculo abaixo não funciona
        if (maxId == 0) { 
            return 0;
        }

        // Calcular o número de bits necessários para a fingerTable
        // Usando log2 para garantir que o cálculo seja adequado
        int bits = static_cast<int>(floor(log2(maxId))) + 1;

        return bits;
    }

/************************************************************************/

    map<int, Node>::iterator findNode(int nodeId) {
        // Verificar se o mapa de nós está vazio
        if (nodes.empty()) {
            throw logic_error("Error: empty DHT, can't search for something that doesn't exist.");
        }

        // Buscar o nó mais próximo (menor ou igual à chave)
        auto position = nodes.lower_bound(nodeId);

        // Caso não tenha encontrado o nó (posição igual ao fim), retorna o primeiro nó
        return (position != nodes.end()) ? position : nodes.begin();
    }

/************************************************************************/

    map<int, Node>::iterator findPrevNode(int nodeId) {
        // Verificar se o mapa de nós está vazio
        if (nodes.empty()) {
            throw logic_error("Error: empty DHT, can't search for prev of something that doesn't exist.");
        }

        // Tentar encontrar o nó com o nodeId
        auto it = nodes.find(nodeId);
        
        // Verificar se o nó foi encontrado
        if (it == nodes.end()) {
            throw logic_error("Error: node not found for this id.");
        }

        // Se o nó encontrado for o primeiro da lista, retornar o último
        return (it == nodes.begin()) ? prev(nodes.end()) : prev(it);
    }

/************************************************************************/

    bool isWithinBounds(int start, int value, int end) const {
        // Se start > end, o valor precisa ser maior que start ou menor ou igual a end
        if (start > end) {
            return (value > start) || (value <= end);
        }

        // Caso contrário, o intervalo é normal [start, end]
        return (value > start) && (value <= end);
    }

/************************************************************************/

/*************************************************************************
 * Funções públicas da classe DHT
*************************************************************************/

public:

/************************************************************************/

    void addNode(int newId) {
        // Verificar se o novo ID é válido
        if (newId < 0) {
            throw logic_error("Error: node ID must be a positive integer.");
        }

        // Verificar se o nó já existe
        if (nodes.find(newId) != nodes.end()) {
            throw logic_error("Error: node already in the network.");
        }

        // Adicionar o novo nó
        nodes[newId] = Node(newId);

        // Gerar a finger table para todos os nós
        for (auto& node : nodes) {
            generateFingerTable(node.second.id);
        }

        // Mover as chaves para o novo nó
        moveKeys(newId);
    }

/************************************************************************/

    void moveKeys(int newId) {
        // Verifica se o novo nó existe
        if (nodes.find(newId) == nodes.end()) {
            throw logic_error("Error: The node with the specified ID does not exist.");
        }

        // Encontra o próximo nó
        auto nextNode = findNode(newId + 1);
        if (nextNode == nodes.find(newId)) {
            // Se o próximo nó for o nó atual, não há chaves para mover
            return;
        }

        // Encontra o nó anterior
        auto prevNode = findPrevNode(newId);

        // Verifica se o nó anterior foi encontrado
        if (prevNode == nodes.end()) {
            throw logic_error("Error: Could not find the previous node.");
        }
        // Verifica se o próximo nó foi encontrado
        if (nextNode == nodes.end()) {
            throw logic_error("Error: Could not find the next node.");
        }

        set<int> nextKeys;

        // Move as chaves para o novo nó
        for (int key : nextNode->second.keyTable) {
            if (isWithinBounds(prevNode->first, key, newId)) {
                nodes[newId].keyTable.insert(key);
            } else {
                nextKeys.insert(key);
            }
        }

        // Atualiza a tabela de chaves do próximo nó
        nextNode->second.keyTable = nextKeys;
    }

/************************************************************************/

    void addKey(int key) {
        // Verifica se há nós no DHT
        if (nodes.empty()) {
            throw logic_error("Error: There are no nodes in the DHT, cannot add key.");
        }

        // Verifica se o nó correspondente ao key foi encontrado
        auto nodeIterator = findNode(key);
        if (nodeIterator == nodes.end()) {
            throw logic_error("Error: The node corresponding to the key was not found.");
        }

        // Adiciona a chave ao nó encontrado
        nodeIterator->second.keyTable.insert(key);
    }

/************************************************************************/

    void deleteNode(int nodeId) {
        // Verifica se o nó existe no DHT antes de tentar removê-lo
        if (nodes.find(nodeId) == nodes.end()) {
            throw logic_error("Error: Cannot remove a node that was never inserted.");
        }

        // Armazena as chaves removidas do nó
        auto removedKeys = nodes[nodeId].keyTable;

        // Remove o nó do DHT
        nodes.erase(nodeId);

        // Se o DHT estiver vazio, não há necessidade de mais ações
        if (nodes.empty()) {
            return;
        }

        // Encontra o nó destino para mover as chaves removidas
        auto destiny = findNode(nodeId);
        if (destiny == nodes.end()) {
            throw logic_error("Error: The destination node for key reassignment could not be found.");
        }

        // Reatribui as chaves removidas ao nó destino
        for (int key : removedKeys) {
            destiny->second.keyTable.insert(key);
        }

        // Gerar a finger table para todos os nós
        for (auto& node : nodes) {
            generateFingerTable(node.second.id);
        }
    }

/************************************************************************/

    void generateFingerTable(int nodeId) {
        // Verifica se o DHT está vazio
        if (nodes.empty()) {
            throw logic_error("Error: The DHT is empty, cannot generate finger table.");
        }

        // Obtém o número de bits necessários para a finger table
        int ftBits = fingerTableBits();

        // Cria o vetor de finger table
        vector<int> fingers(ftBits);

        // Preenche a finger table com os IDs dos nós
        for (int i = 0; i < ftBits; i++) {
            int fingerId = (nodeId + (1 << i)) % (1 << ftBits);

            // Verifica se o nó correspondente ao fingerId existe
            auto fingerNode = findNode(fingerId);
            if (fingerNode == nodes.end()) {
                throw logic_error("Error: Finger node not found for fingerId " + to_string(fingerId));
            }

            fingers[i] = fingerNode->second.id;
        }

        // Atribui a finger table ao nó
        nodes[nodeId].fingerTable = fingers;
    }

/************************************************************************/

    void lookupKey(int time, int nodeId, int key) {
        // Verifica se o DHT está vazio
        if (nodes.empty()) {
            throw logic_error("Error: The DHT is empty, cannot perform key lookup.");
        }

        // Verifica se o nó inicial existe
        if (nodes.find(nodeId) == nodes.end()) {
            throw logic_error("Error: Node with ID " + to_string(nodeId) + " does not exist.");
        }

        // Inicia a pesquisa pela chave
        cout << time << " L " << key << " {";

        int ftBits = fingerTableBits();
        cout << nodeId;

        vector<Node> path;

        // Continua a busca até encontrar a chave
        while (!nodes[nodeId].keyTable.count(key)) {
            path.push_back(nodes[nodeId]);

            // Calcula o próximo nó a ser visitado
            int nextNode = __lg(((min((1 << ftBits), key) - nodeId) % (1 << ftBits) + (1 << ftBits)) % (1 << ftBits));

            nodeId = nodes[nodeId].fingerTable[nextNode];
            cout << "," << nodeId;
        }

        cout << "}\n";

        path.push_back(nodes[nodeId]);

        // Exibe o caminho percorrido
        for (const Node& node : path) {
            cout << time << " T " << node.id << " {";
            bool isFirst = true;
            for (int fingerId : node.fingerTable) {
                if (!isFirst) {
                    cout << ",";
                }
                cout << fingerId;
                isFirst = false;
            }
            cout << "}\n";
        }
    }

/************************************************************************/

    void print() const {
        if (nodes.empty()) {
            return;
        }
        cout << "DHT Contents: " << endl;
        for (const auto& pair : nodes) {
            cout << "Node ID: " << pair.first << endl;
            pair.second.print();
            cout << endl;
        }
    }
};