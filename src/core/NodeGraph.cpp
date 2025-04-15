#include "NodeGraph.h"
#include <unordered_set>
#include <stack>
#include <iostream>

// NodeGraph class manages a collection of nodes and their connections
// It provides methods to add nodes, connect them, and evaluate the graph
void NodeGraph::addNode(Node* node) {
    nodes.push_back(node);
}

// Destructor for NodeGraph
// Cleans up all nodes in the graph
void NodeGraph::connect(Node* from, Node* to) {
    to->inputs.push_back(from); // Add the 'from' node to the 'to' node's inputs
    to->isDirty = true; // Mark the 'to' node as dirty to indicate it needs processing
}

// Destructor for NodeGraph
// Cleans up all nodes in the graph
void visit(Node* node, std::unordered_set<Node*>& visited, std::stack<Node*>& stack) {
    if (visited.count(node)) return; // If the node has already been visited, return
    visited.insert(node); // Mark the node as visited
    for (Node* input : node->inputs) {
        visit(input, visited, stack);
    } // Recursively visit all input nodes
    stack.push(node); // Push the node onto the stack after all its inputs have been visited
}

// Topological sort of the nodes in the graph
// This function orders the nodes in a way that respects their dependencies
std::vector<Node*> NodeGraph::topologicalSort() {
    std::unordered_set<Node*> visited;
    std::stack<Node*> stack;

    for (Node* node : nodes) {
        visit(node, visited, stack);
    } // Visit each node in the graph

    std::vector<Node*> sorted; // Vector to hold the sorted nodes
    while (!stack.empty()) {
        sorted.push_back(stack.top());
        stack.pop();
    } // Pop nodes from the stack to get them in topological order
    return sorted;
}

// Evaluate the graph by processing each node in topological order
void NodeGraph::evaluate() {
    auto sorted = topologicalSort(); // Get the sorted nodes
    for (Node* node : sorted) {
        if (node->isDirty) {
            node->process();
            node->isDirty = false;
        } // Process each node if it is dirty
    } // Mark the node as clean after processing
}