#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

// Node class represents a base class for all nodes in the node editor
// It contains common properties and methods for processing and rendering nodes
class Node {
public:
    Node(const std::string& name) : name(name), isDirty(true) {} // Constructor initializes the node with a name and sets the dirty flag
    virtual ~Node() = default; // Virtual destructor for proper cleanup of derived classes

    std::string name;
    std::vector<Node*> inputs;
    cv::Mat output; 

    bool isDirty; // Flag to indicate if the node needs processing

    virtual void process() = 0; // Process the node's input and produce output
    virtual void renderUI() = 0; // Render the node's UI in the editor
};