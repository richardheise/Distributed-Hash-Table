#include "dht.hpp"
using namespace std;

int fingerTable_size = 0;

void addNode(DHT* dht, int newId) {
    if (dht->nodes.find(newId) != end(dht->nodes))
        throw logic_error("This node has already been added.");
    
    dht->nodes[newId] = {};

    /*
    // moving the relevant keys from existing nodes
    auto next = lower_bound_circular(newId+1);

    // first node, nothing to move!
    if (next == dht->nodes.find(newId)) { return; }

    // not the first node, what comes before us?
    auto prev = prev_circular(newId);

    // moving the keys that are our own
    set<int> new_next_keys;
    for (int key : next->second.keys) {
        if (between_exclusive_inclusive_circular(prev->first, key, newId)) {
            dht->nodes[newId].keyTable.insert(key);
        } else {
            new_next_keys.insert(key);
        }
    }
    next->second.keys = new_next_keys;
    */
}

void printNode(const node& n) {
    cout << "Node ID: " << n.id << endl;
    
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

void printDHT(const DHT& dht) {
    cout << "DHT Contents: " << endl;
    for (const auto& pair : dht.nodes) {
        cout << "Node ID: " << pair.first << endl;
        printNode(pair.second);
        cout << endl;
    }
}