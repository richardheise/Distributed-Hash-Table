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

#include <sstream>
#include "dht.cpp" // Inclui o arquivo de cabeçalho da DHT
using namespace std;

int main() {
    DHT dht; // Instanciação do objeto DHT
    string line;
    while (getline(cin, line)) {
        if (line.empty()) { continue; }
        istringstream iss(line);
        int timestamp;
        char operation;
        int nodeId;
        int key = -1; // Opcional, só é necessário para as operações I e L

        // Lê os 3 ou 4 campos da entrada
        iss >> timestamp >> operation >> nodeId;

        // Lê a chave apenas para operações de inserção ou lookup
        if (operation == 'I' || operation == 'L') {
            iss >> key; 
        }

        // Processa a operação
        switch (operation) {
            case 'E': // Add nodo
                // cout << "Fazendo inserção do nodo: " << nodeId << " time -" << timestamp << endl;
                dht.addNode(nodeId);
                break;
            case 'S': // Remove nodo
                // cout << "Fazendo remoção do nodo: " << nodeId << " time -" << timestamp << endl;
                dht.deleteNode(nodeId);
                break;
            case 'I': // Inclusão de chave
                // cout << "Fazendo adição da chave" << key <<  " no nodo: " << nodeId << " time -" << timestamp << endl;
                dht.addKey(key);
                break;
            case 'L': // Lookup de chave
                //  cout << "Fazendo lookup da chave " << key <<  " no nodo: " << nodeId << " time - " << timestamp << endl;
                dht.lookupKey(timestamp, nodeId, key);
                break;
            default:
                cerr << "Operação desconhecida: " << operation << endl;
                break;
        }
    }
    return 0;
}
