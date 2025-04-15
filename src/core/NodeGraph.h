#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "Node.h"

class NodeGraph {
public:
    std::vector<Node*> nodes;

    void addNode(Node* node);
    void connect(Node* from, Node* to);
    void evaluate();  // run topological sort and process

private:
    std::vector<Node*> topologicalSort();
};