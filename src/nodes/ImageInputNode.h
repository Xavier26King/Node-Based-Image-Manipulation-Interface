#pragma once
#include "../core/Node.h"
#include <string>

// ImageInputNode class inherits from Node
class ImageInputNode : public Node {
public:
    ImageInputNode();

    std::string filePath;
    void process() override;
    void renderUI() override;
};