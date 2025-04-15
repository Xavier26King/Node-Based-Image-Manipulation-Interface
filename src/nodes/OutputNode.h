#pragma once
#include "../core/Node.h"
#include <string>

class OutputNode : public Node {
public:
    OutputNode();

    std::string savePath;
    bool saveRequested = false;

    void process() override;
    void renderUI() override;
};