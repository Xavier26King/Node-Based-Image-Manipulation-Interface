#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "Node.h"


// NodeGraph class manages a collection of nodes and their connections
// It provides methods to add nodes, connect them, and evaluate the graph
class NodeGraph {
public:
    std::vector<Node*> nodes;

    void addNode(Node* node);
    void connect(Node* from, Node* to);
    void evaluate();  // run topological sort and process

private:
    std::vector<Node*> topologicalSort();
};