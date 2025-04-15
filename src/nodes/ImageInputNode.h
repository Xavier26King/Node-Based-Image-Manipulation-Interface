#pragma once
#include "../core/Node.h"
#include <string>

class ImageInputNode : public Node {
public:
    ImageInputNode();

    std::string filePath;
    void process() override;
    void renderUI() override;
};