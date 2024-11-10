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
#include <iostream>
#include <cmath>
#include <vector> 
#include <map>
#include <cassert>
#include <set>


// Literalmente ninguém deixa de fazer isso
using namespace std;

/*********************************************************************
 * Estruturas do código
*********************************************************************/

// Controle de inclusão
#ifndef __DHT__
#define __DHT__

// Constantes
#define MEM_ERROR -1

// Tamanho da finger table muda de acordo com o maior ID sempre que adicionamos ou retiramos um nodo da rede
extern int fingerTable_size;

// Definição de macro para funções hash
#define FINGERID(n, k) ( (n + (int)pow(2, k - 1)) % pow(2, fingerTable_size) )

// Macro que recalcula o fingerTable Size a partir do maior ID da rede, dado por piso(log(MaiorID) + 1)
#define FINGERTABLE_VALUE(n) ( floor(log(n) + 1) )

// Um nodo possui id na rede, uma fingerTable de tamanho fingerTable_size e uma keyTable
struct node {
    int id;
    vector<int> fingerTable;
    set<int> keyTable;
};

// DHT é só uma lista de nodos, cada uma com um índice separado do ID individual
struct DHT {
    map<int, node> nodes;
};


/*********************************************************************
 * Funções
*********************************************************************/

void addNode(DHT* dht, int newId);
void printNode(const node& n);
void printDHT(const DHT& dht);
void moveKeys(DHT* dht, int newId);
map<int, node>::iterator findNode(DHT *dht, int key);
map<int, node>::iterator findPrevNode(DHT *dht, int node);
bool is_within_bounds(int start, int value, int end);
void addKey(DHT* dht, int key);
void deleteNode(DHT* dht, int nodeId);


#endif