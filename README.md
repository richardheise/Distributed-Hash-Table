##### Author: Richard Fernando Heise Ferreira  
##### Registration: 201900121214  
##### Date: 11/2024  
##### Institution: Federal University of Paraná  
##### Course: Graduate Program in Computer Security  
##### Purpose: Work for the Database Management System Performance course

# Distributed Hash Table

In this exercise, we will implement routing and storage for a Distributed Hash Table (DHT). The execution will be centralized, and our DHT is a simplification of the Chord DHT ["Ion Stoica, Robert Morris, David R. Karger, M. Frans Kaashoek, Hari Balakrishnan: Chord: A scalable peer-to-peer lookup service for internet applications. SIGCOMM 2001: 149-160"].

### Objective

A Distributed Hash Table can be used in various contexts, but mainly in distributed databases. The idea here is to allow a client to connect to any node and store/read data from the system transparently. Each node has a routing table, called a FingerTable, which is updated whenever a new node joins or leaves the network. Each node also has a KeyTable, where keys (data) are stored. There are no duplicate keys, and the system is circular, meaning the next node after the last node ID is the first node (the smallest ID in the network). Whenever a new node joins or leaves the system, its respective keys must be moved to a new node so that no data is lost.

### Hypothesis

It is theoretically possible to distribute a hash table indefinitely among N nodes and read/store data this way.

### Implementation

The program is divided into:

- `main.cpp`, which contains the function calls and input processing, as well as the processing function itself.
- `dht.cpp`, which contains the implementations of the functions used in this exercise.

A node has:
- ID
- FingerTable
- KeyTable

The DHT class has:
- `map<int, node> nodes`

Thus, the DHT stores only a red-black tree (map) of nodes, which simplifies access.

The notable functions are:

- `findNode(int nodeId)`:
    - This function finds the next node that should come after the nodeId passed by copy. It uses the `lower_bound()` function due to the use of `map<>`.
  
- `findPrevNode(int nodeId)`:
    - Finds the previous node to the nodeId passed by copy. It uses the `prev()` function for `map<>`.
  
- `fingerTableBits()`:
    - This function determines the number of bits required to represent the FingerTable size. It is given by `floor(log_2(maxId)) + 1`.
  
- `isWithinBounds(int start, int value, int end)`:
    - Checks if the value is between start and end in a circular network.

- `addKey(int key)`:
    - Here, we do not use the search to check where to insert the key. We simply insert it into the correct node using the `findNode()` function.
  
- `lookupKey(int time, int nodeId, int key)`:
    - This is the most complex function in the code. We print the path we take through the network to find the node that holds the key. Remember, we can search for a key through any node. The function used to find the next node is: `log2((min(pow(2, ftBits), key) - nodeId) % pow(2, ftBits) + pow(2, ftBits)) % pow(2, ftBits))`, which uses the distance between where we are and where we need to be to find the key modularly.

### Experiments

Use:

```bash
./execTests.sh
```

To run every test in the tests/ dir. There reulsts are then write into results/ dir and a diff is made between each result and each input.

### Conclusion

The C++ implementation was sufficient to demonstrate the simplified version of this Distributed Hash Table.

