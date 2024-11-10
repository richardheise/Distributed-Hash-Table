/***********************************************************************
*
* Autor: Richard Fernando Heise Ferreira
* Matrícula: 201900121214
* Data: 11/2024
* Instituição: Universidade Federal do Paraná
* Curso: Ciência da Computação
* Motivo: Trabalho da Disciplina de Desempenho de SGBD
*
************************************************************************/

#include "dht.hpp"
using namespace std;

int fingerTable_size = 0;

/***********************************************************************/

void addNode(DHT* dht, int newId) {

    // Verifica se já exisite o novo nodo
    if (dht->nodes.find(newId) != end(dht->nodes))
        throw logic_error("This node has already been added.");
    
    // Adição de novo nodo
    dht->nodes[newId] = {};

    // Movemos as chaves se for necessário
    moveKeys(dht, newId);
}

/***********************************************************************/

void moveKeys(DHT* dht, int newId) {

    // Achamos o próximo nodo usando lower_bound
    auto nextNode = findNode(dht, newId+1);

    // See o próximo é igual ao próprio novo nodo, então estamos no primeiro nodo,
    // não precisa mover nada
    if (nextNode == dht->nodes.find(newId)) { 
        return; 
    }
    // Não somos o primeiro nodo ->

    // Antecessor, porque não somos o primeiro
    auto prevNode = findPrevNode(dht, newId);

    // Movemos nossas chaves
    set<int> nextKeys;

    for (int key : nextNode->second.keyTable) {
        if (is_within_bounds(prevNode->first, key, newId)) {
            dht->nodes[newId].keyTable.insert(key);
        } else {
            nextKeys.insert(key);
        }
    }
    nextNode->second.keyTable = nextKeys;
}

/***********************************************************************/

map<int, node>::iterator findNode(DHT *dht, int key) {
    // Encontra o primeiro nodo cuja chave não é menor que key
    auto position = dht->nodes.lower_bound(key);

    // Retorna o iterador encontrado, ou, se chegar ao fim, retorna o primeiro nó (circular)
    return (position != dht->nodes.end()) ? position : dht->nodes.begin();
}

/***********************************************************************/


map<int, node>::iterator findPrevNode(DHT* dht, int nodeId) {
    // Encontra o iterador para o nodo atual
    auto it = dht->nodes.find(nodeId);

    // Se o iterador aponta para o primeiro elemento, retorna um iterador para o último elemento
    if (it == dht->nodes.begin()) {
        return prev(dht->nodes.end());
    }

    // Caso contrário, retorna o iterador para o elemento anterior a 'it'.
    return prev(it);
}

/***********************************************************************/

bool is_within_bounds(int start, int value, int end) {
    // Caso 1: Intervalo normal, não circular (start < end)
    if (start < end) {
        // Verifica se value está entre start e end
        return (value > start) && (value <= end);
    }

    // Caso 2: Intervalo circular (start >= end)
    // Verifica se valor está em [0, end] ou (start, max]
    return (value <= end) || (value > start);
}

/***********************************************************************/

void addKey(DHT* dht, int key) {
    if (dht->nodes.empty())
        throw logic_error("There are no nodes, can't add key");
    findNode(dht, key)->second.keyTable.insert(key);
}

/***********************************************************************/

void printNode(const node& n) {
    // Imprimir fingerTable
    cout << "Finger Table: ";
    for (int finger : n.fingerTable) {
        cout << finger << " ";
    }
    cout << endl;
    
    // Imprimir keyTable
    cout << "Key Table: ";
    for (int key : n.keyTable) {
        cout << key << " ";
    }
    cout << endl;
}

/***********************************************************************/

void printDHT(const DHT& dht) {
    if (dht.nodes.empty()) {
        return;
    }
    cout << "DHT Contents: " << endl;
    for (const auto& pair : dht.nodes) {
        cout << "Node ID: " << pair.first << endl;
        printNode(pair.second);
        cout << endl;
    }
}

/***********************************************************************/

void deleteNode(DHT* dht, int nodeId) {
    if (dht->nodes.find(nodeId) == end(dht->nodes))
        throw logic_error("Can't remove a node that was never inserted");

    // Guardamos as chaves removidas
    auto removedKeys = dht->nodes[nodeId].keyTable;
    dht->nodes.erase(nodeId);

    // Não temeos nada na DHT
    if (dht->nodes.empty()) {
        return;
    }

    // Mandamos a chave para o nodo apropriado
    auto destiny = findNode(dht, nodeId);
    for (int key : removed_keys) {
        destiny->second.keyTable.insert(key);
    }
}
