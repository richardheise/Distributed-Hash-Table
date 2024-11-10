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

#include <sstream>
#include "dht.hpp"
using namespace std;


// Função para ler e processar a entrada
void readInput(DHT* dht) {
    string line;
    while (getline(cin, line)) {
         if (line.empty()) { continue; }
        istringstream iss(line);
        int timestamp;
        char operation;
        int node_id;
        int key = -1; // Opcional, só é necessário para as operações I e L

        // Lê os 3 ou 4 campos da entrada
        iss >> timestamp >> operation >> node_id;

        if (operation == 'I' || operation == 'L') {
            iss >> key; // Lê a chave apenas para operações de inserção ou lookup
        }

        // Processa a operação
        switch (operation) {
            case 'E': // Add nodo
                addNode(dht, node_id);
                break;
            case 'S': // Remove nodo
                deleteNode(dht, node_id);
                break;
            case 'I': // Inclusão de chave
                addKey(dht, key);
                break;
            case 'L': // Lookup de chave
                // dht.lookupKey(node_id, key, timestamp);
                break;
            default:
                cerr << "Operação desconhecida: " << operation << endl;
                break;
        }
    }
}


int main() {
    DHT dht;

    readInput(&dht);
    printDHT(dht);
    return 0;
}
