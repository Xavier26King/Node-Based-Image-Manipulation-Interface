#include "NodeGraph.h"
#include <unordered_set>
#include <stack>
#include <iostream>

void NodeGraph::addNode(Node* node) {
    nodes.push_back(node);
}

void NodeGraph::connect(Node* from, Node* to) {
    to->inputs.push_back(from);
    to->isDirty = true;
}

void visit(Node* node, std::unordered_set<Node*>& visited, std::stack<Node*>& stack) {
    if (visited.count(node)) return;
    visited.insert(node);
    for (Node* input : node->inputs) {
        visit(input, visited, stack);
    }
    stack.push(node);
}

std::vector<Node*> NodeGraph::topologicalSort() {
    std::unordered_set<Node*> visited;
    std::stack<Node*> stack;

    for (Node* node : nodes) {
        visit(node, visited, stack);
    }

    std::vector<Node*> sorted;
    while (!stack.empty()) {
        sorted.push_back(stack.top());
        stack.pop();
    }
    return sorted;
}

void NodeGraph::evaluate() {
    auto sorted = topologicalSort();
    for (Node* node : sorted) {
        if (node->isDirty) {
            node->process();
            node->isDirty = false;
        }
    }
}