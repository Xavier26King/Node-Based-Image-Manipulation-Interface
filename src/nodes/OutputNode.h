#pragma once
#include "../core/Node.h"
#include <string>

// Include necessary libraries for OpenCV and ImGui
class OutputNode : public Node {
public:
    OutputNode();

    std::string savePath;
    bool saveRequested = false;

    void process() override;
    void renderUI() override;
};